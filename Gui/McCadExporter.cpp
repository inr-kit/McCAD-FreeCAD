#include "PreCompiled.h"
#include "McCadExporter.h"
#include "ui_Exporter.h"

#include <QDockWidget>
#include <QSplitter>

#include <Gui/Application.h>
#include <Gui/Command.h>
#include <Gui/Document.h>
#include <Gui/View3DInventor.h>
#include <Gui/View3DInventorViewer.h>
#include <Gui/DockWindowManager.h>
#include <Gui/ViewProviderDocumentObject.h>
#include <Gui/BitmapFactory.h>
#include <Gui/WorkbenchManager.h>
#include <Gui/Control.h>

#include <Mod/Part/App/PreCompiled.h>
#include <Mod/Part/App/PartFeature.h>

#include <App/Document.h>
#include <App/DocumentObject.h>
#include <App/Application.h>

#include <Base/Stream.h>

#include <TopTools_HSequenceOfShape.hxx>
#include <STEPControl_Writer.hxx>
#include <qmessagebox.h>
#include <qstring.h>

#include "McCadSettingDlg.h"
#include "Workbench.h"
#include <qeventloop.h>

using namespace McCadGui;

class ExportWidget::Private
{
public:
    Ui_ExportModel ui;
    QEventLoop loop;
    Private()
    {
    }
    ~Private()
    {
    }
};


ExportWidget::ExportWidget(QWidget* parent)
  : d(new Private())
{
    d->ui.setupUi(this);
    d->ui.progressBar->setMaximum(100);
    d->ui.progressBar->setMinimum(0);

    if(!m_strDir.isEmpty())
    {
        d->ui.textEditDir->setText(m_strDir);
    }
    else
    {
        Gui::Document *pDoc = Gui::Application::Instance->activeDocument();

        /** Get the material manager of current document */
        McCadGui::Workbench *wb = (McCadGui::Workbench *)Gui::WorkbenchManager::instance()->active();
        App::Document *doc = App::GetApplication().getActiveDocument();
        string doc_name = doc->getName();
        McCadData *pData = wb->GetDataManager()->getData(doc_name);
        QString strWorkingDir = pData->GetWorkingDir();

        if(!strWorkingDir.isEmpty())
        {
            m_strDir = strWorkingDir;
            d->ui.textEditDir->append(QString::fromLatin1("The model will be saved as:"));
            QString strConfFile = m_strDir + QString::fromLatin1("/ConvertedModel.stp");
            d->ui.textEditDir->append(strConfFile);
        }
    }
}

ExportWidget::~ExportWidget()
{
    delete d;
}

bool ExportWidget::accept()
{
    /** Get the material manager of current document */
    McCadGui::Workbench *wb = (McCadGui::Workbench *)Gui::WorkbenchManager::instance()->active();
    App::Document *doc = App::GetApplication().getActiveDocument();
    string doc_name = doc->getName();
    McCadData *pData = wb->GetDataManager()->getData(doc_name);
    QString strWorkingDir = pData->GetWorkingDir();

    if(strWorkingDir.isEmpty())
    {
        QMessageBox::warning(NULL,tr("Warning"),tr("The working directory is not set!"));
        return false;
    }

    //d->loop.exec();
    QString strDocName;

    Gui::Document *guiDoc = Gui::Application::Instance->activeDocument();
    Gui::View3DInventor* view = static_cast<Gui::View3DInventor*>(guiDoc->getActiveView());
    if (view)
    {
        Gui::View3DInventorViewer* viewer = view->getViewer();
        strDocName = QLatin1String(viewer->Name());
    }
    else
        return false;

    // Get the combo view which has the tree widget
    Gui::DockWindowManager *mgr = Gui::DockWindowManager::instance();
    QWidget *dw = mgr->getDockWindow("Combo View");
    if(!dw)
        return false;

    QTabWidget *tab = dw->findChild<QTabWidget*>();
    if(!tab)
        return false;

    QSplitter * split = tab->findChild<QSplitter*>();
    Gui::TreeWidget *tree = split->findChild<Gui::TreeWidget*>();

    if (tree)
    {
        QTreeWidgetItem * docItem = NULL;

        // find the item in the treeview of current document
        for(int i = 0 ;  i < tree->topLevelItemCount(); i++)
        {
            QTreeWidgetItem * item = tree->topLevelItem(i);
            for (int j = 0; j < item->childCount(); j++)
            {
                QTreeWidgetItem *itemChild = item->child(j);
                if (itemChild->text(0) == QString(QLatin1String(guiDoc->getDocument()->getName())))
                {
                    docItem = itemChild;
                }
            }
        }

        d->ui.labelSteps->setText(QLatin1String("Resort the nodes of Treeview."));
        string tempStr = "&Temp%$* ";
        string headStr = "Solid ";
        // Set temporary label name to avoid the repeated solid number
        renameTreenode(docItem,tempStr,0);
        // Resort the solids, and give the name "Solid No.X"
        renameTreenode(docItem,headStr,0);

        saveSolids(docItem);
        d->ui.labelSteps->setText(QLatin1String("Write finished."));       
    }

    return true;
    //d->loop.quit();
}

