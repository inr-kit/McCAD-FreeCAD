#include "CreateBox.h"

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <STEPControl_Writer.hxx>

#include <Base/UnitsApi.h>
#include <App/Document.h>
#include <App/DocumentObject.h>
#include <App/Application.h>

#include <Handle_TDocStd_Document.hxx>
#include <Mod/Part/App/FeaturePartBox.h>
#include <Mod/Part/App/FeatureFace.h>

#include <Gui/Application.h>
#include <Gui/Command.h>
#include <Gui/Document.h>
#include <Gui/View3DInventor.h>
#include <Gui/View3DInventorViewer.h>
#include <Gui/BitmapFactory.h>
#include <Gui/DockWindowManager.h>
#include <QWidget>
#include <qstring.h>
#include <qstandarditemmodel.h>
#include <QDockWidget>

#include <iostream>
#include <qtextstream.h>
#include <QSplitter>
#include <Gui/Tree.h>

#include <Base/Console.h>
#include <App/Document.h>
#include <Gui/Application.h>
#include <Gui/Command.h>
#include <Gui/Document.h>
#include <Gui/MainWindow.h>
#include <Gui/FileDialog.h>

#include <Gui/ViewProviderDocumentObject.h>
#include <Gui/ViewProviderDocumentObjectGroup.h>
#include <Gui/ViewProviderGeometryObject.h>

#include <App/DocumentObjectGroup.h>
#include <QString>
#include <cstdlib>
#include <ctime>

#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <string.h>
#include <GeomAdaptor_Surface.hxx>

#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/SoPickedPoint.h>

#include <Inventor/details/SoFaceDetail.h>
#include <Gui/ViewProviderMeasureDistance.h>
#include <QCursor>
#include <QLabel>

#include <Gui/NavigationStyle.h>
#include <Gui/SoFCUnifiedSelection.h>


using namespace std;

CreateBox::CreateBox()
{
}

//=======================================================================
/*!
 * \brief Test code, creat a box and cylinder and add to current view
 */
//=======================================================================

void CreateBox::MakeBox()
{
  static TopoDS_Solid aSolid;
  if ( aSolid.IsNull() )
  {
    aSolid = BRepPrimAPI_MakeBox(1,10,2);
  }

  //STEPControl_Writer wrt;
  //wrt.Transfer(aSolid,STEPControl_AsIs);
  //wrt.Write("test1710.stp");

  App::Document* pcDoc = App::GetApplication().getActiveDocument();

  Part::Box * pcBox;
  pcBox = static_cast<Part::Box*>(pcDoc->addObject("Part::Box","Cube"));
  pcBox->Length.setValue(50);
  pcBox->Width.setValue(50);
  pcBox->Height.setValue(50);

  BRepPrimAPI_MakeBox box(1,10,5);

  TopoDS_Shape boxShape = box.Shape();
  pcBox->Shape.setValue(boxShape);

  gp_Pnt p(0,0,0);
  gp_Dir d(0,0,1);
  double radius, height, angle=360;
  radius=50;
  height=200;
  BRepPrimAPI_MakeCylinder mkCyl(gp_Ax2(p,d),radius,height,angle*(M_PI/180.0f));

  TopoDS_Shape cylShape = mkCyl.Shape();
  //pcBox->Shape.setValue(resultshape);

  Part::Feature* part = static_cast<Part::Feature*>(pcDoc->addObject("Part::Feature"));
  part->Shape.setValue(cylShape);
  std::string name("Example");
  part->Label.setValue(name);

  pcDoc->recompute();  
  Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"Gui.SendMsgToActiveView(\"ViewFit\")");

  /* C++ Code
  Gui::Document* doc = Gui::Application::Instance->activeDocument();
  Gui::MDIView* view = doc->getActiveView();

  if (view && view->isDerivedFrom(Gui::View3DInventor::getClassTypeId()))
  {
      Gui::View3DInventorViewer* viewer = static_cast<Gui::View3DInventor*>(view)->getViewer();
      viewer->viewAll();
  } */

}

