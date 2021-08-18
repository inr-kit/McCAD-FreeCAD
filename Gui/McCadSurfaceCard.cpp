#include "PreCompiled.h"
#include "McCadSurfaceCard.h"

#include <Base/UnitsApi.h>
#include <App/Document.h>
#include <App/DocumentObject.h>
#include <App/Application.h>

#include <Handle_TDocStd_Document.hxx>
//#include <Mod/Part/App/FeaturePartBox.h>
//#include <Mod/Part/App/FeatureFace.h>

#include <Gui/Document.h>
#include <Gui/View3DInventor.h>
#include <Gui/Application.h>
#include <Gui/View3DInventorViewer.h>
#include <Gui/NavigationStyle.h>
#include <Gui/SoFCUnifiedSelection.h>
#include <Gui/ViewProviderDocumentObject.h>
#include <Gui/ViewProviderDocumentObjectGroup.h>
#include <Gui/ViewProviderGeometryObject.h>

#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/SoPickedPoint.h>

#include <Base/Console.h>

#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <GeomAdaptor_Surface.hxx>

using namespace std;

McCadSurfaceCard::McCadSurfaceCard()
{

}

//void McCadSurfaceCard::onSelectionChanged(const Gui::SelectionChanges& msg)
//{
//    cout<<"Test"<<endl;
//}


void McCadSurfaceCard::selSurface()
{
    Gui::Document* doc = Gui::Application::Instance->activeDocument();
    Gui::View3DInventor* view = static_cast<Gui::View3DInventor*>(doc->getActiveView());
    if (view)
    {
        Gui::View3DInventorViewer* viewer = view->getViewer();
        viewer->setEditing(true);
        viewer->addEventCallback(SoMouseButtonEvent::getClassTypeId(),
            McCadSurfaceCard::SelectSurfaceCallback);
        viewer->navigationStyle()->stopSelection();
    }
}


void McCadSurfaceCard::SelectSurfaceCallback(void * ud, SoEventCallback * n)
{
    const SoMouseButtonEvent * mbe = static_cast<const SoMouseButtonEvent*>(n->getEvent());
    Gui::View3DInventorViewer* view  = reinterpret_cast<Gui::View3DInventorViewer*>(n->getUserData());

    if (mbe->getButton() == SoMouseButtonEvent::BUTTON1 && mbe->getState() == SoButtonEvent::DOWN)
    {
        const SoPickedPoint * point = n->getPickedPoint();
        if (point == NULL)
        {
            Base::Console().Message("No point picked.\n");
            return;
        }
    }
    else if(mbe->getButton() == SoMouseButtonEvent::BUTTON2 && mbe->getState() == SoButtonEvent::UP)
    {
        view->setEditing(false);
        view->removeEventCallback(SoMouseButtonEvent::getClassTypeId(), SelectSurfaceCallback);
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

    const SoPickedPointList& pp = action.getPickedPointList();
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
                string strType;

                Base::Console().Message("Selected Face\n");

                TopoDS_Face theFace = TopoDS::Face(subShape);

                TopLoc_Location loc;
                Handle_Geom_Surface geom_surface = BRep_Tool::Surface(theFace,loc);
                GeomAdaptor_Surface surface_adoptor(geom_surface);

                switch(surface_adoptor.GetType())
                {
                case GeomAbs_Plane:{
                        strType = "Plane\n";
                        break;}
                case GeomAbs_Cylinder:{
                      strType = "Cylinder\n";
                      break;}
                default:break;
                }

                Base::Console().Message(strType.c_str());
            }
        }
    }
}
