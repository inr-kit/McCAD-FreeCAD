#include "PreCompiled.h"
#include "McCadSurfaceCard.h"

#include <Base/UnitsApi.h>
#include <App/Document.h>
#include <App/DocumentObject.h>
#include <App/Application.h>
#include <App/Annotation.h>

#include <Handle_TDocStd_Document.hxx>
#include <Mod/Part/App/FeaturePartBox.h>
#include <Mod/Part/App/FeatureFace.h>

#include <Gui/Document.h>
#include <Gui/View3DInventor.h>
#include <Gui/Application.h>

#include <Gui/NavigationStyle.h>
#include <Gui/SoFCUnifiedSelection.h>
#include <Gui/ViewProviderDocumentObject.h>
#include <Gui/ViewProviderDocumentObjectGroup.h>
#include <Gui/ViewProviderGeometryObject.h>
#include <Gui/ViewProviderAnnotation.h>

#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/SoPickedPoint.h>

#include <Base/Console.h>
#include <BRep_Tool.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>

#include <GeomAdaptor_Surface.hxx>

#include "Convertor/IGeomFace.hxx"
#include "Convertor/McCadGeomTorus.hxx"
#include "Convertor/McCadGeomPlane.hxx"
#include "Convertor/McCadGeomCylinder.hxx"
#include "Convertor/McCadGeomCone.hxx"
#include "Convertor/McCadGeomRevolution.hxx"
#include "Convertor/McCadGeomSphere.hxx"

#include <Base/Placement.h>

string McCadSurfaceCard::cs = "";

McCadSurfaceCard::McCadSurfaceCard()
{
    activeCB = NULL;


//    App::PropertyType type = static_cast<App::PropertyType>(App::Prop_ReadOnly);
//    ADD_PROPERTY_TYPE(Height,(1),"Structured points", App::Prop_None, "Height of the image");

    //ADD_PROPERTY_TYPE(MaxSize,(1000),       "MeshParams",Prop_None,"Maximum element size");
}

McCadSurfaceCard::~McCadSurfaceCard()
{  
    stopInteractiveCallback();
}


void McCadSurfaceCard::startInteractiveCallback()
{
    if (this->activeCB)
        return;

    Gui::View3DInventorViewer* viewer = getViewer();
    if(viewer)
    {
        viewer->setEditing(true);
        viewer->addEventCallback(SoMouseButtonEvent::getClassTypeId(),SelectSurfaceCallback,this);
        //viewer->navigationStyle()->stopSelection();
        this->activeCB = SelectSurfaceCallback;

        App::Document* pcDoc = App::GetApplication().getActiveDocument();
        App::Annotation* anno = static_cast<App::Annotation*>
                (pcDoc->addObject("App::Annotation", "Surf_Info"));
        Gui::Document *pDoc = Gui::Application::Instance->activeDocument();
        Gui::ViewProviderAnnotation *vp = static_cast<Gui::ViewProviderAnnotation*>(pDoc->getViewProvider(anno));

        cs = vp->getObject()->getNameInDocument();

        if (vp && vp->isVisible())
        {
            vp->FontSize.setValue(20);
            vp->TextColor.setValue(1.0f,0.0f,0.0f,0.5f);
        }
    }
}


void McCadSurfaceCard::stopInteractiveCallback()
{
    if (!this->activeCB)
        return;

    Gui::View3DInventorViewer* viewer = this->getViewer();

    viewer->setEditing(false);
    viewer->removeEventCallback(SoMouseButtonEvent::getClassTypeId(), this->activeCB, this);
    this->activeCB = 0;

    App::Document* pcDoc = App::GetApplication().getActiveDocument();
    if (pcDoc)
    {
        vector<App::DocumentObject*> list = pcDoc->findObjects(App::Annotation::getClassTypeId(),"Surf_Info");
        if ( list.size() != 0)
        {
            pcDoc->remObject(cs.c_str());
            list.clear();
        }
    } 
}

Gui::View3DInventorViewer* McCadSurfaceCard::getViewer() const
{
    Gui::Document* doc = Gui::Application::Instance->activeDocument();
    if (!doc) return 0;
    Gui::MDIView* view = doc->getActiveView();
    if (view && view->getTypeId().isDerivedFrom(Gui::View3DInventor::getClassTypeId())) {
        Gui::View3DInventorViewer* viewer = static_cast<Gui::View3DInventor*>(view)->getViewer();
        return viewer;
    }

    return 0;
}