//=======================================================================
/*!
 * \brief Test code, Get the selection and save them as step file
 */
//=======================================================================

bool CreateBox::GetSelection()
{
    std::vector<Gui::SelectionSingleton::SelObj> selections = Gui::Selection().getSelection();
    std::vector<TopoDS_Shape> shapes;

    if (selections.size() < 1)
    {
       Gui::Document *pDoc = Gui::Application::Instance->activeDocument();
       std::vector<Part::Feature *> objs = pDoc->getDocument()->getObjectsOfType<Part::Feature>();
       for (std::vector<Part::Feature*>::iterator it = objs.begin(); it != objs.end(); ++it)
       {
           Gui::ViewProvider *vp = pDoc->getViewProvider(*it);
           if (vp && vp->isVisible())
           {
               const TopoDS_Shape & shape = (*it)->Shape.getValue();
               if(!shape.IsNull())
               {
                   TopAbs_ShapeEnum type = shape.ShapeType();
                   if(type == TopAbs_SOLID)
                   {
                       shapes.push_back(shape);
                   }
               }
           }
       }
    }

    std::vector<Gui::SelectionSingleton::SelObj>::iterator it;
    for (it = selections.begin(); it != selections.end(); ++it)
    {
        Part::Feature *feature = dynamic_cast<Part::Feature *>((*it).pObject);
        if (!feature)
            break;
        TopoDS_Shape shape = feature->Shape.getValue();
        if (strlen((*it).SubName) > 0)
        {
            shape = feature->Shape.getShape().getSubShape((*it).SubName);
        }
        if (shape.IsNull())
            break;
        else if (shape.ShapeType() != TopAbs_SOLID)
            continue;
        shapes.push_back(shape);
    }

    STEPControl_Writer wrt;
    //wrt.Transfer(aSolid,STEPControl_AsIs);
    //wrt.Write("test1710.stp");

    for(unsigned int i = 0; i < shapes.size(); i++)
    {
        TopoDS_Shape itShape = shapes.at(i);
        wrt.Transfer(itShape,STEPControl_AsIs);
    }

    wrt.Write("test1910.stp");
    return true;
}

void CreateBox::PrintBox()
{
    std::vector<Gui::SelectionSingleton::SelObj> selections = Gui::Selection().getSelection();
    std::vector<TopoDS_Shape> shapes;

    if (selections.size() < 1)
        return;

    std::vector<Gui::SelectionSingleton::SelObj>::iterator it;
    for (it = selections.begin(); it != selections.end(); ++it)
    {
        Part::Feature *feature = dynamic_cast<Part::Feature *>((*it).pObject);
        if (!feature)
            break;
        TopoDS_Shape shape = feature->Shape.getValue();
        if (strlen((*it).SubName) > 0)
        {
            shape = feature->Shape.getShape().getSubShape((*it).SubName);
        }
        if (shape.IsNull())
            break;
        //else if (shape.ShapeType() != TopAbs_SOLID)
        //    continue;
        shapes.push_back(shape);
    }

    //int iEdge = 0;
    //int iVertex = 0;
    int iFace = 0;

    string strType;

    for(unsigned int i = 0; i < shapes.size(); i++)
    {
        TopoDS_Shape itShape = shapes.at(i);

        if(itShape.ShapeType() == TopAbs_SOLID)
        {
              TopExp_Explorer exp;
              TopoDS_Solid theSolid = TopoDS::Solid(itShape);

              for (exp.Init(theSolid,TopAbs_FACE);exp.More();exp.Next())
              {
                  TopoDS_Face theFace = TopoDS::Face(exp.Current());
                  iFace ++;
              }

        }
        else if (itShape.ShapeType() == TopAbs_FACE)
                {
                      TopExp_Explorer exp;
                      TopoDS_Face theFace = TopoDS::Face(itShape);

                      TopLoc_Location loc;
                      Handle_Geom_Surface geom_surface = BRep_Tool::Surface(theFace,loc);
                      GeomAdaptor_Surface surface_adoptor(geom_surface);

                      switch(surface_adoptor.GetType())
                      {
                        case GeomAbs_Plane:
                      {     strType = "Plane";
                            break;
                            }
                      case GeomAbs_Cylinder:{
                            strType = "Cylinder";
                            break;}
                      default:break;
                      }

                      Base::Console().Message(strType.c_str());

                }
    }

    QString str;

    stringstream strs;
    strs << "The solid has " <<iFace<<" faces"<<endl;
    str.sprintf("The solid has %d faces\n", iFace);
    Base::Console().Message(strs.str().c_str());

}