bool ExportWidget::reject()
{
    if (d->loop.isRunning())
        return false;
    return true;
}



TaskExport::TaskExport()
{
    widget = new ExportWidget();
    taskbox = new Gui::TaskView::TaskBox(
        Gui::BitmapFactory().pixmap("Download.svg"),
        widget->windowTitle(), true, 0);
    taskbox->groupLayout()->addWidget(widget);
    Content.push_back(taskbox);
}

TaskExport::~TaskExport()
{

}

void TaskExport::open()
{
}

void TaskExport::clicked(int id)
{

}

bool TaskExport::accept()
{    
    return widget->accept();
}

bool TaskExport::reject()
{
    return widget->reject();
}


void ExportWidget::renameTreenode( QTreeWidgetItem *& treeItem, string strHead,int iStartNum)
{
    for(int iGroup = 0 ; iGroup < treeItem->childCount(); iGroup ++)
    {
        QTreeWidgetItem * itemGroup = treeItem->child(iGroup);
        for(int iSolid = 0 ;  iSolid < itemGroup->childCount(); iSolid ++)
        {
            QTreeWidgetItem * itemSolid = itemGroup->child(iSolid);
            if (itemSolid->type() == Gui::TreeWidget::ObjectType)
            {
                Gui::DocumentObjectItem* objitem = static_cast<Gui::DocumentObjectItem*>(itemSolid);
                App::DocumentObject* obj = objitem->object()->getObject();

                if (obj && obj->getTypeId().isDerivedFrom(Part::Feature::getClassTypeId()))
                {
                    obj->Label.setValue(strHead + std::to_string(++iStartNum));
                }
            }
        }
    }
}


void ExportWidget::saveSolids(QTreeWidgetItem *& treeItem)
{
    Gui::Document *pDoc = Gui::Application::Instance->activeDocument();
    QString strFileName = m_strDir +QLatin1String("/ConvertedModel.stp");
    //Handle(TopTools_HSequenceOfShape) SolidList;
    //std::vector<App::DocumentObject*> sel; // selected
    QString info;

    STEPControl_Writer wrt;

    for(int iGroup = 0 ; iGroup < treeItem->childCount(); iGroup ++)
    {
        QTreeWidgetItem * itemGroup = treeItem->child(iGroup);
        for(int iSolid = 0 ;  iSolid < itemGroup->childCount(); iSolid ++)
        {
            info = QString::fromLatin1("Save the %1 solid %2").arg(itemGroup->text(0)).arg(iSolid);
            d->ui.labelSteps->setText(info);

            int iProgress = (iSolid+1)*100/itemGroup->childCount();
            d->ui.progressBar->setValue(iProgress);

            QTreeWidgetItem * itemSolid = itemGroup->child(iSolid);
            if (itemSolid->type() == Gui::TreeWidget::ObjectType)
            {
                Gui::DocumentObjectItem* objitem = static_cast<Gui::DocumentObjectItem*>(itemSolid);
                App::DocumentObject* obj = objitem->object()->getObject();

                if (obj && obj->getTypeId().isDerivedFrom(Part::Feature::getClassTypeId()))
                {
                    Part::Feature *feature = dynamic_cast<Part::Feature *>(obj);
                    Gui::ViewProvider *vp = pDoc->getViewProvider(obj);
                    if (vp && vp->isVisible())
                    {
                        const TopoDS_Shape & shape = feature->Shape.getValue();
                        if(!shape.IsNull())
                        {
                            TopAbs_ShapeEnum type = shape.ShapeType();
                            if(type == TopAbs_SOLID)
                            {
                                //SolidList->Append(shape);
                                wrt.Transfer(shape, STEPControl_AsIs);
                                //sel.push_back(obj);
                            }
                        }
                    }
                }
            }
        }
    }

    wrt.Write(strFileName.toStdString().c_str());

   // Base::ofstream out(strFileName.toStdString().c_str(), std::ios::out);
   // doc->exportObjects(sel,out);
}




#include "moc_McCadExporter.cpp"