void McCadSurfaceCard::SelectSurfaceCallback(void * ud, SoEventCallback * n)
{
    const SoMouseButtonEvent * mbe = static_cast<const SoMouseButtonEvent*>(n->getEvent());
    Gui::View3DInventorViewer* view  = reinterpret_cast<Gui::View3DInventorViewer*>(n->getUserData());
    const SoPickedPoint * point = n->getPickedPoint();

    if (mbe->getButton() == SoMouseButtonEvent::BUTTON1 && mbe->getState() == SoButtonEvent::DOWN)
    {        
        if (point == NULL)
        {
            Base::Console().Message("No point picked.\n");
            return;
        }
    }
    else if(mbe->getButton() == SoMouseButtonEvent::BUTTON2 && mbe->getState() == SoButtonEvent::UP)
    {
        view->setEditing(false);
        view->removeEventCallback(SoMouseButtonEvent::getClassTypeId(),SelectSurfaceCallback, ud);

        App::Document* pcDoc = App::GetApplication().getActiveDocument();
        if (pcDoc)
        {
            vector<App::DocumentObject*> list = pcDoc->findObjects(App::Annotation::getClassTypeId(),"Surf_Info");
            if ( list.size() != 0)
            {
               // activeVP->getNameInDocument();

                pcDoc->remObject("Surf_Info");
                list.clear();
            }
        }

        return;
    }
    else
    {
        return;
    }

    SoRayPickAction action(view->getSoRenderManager()->getViewportRegion());

    const SoEvent * ev = n->getEvent();
    const SoMouseButtonEvent * const event = (const SoMouseButtonEvent *) ev;

    SbVec2s pos = event->getPosition();
    action.setPickAll(true);
    action.setPoint(pos);
    action.apply(view->getSoRenderManager()->getSceneGraph());

    //const SoPickedPointList& pp = action.getPickedPointList();
    const SoPickedPoint * p1 = action.getPickedPoint();

    if(!p1)
    {
        return;
    }

    Gui::ViewProvider* vp = static_cast<Gui::ViewProvider*>(view->getViewProviderByPath(p1->getPath()));

    if (vp && vp->getTypeId().isDerivedFrom(Gui::ViewProviderGeometryObject::getClassTypeId()))
    {
        Gui::ViewProviderGeometryObject* vpd = static_cast<Gui::ViewProviderGeometryObject*>(vp);
        App::DocumentObject *obj =  vpd->getObject();

        if(obj && obj->getTypeId().isDerivedFrom(Part::Feature::getClassTypeId()))
        {
            std::string element = vp->getElement(p1->getDetail());
            const Part::TopoShape& shape = static_cast<Part::Feature*>(obj)->Shape.getShape();
            TopoDS_Shape subShape = shape.getSubShape(element.c_str());

            if(!subShape.IsNull() && subShape.ShapeType() == TopAbs_FACE)
            {
                TopoDS_Face theFace = TopoDS::Face(subShape);
                string str = GetSurfEquation(theFace);
                Base::Console().Message(str.c_str());


                App::Document* pcDoc = App::GetApplication().getActiveDocument();
                if (!pcDoc)
                    return;

                vector<App::DocumentObject*> list = pcDoc->findObjects(App::Annotation::getClassTypeId(),"Surf_Info");
                SbVec3f pn = point->getPoint();

                if ( list.size() != 0)
                {
                    for (vector<App::DocumentObject*>::const_iterator iter = list.begin();iter != list.end();++iter)
                    {
                        if ((*iter)->getTypeId().isDerivedFrom(App::Annotation::getClassTypeId()))
                        {
                            App::Annotation* an = static_cast<App::Annotation*>(*iter);
                            an->LabelText.setValue(str.c_str());
                            an->Position.setValue(pn[0],pn[1],pn[2]);
                        }
                    }
                }
                else
                {
                    return;
                }
            }
        }
    }
}



string McCadSurfaceCard::GetSurfEquation(TopoDS_Face &theFace)
{
    TopLoc_Location loc;
    Handle(Geom_Surface) geom_surface = BRep_Tool::Surface(theFace, loc);
    GeomAdaptor_Surface surf_adoptor(geom_surface);

    /* Generate the geometry surface interface based on the type of sufaces */
    IGeomFace * pGeomFace = NULL;
    switch (surf_adoptor.GetType())
    {
        case GeomAbs_Plane:
        {
            pGeomFace = new McCadGeomPlane(surf_adoptor);
            break;
        }
        case GeomAbs_Cylinder:
        {
            pGeomFace = new McCadGeomCylinder(surf_adoptor);
            break;
        }
        case GeomAbs_Cone:
        {
            pGeomFace = new McCadGeomCone(surf_adoptor);
            break;
        }
        case GeomAbs_Sphere:
        {
            pGeomFace = new McCadGeomSphere(surf_adoptor);
            break;
        }
        case GeomAbs_Torus:
        {
            pGeomFace = new McCadGeomTorus(surf_adoptor);
            break;
        }
        case GeomAbs_SurfaceOfRevolution:
        {
            pGeomFace = new McCadGeomRevolution(surf_adoptor);
            break;
        }
        case GeomAbs_BezierSurface:
        {
            cout << "Surface is not analytic (BezierSurface) " << endl;
            break;
        }
        case GeomAbs_BSplineSurface:
        {
            cout << "Surface is not analytic (BSplineSurface) " << endl;
            break;
        }
        case GeomAbs_SurfaceOfExtrusion:
        {
            cout << "Surface is not analytic (SurfaceOfRevolution) " << endl;
            break;
        }
        case GeomAbs_OffsetSurface:
        {
            cout << "Surface is not analytic (OffsetSurface) " << endl;
            break;
        }
        case GeomAbs_OtherSurface:
        {
            cout << "Surface is not analytic " << endl;
            break;
        }
        default:break;
    }

    if (pGeomFace)
    {
        std::ostringstream oStream;
        oStream<<pGeomFace->GetSurfSymb();          // The symbol of surface type

        oStream.setf(ios::fixed);
        oStream<<setprecision(7);
        for (unsigned int i = 0; i < (pGeomFace->GetPrmtList()).size(); i++ )
        {
            //if ( i%3 == 0 && i != 0 )
            //{
            //    oStream<<endl;
            //    oStream<<setw(6)<<"";
            //}
            oStream.unsetf(ios::left);
            oStream.setf(ios::right);
            oStream<<setw(15)<<pGeomFace->GetPrmtList().at(i)<<" ";
        }
        oStream<<endl;
        oStream.unsetf(ios::right);
        return oStream.str();
    }
    else
    {
        return "";
    }
}