void CreateBox::TestCombiView()
{
    Gui::Document* doc = Gui::Application::Instance->activeDocument();
    Gui::View3DInventor* view = static_cast<Gui::View3DInventor*>(doc->getActiveView());
    if (view) {
        Gui::View3DInventorViewer* viewer = view->getViewer();
        viewer->setEditing(true);
        viewer->addEventCallback(SoMouseButtonEvent::getClassTypeId(),
            CreateBox::SelectSurfaceCallback);

        viewer->navigationStyle()->stopSelection();
    }

    Gui::DockWindowManager *mgr = Gui::DockWindowManager::instance();
    QWidget *dw = mgr->getDockWindow("Combo View");
    QWidget *tr = mgr->getDockWindow("Tree View");

   /* QWidget* tv = Gui::DockWindowManager::instance()->getDockWindow("Tree view");

    if (tv && tv->isVisible()) {
        QTreeWidget *tree = tv->findChild<QTreeWidget*>();

        if (tv)
        {
            QStandardItemModel *model = qobject_cast<QStandardItemModel*>(tree->model());

            QString t = QString(QLatin1String("My tab"));

            int st = model->horizontalHeaderItem(0)->columnCount();
            //m_ss = QObject::tr("%1").arg(st);
        }
    }*/


    if(dw)
    {
       // QTreeWidget *tree = dw->findChild<QTreeWidget*>();
        QTabWidget *tab = dw->findChild<QTabWidget*>();
        if(tab)
        {
            //tab->setCurrentIndex(1);
            QSplitter *qs = tab->findChild<QSplitter*>();
            Gui::TreeWidget *tree = qs->findChild< Gui::TreeWidget*>();
            QString sss;
            if (tree)
            {
                QTreeWidgetItemIterator it(tree);
                while(*it)
                {
                    sss += (*it)->text(0);
                    sss += QLatin1String("\n") ;
                    it++;
                }

                m_ss = sss;

                for(int i =0;  i< tree->topLevelItemCount(); i++)

                {
                    QTreeWidgetItem * item = tree->topLevelItem(i);
                    sss += item->text(0);sss += QLatin1String("     ");

                    for (int j=0; j < item->childCount(); j++)
                    {
                        QTreeWidgetItem *itemChild = item->child(j);
                     //   sss += itemChild->text(0); sss += QLatin1String("     ");
                    }


                }


                //model->clear();
                //m_ss = sss;
            }



          //QString sss = tab->objectName();


            //QString t = QString(QLatin1String("My tab"));
            //tab->addTab(dw,t);
            //tab->setCurrentIndex(1);
            /* QTreeView *tre =  tab->findChild<QTreeView*>();

            if (tre)
            {
                QStandardItemModel *model = qobject_cast<QStandardItemModel*>(tre->model());

                QString t = QString(QLatin1String("My tab"));
                //model->setHorizontalHeaderLabels(QStringList()<<t);

                // model->appendRow(itemMat);

            }*/


        }
    }

}



void CreateBox::SelectSurface()
{
    Gui::Document* doc = Gui::Application::Instance->activeDocument();
    Gui::View3DInventor* view = static_cast<Gui::View3DInventor*>(doc->getActiveView());
    if (view) {
        Gui::View3DInventorViewer* viewer = view->getViewer();
        viewer->setEditing(true);
        viewer->addEventCallback(SoMouseButtonEvent::getClassTypeId(),
            CreateBox::SelectSurfaceCallback);

        viewer->navigationStyle()->stopSelection();

        viewer->setEditing(false);
        viewer->removeEventCallback(SoMouseButtonEvent::getClassTypeId(),CreateBox::SelectSurfaceCallback, this);


    }

    /*std::vector<Part::Feature *> objs = doc->getDocument()->getObjectsOfType<Part::Feature>();
    for (std::vector<Part::Feature*>::iterator it = objs.begin(); it != objs.end(); ++it)
    {
        Gui::ViewProvider *vp = doc->getViewProvider(*it);
        Gui::ViewProviderGeometryObject* vpd = static_cast<Gui::ViewProviderGeometryObject*>(vp);
        vpd->Selectable.setValue(false);
    }*/

    /* Gui::Document* doc = Gui::Application::Instance->activeDocument();
    Gui::MDIView* view = doc->getActiveView();

    if (view->getTypeId().isDerivedFrom(Gui::View3DInventor::getClassTypeId()))
    {
         Gui::View3DInventorViewer* viewer = ((Gui::View3DInventor*)view)->getViewer();

         //viewer->setRedirectToSceneGraph(true);
         //viewer->startSelection(Gui::View3DInventorViewer::Lasso);
         viewer->stopSelection();

         viewer->setEditing(false);
         viewer->setRedirectToSceneGraphEnabled(true);
         viewer->setRedirectToSceneGraph(true);

         viewer->addEventCallback(SoMouseButtonEvent::getClassTypeId(), SelectSurfaceCallback);
    } */
}

void CreateBox::SelectSurfaceCallback(void * ud, SoEventCallback * n)
{
    const SoMouseButtonEvent * mbe = static_cast<const SoMouseButtonEvent*>(n->getEvent());
    Gui::View3DInventorViewer* view  = reinterpret_cast<Gui::View3DInventorViewer*>(n->getUserData());

    // view->stopSelection();

    // Mark all incoming mouse button events as handled, especially, to deactivate the selection node    
    //n->getAction()->setHandled();
    //n->setHandled();

    /*int i = mbe->getButton();
    int j = mbe->getState();
    stringstream strs;
    strs << "Button " <<i<<"  Up&Down  "<<j<<endl;
    Base::Console().Message(strs.str().c_str());*/

    if (mbe->getButton() == SoMouseButtonEvent::BUTTON1 && mbe->getState() == SoButtonEvent::DOWN)
    {
        const SoPickedPoint * point = n->getPickedPoint();
        if (point == NULL)
        {
            Base::Console().Message("No point picked.\n");
            return;
        }
        //n->setHandled();
    }    
    else if(mbe->getButton() == SoMouseButtonEvent::BUTTON2 && mbe->getState() == SoButtonEvent::UP)
    {
        //n->setHandled();
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

    //SoPickedPoint * p2 = pp[0];

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
            //Part::Feature* part = static_cast<Part::Feature*>(obj);
            //const Part::TopoShape TShape = part->Shape.getShape();

            std::string element = vp->getElement(p1->getDetail());
            const Part::TopoShape& shape = static_cast<Part::Feature*>(obj)->Shape.getShape();
            TopoDS_Shape subShape = shape.getSubShape(element.c_str());


            //App::Document* doc = App::GetApplication().getActiveDocument();
            //Gui::Selection().addSelection(doc->getName(),obj->getNameInDocument(),0);

            /*Gui::SoHighlightElementAction action;

            SbColor selectionColor(0.1f, 0.8f, 0.1f);
            action.setColor(selectionColor);

            action.setHighlighted(1);
            action.setElement(p1->getDetail());
            action.apply(p1->getPath());*/

            //const TopoDS_Shape & shape = part->Shape.getShape()._Shape;

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


void CreateBox::run() {
//        static QWeakPointer<ConvertorDlg>pp;
//        if(!pp)
//        {
//            pp = new ConvertorDlg();
//        }

//        ConvertorDlg *pDlg = pp.data();
//        pDlg->setFixedSize(pDlg->width(),pDlg->height());
//        pDlg->setAttribute(Qt::WA_DeleteOnClose);
//        pDlg->show();
//        pDlg->raise();
//        pDlg->activateWindow();

}



