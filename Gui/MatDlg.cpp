#include "PreCompiled.h"

#include "MatDlg.h"
#include "ui_MatEditorDlg.h"

#include <App/DocumentObject.h>
#include <App/DocumentObjectGroup.h>

#include <Gui/Application.h>
#include <Gui/Document.h>

#include <Gui/View3DInventor.h>
#include <Gui/View3DInventorViewer.h>
#include <Gui/ViewProviderDocumentObject.h>
#include <Gui/DockWindowManager.h>
#include <Gui/WorkbenchManager.h>

#include <QDockWidget>
#include <QSplitter>
#include <Gui/Tree.h>
#include <iostream>

#include <Mod/Part/App/PreCompiled.h>
#include <Mod/Part/App/PartFeature.h>

#include <qstandarditemmodel.h>
#include <qstring.h>
#include <qfile.h>

#include <iostream>
#include <qtextstream.h>

#include <QFileDialog>
#include <QDomDocument>
#include <QDomNode>
#include <QMessageBox>

#include "../App/McCadDocObserver.h"
#include "Workbench.h"
#include "McCadUtils.h"


using namespace std;
using namespace Gui;

MatDlg::MatDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MatDlg)
{
    ui->setupUi(this);

    connect(ui->BnLoad, SIGNAL(clicked()), this, SLOT(LoadXMLFile()));
    connect(ui->BnAddMatList, SIGNAL(clicked()), this, SLOT(AddMaterial()));
    connect(ui->BnRvMatList, SIGNAL(clicked()), this, SLOT(DelMaterial()));
    connect(ui->BnConnect, SIGNAL(clicked()), this, SLOT(ConnectGroupMat()));
    connect(ui->BnAuto,SIGNAL(clicked()),this,SLOT(AutoSetMaterial()));
    connect(ui->MatTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(ShowSelectedMaterial(QTreeWidgetItem*,int)));
    connect(ui->GroupTree, SIGNAL(clicked(QModelIndex)), this, SLOT(ShowSelectedGroup()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(SaveXMLFile()));

    /** Get the material manager of current document */
    McCadGui::Workbench *wb = (McCadGui::Workbench *)Gui::WorkbenchManager::instance()->active();
    App::Document *doc = App::GetApplication().getActiveDocument();
    string doc_name = doc->getName();
    m_MatManager = wb->GetDataManager()->getMatManager(doc_name);
    m_Data = wb->GetDataManager()->getData(doc_name);

    /** Load the groups and materials of the current document */
    LoadGroups();
    LoadMaterials();   
}


MatDlg::~MatDlg()
{
    delete ui;
}

void MatDlg::SaveXMLFile()
{
    QString theFileName = m_Data->GetWorkingDir();

    if(!theFileName.isEmpty())
    {
        theFileName += QString::fromLatin1("/Materials.xml");        
        m_MatManager->SaveXMLfile(theFileName);
        QMessageBox::information(this, tr("Information"),
            tr("Mateirals.xml file is saved as %1").arg(theFileName));
    }
    else
    {
        QMessageBox::information(this, tr("Information"),
            tr("Woring directory is not set.\nPlease choose working directory and save material file."));

        QString fileFilters = QString::fromLatin1("*.xml");
        theFileName = QFileDialog::getSaveFileName(this, tr("Save As"),	tr("Materials.xml"), fileFilters );

        if (!theFileName.isEmpty())
        {
            if(!theFileName.endsWith(tr(".xml")))
            {
                theFileName.append(tr(".xml"));
            }
            m_MatManager->SaveXMLfile(theFileName);

            int index = theFileName.lastIndexOf(tr("/"));
            QString strWorkingDir = theFileName.mid(0,index+1);
            m_Data->SetWorkingDir(strWorkingDir);

            QMessageBox::information(this, tr("Information"),
                tr("Working directory is set: \n%1").arg(strWorkingDir));
        }
    }    
}


void MatDlg::LoadGroups()
{
    QString strDocName;

    Gui::Document *guiDoc = Gui::Application::Instance->activeDocument();
    Gui::View3DInventor* view = static_cast<Gui::View3DInventor*>(guiDoc->getActiveView());
    if (view)
    {
        Gui::View3DInventorViewer* viewer = view->getViewer();
        strDocName = QLatin1String(viewer->Name());
    }

    // Get the combo view which has the tree widget
    Gui::DockWindowManager *mgr = Gui::DockWindowManager::instance();
    QWidget *dw = mgr->getDockWindow("Combo View");

    // Fill the data in the tree view on the material dialog box
    ui->GroupTree->setRootIsDecorated(true);
    QStandardItemModel *model = new QStandardItemModel(ui->GroupTree);
    model->setHorizontalHeaderLabels(QStringList()<<QString(QLatin1String("Group")));

    // Fill the data of material list view
    QStandardItem * itemMat = new QStandardItem(QString(QLatin1String("Material Groups")));
    model->appendRow(itemMat);

    // found docking combo view
    if(!dw) return;

    QTabWidget *tab = dw->findChild<QTabWidget*>();
    if(!tab) return;

    QSplitter * split = tab->findChild<QSplitter*>();
    Gui::TreeWidget *tree = split->findChild<Gui::TreeWidget*>();
    if (tree)
    {
        QTreeWidgetItem * docItem = NULL;
        m_MatManager->ClearSolidGroupMap();

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

        unsigned int iSolid = 0;
        for(int iGroup = 0 ; iGroup < docItem->childCount(); iGroup ++)
        {
            QTreeWidgetItem * itemGroup = docItem->child(iGroup);
            QString strItem;

            if(CheckItem(itemGroup) == GROUP)
            {
                strItem = itemGroup->text(0);
                QStandardItem * item = new QStandardItem(QIcon(QString(QLatin1String(":/icons/folder.svg"))),strItem);
                itemMat->appendRow(item);

                CrtSolidGroupMap(itemGroup,iSolid);
            }
        }
    }

    ui->GroupTree->setModel(model);
    ui->GroupTree->expandAll();
}


void MatDlg::CrtSolidGroupMap(QTreeWidgetItem *& pGroupItem, unsigned int &iNumSolid )
{
    // the sort number of solids
    cout<<"iSolid = "<<iNumSolid<<endl;

    QString strItem = pGroupItem->text(0);          // Group name

    unsigned int iBegin, iEnd;                      // The begin and end numbers of solids in the group
    iBegin = iNumSolid +1;

    QTreeWidgetItemIterator iterSolid(pGroupItem,QTreeWidgetItemIterator::All);

    while(*iterSolid)
    {
        QTreeWidgetItem * pItemSolid = (*iterSolid);
        if(CheckItem(pItemSolid) == SOLID)
            iNumSolid++;
        iterSolid++;
        QTreeWidgetItem * pNextItem = (*iterSolid);
        if(CheckItem(pNextItem ) == GROUP)
        {
           break;
        }
    }

    iEnd = iNumSolid;
    m_MatManager->GenSolidGroupMap(iBegin,iEnd,strItem);

    cout<<" iBegin  "<<iBegin<<"  iEnd   "<<iEnd<<strItem.toStdString()<<endl;

}


ItemType MatDlg::CheckItem(QTreeWidgetItem *& item)
{
    if (!item) return OTHER;

    if (item->type() == TreeWidget::DocumentType)
    {
        return DOC;
    }
    else if(item->type() == TreeWidget::ObjectType)
    {
        DocumentObjectItem* objitem = static_cast<DocumentObjectItem*>(item);
        App::DocumentObject* obj = objitem->object()->getObject();

        if (obj->getTypeId().isDerivedFrom(Part::Feature::getClassTypeId()))
        {
            // Only solid can be choosed
            return SOLID;
        }
        else if(obj->getTypeId().isDerivedFrom(App::DocumentObjectGroup::getClassTypeId())
                && item->parent()->type() == TreeWidget::DocumentType )
        {
            return GROUP;
        }
        else
        {
            return OTHER;
        }
    }
    else
    {
        return OTHER;
    }
}



void MatDlg::LoadMaterials()
{
    ui->MatTree->setHeaderLabel(QString::fromLatin1("Material"));
    vector<int> MatIDList = m_MatManager->GetMatIDList();
    for(unsigned int i = 0; i< MatIDList.size(); i++)
    {
        QTreeWidgetItem* itemAdd = new QTreeWidgetItem();
        QString itemText = QString::fromLatin1("M ") + QString::number(MatIDList.at(i));
        itemAdd->setText(0,itemText);
        itemAdd->setText(1,QString::number(MatIDList.at(i)));
        ui->MatTree->addTopLevelItem(itemAdd);
    }
}


void MatDlg::ShowMaterial(QString strSelectedGrp)
{
    int iMatNum = m_MatManager->GetMatNum(strSelectedGrp); // The material number of selected group
    if(iMatNum == 0)
        return;

    QTreeWidgetItemIterator it(ui->MatTree,QTreeWidgetItemIterator::All);
    while(*it)
    {
        QTreeWidgetItem * pItem = (*it);
        if(pItem->text(1).toInt() == iMatNum)
        {
            ui->MatTree->setItemSelected(pItem,true); // Select the material number of being edited group
            DisplayMaterial(iMatNum);
        }
        it++;
    }
}


void MatDlg::AutoSetMaterial()
{
    // Clean the existed materials
    if(ui->MatTree->topLevelItemCount() > 0)
    {
        int reply = QMessageBox::question(this,
                                          tr("Warning"),
                                          tr("Are you sure to remove the existed materials?"),
                                          QMessageBox::Yes,QMessageBox::No);
        if (reply == QMessageBox::No){
            return;
        }
        else{
            m_MatManager->RemoveMaterials();
            ui->MatTree->clear();
        }
    }

    vector<QString> group_list;

    QStandardItemModel * model = qobject_cast<QStandardItemModel*>(ui->GroupTree->model());
    QModelIndex rootIndex = ui->GroupTree->rootIndex();
    QModelIndex matIndex = model->index(0,0,rootIndex);

    if(model->hasChildren(matIndex))
    {
        for (unsigned int i = 0; i < model->rowCount(matIndex); ++i)
        {
            QModelIndex index = model->index(i,0,matIndex);
            QString group_name = model->data(index).toString();
            if(!group_name.isEmpty())
                group_list.push_back(group_name);
        }

        for(unsigned int i = 0; i< group_list.size(); i++)
        {
            int iMatNum = i+1;

            QTreeWidgetItem* itemAdd = new QTreeWidgetItem();
            QString itemText = QString::fromLatin1("M ") + QString::number(iMatNum);
            itemAdd->setText(0,itemText);
            itemAdd->setText(1,QString::number(iMatNum));
            ui->MatTree->addTopLevelItem(itemAdd);

            double fDensity = iMatNum * 0.001f;
            QString strGroupName = group_list.at(i);
            QString strMatName = QString::fromLatin1("Mat.%1 (%2)").arg(iMatNum).arg(strGroupName);
            QString strCard = QString::fromLatin1("# Edit material composition for Group: %1 #").arg(strGroupName);

            m_MatManager->InsertNewMaterial(iMatNum,fDensity,strMatName,strCard);
            m_MatManager->InsertMatGroup(strGroupName,iMatNum);
        }
    }
}


void MatDlg::ConnectGroupMat()
{
    QModelIndex index = ui->GroupTree->currentIndex();
    if( index.parent() == ui->GroupTree->rootIndex() || index == ui->GroupTree->rootIndex())
        return;

    QModelIndexList list = ui->GroupTree->selectionModel()->selectedIndexes();

    for(int i = 0; i < list.size(); i++)
    {
        QModelIndex index = list.at(i);
        if( index.parent() == ui->GroupTree->rootIndex())
            return;

        QVariant data = ui->GroupTree->model()->data(index);
        QString strGroup = data.toString();

        if(strGroup.isEmpty())
            return;

        QTreeWidgetItem* itemMat = ui->MatTree->currentItem();
        if (NULL == itemMat)
            return;

        int iItem = itemMat->text(1).toInt();            // Get the material ID
        m_MatManager->InsertMatGroup(strGroup,iItem);       
    }
}


void MatDlg::LoadXMLFile()
{
    QString fileFilters(QString::fromLatin1("*.xml"));
    QString theFileName = QFileDialog::getOpenFileName(this, QString::fromLatin1("Load"),QString::fromLatin1("."), fileFilters );

    if (!theFileName.isEmpty())
    {
        if(!theFileName.endsWith(QString::fromLatin1(".xml")))
        {
            theFileName.append(QString::fromLatin1(".xml"));
        }
        assert(m_MatManager);
        m_MatManager->LoadXMLfile(theFileName,false);  // Only material cards are loaded
        Refresh(); // Display the loaded materials
    }
}


void MatDlg::Refresh()
{
    vector<int> MatIDList = m_MatManager->GetMatIDList();
    for(unsigned int i = 0; i< MatIDList.size(); i++)
    {
        QTreeWidgetItem* itemAdd = new QTreeWidgetItem();
        QString itemText = tr("M ") + QString::number(MatIDList.at(i));
        itemAdd->setText(0,itemText);
        itemAdd->setText(1,QString::number(MatIDList.at(i)));
        ui->MatTree->addTopLevelItem(itemAdd);
    }

//    if (MatIDList.size() != 0 )
//    {
//        DisplayMaterial(MatIDList.at(0));   // Load the detail information of the first material
//    }
}


void MatDlg::DisplayMaterial(int &iItem)
{
    Material *pMaterial = NULL;
    pMaterial = m_MatManager->GetMaterial(iItem);

    if (NULL != pMaterial)
    {
        QString strID = QString::number(pMaterial->GetMatID());
        QString strDensity = QString::number(pMaterial->GetMatDensity());
        QString strName = pMaterial->GetMatName();
        QString strMatCard = pMaterial->GetMatCard();

        ui->lineEditID->setText(strID);
        ui->lineEditDensity->setText(strDensity);
        ui->lineEditMatName->setText(strName);
        ui->TextEditCmp->setPlainText(strMatCard);
    }
    return;
}


void MatDlg::AddMaterial()
{
    QString strMessage;

    if(!CheckInput(strMessage))             // Check the input data and their formats
    {
        QMessageBox msg;
        msg.setText(strMessage);
        msg.exec();
        return;
    }

    unsigned int iSelID;  // Record the selected material id and compared with the new one
    QTreeWidgetItem* itemCur = ui->MatTree->currentItem();
    if (NULL != itemCur)
    {
        iSelID = itemCur->text(1).toInt();  // Get the material ID
    }
    else
    {
        iSelID = 0;
    }

    if ( ui->lineEditMatName->text().isEmpty() ) // The empty material name can be assigend automatically
    {
        m_Name = QString::fromLatin1("Mat.%1").arg(m_ID);
        ui->lineEditMatName->setText(m_Name);
    }

    if(m_ID == iSelID)  // if the new material id is same as selected material, then modify it.
    {
        ModifyMaterial();
    }                   // else add a new material
    else if(m_MatManager->InsertNewMaterial(m_ID,m_Density,m_Name,m_Card))
    {
        QTreeWidgetItem* itemAdd = new QTreeWidgetItem();
        QString itemText = QString::fromLatin1("M ") + ui->lineEditID->text();
        itemAdd->setText(0,itemText);
        itemAdd->setText(1,ui->lineEditID->text());
        ui->MatTree->addTopLevelItem(itemAdd);
        ClearEditor();
    }
    else                // if the material has already existed, popup the massage box.
    {
        QMessageBox msg;

        QString strMsg = QString::fromLatin1("Material %1 has been already existed").arg(m_ID);
        msg.setText(strMsg);
        msg.exec();
    }
}



bool MatDlg::CheckInput(QString &strErrorMsg)
{
    if ( ui->lineEditID->text().isEmpty() )
    {
        strErrorMsg = QString::fromLatin1("Material ID is missing");
        return false;
    }

    if ( ui->lineEditDensity->text().isEmpty() )
    {
        strErrorMsg = QString::fromLatin1("Density is missing");
        return false;
    }

    if ( ui->lineEditMatName->text().length() > 50 )
    {
        strErrorMsg = QString::fromLatin1("Material name is too long");
        return false;
    }

    m_Name = ui->lineEditMatName->text();
    m_ID = ui->lineEditID->text().toInt();
    m_Density = ui->lineEditDensity->text().toDouble();
    m_Card = ui->TextEditCmp->toPlainText();

    if( m_ID == 0 )
    {
        strErrorMsg = QString::fromLatin1("Material ID should be between 1 and 1000");
        return false;
    }

    if( m_Density == 0 )
    {
        strErrorMsg = QString::fromLatin1("Value of density can not be 0");
        return false;
    }

    return true;
}



void MatDlg::ModifyMaterial()
{
    QTreeWidgetItem* itemCur = ui->MatTree->currentItem();
    if (NULL == itemCur)
    {
        return;
    }

    m_Card = ui->TextEditCmp->toPlainText();
    m_Name = ui->lineEditMatName->text();
    m_ID = ui->lineEditID->text().toInt();
    m_Density = ui->lineEditDensity->text().toDouble();

    int index = itemCur->text(1).toInt();            // Get the material ID
    if (m_MatManager->UpdateMaterial(index,m_ID,m_Density,m_Name,m_Card))
    {
        QString itemText = tr("M ") + ui->lineEditID->text();
        itemCur->setText(0,itemText);
        itemCur->setText(1,ui->lineEditID->text());
    }
    else
    {
        return;
    }
}


void MatDlg::DelMaterial()
{
    QTreeWidgetItem* itemCur = ui->MatTree->currentItem();
    if (NULL == itemCur)
    {
        return;
    }

    int index = ui->MatTree->indexOfTopLevelItem(itemCur);
    int iDelItem = itemCur->text(1).toInt();            // Get the material ID

    if(m_MatManager->DeleteMaterial(iDelItem))
    {
        ui->MatTree->takeTopLevelItem(index);  // Delete the item on the tree

        delete itemCur;         // Delete the iterCur pointer
        itemCur = NULL;

        ClearEditor();          // Clear the edit box
    }

    return;
}


void MatDlg::ClearEditor()
{
    ui->lineEditID->clear();
    ui->lineEditMatName->clear();
    ui->lineEditDensity->clear();
    ui->TextEditCmp->clear();
}


void MatDlg::ShowSelectedMaterial(QTreeWidgetItem* item,int colume)
{
    QTreeWidgetItem* itemCur = ui->MatTree->currentItem();
    if (NULL == itemCur)
    {
        return;
    }

    int iItem = item->text(1).toInt();          // Get the material ID
    DisplayMaterial(iItem);                     // Display the material whose ID is iItem

    return;
}


void MatDlg::ShowSelectedGroup()
{
    ui->MatTree->clearSelection();

    QModelIndexList list = ui->GroupTree->selectionModel()->selectedIndexes();

    if (!list.isEmpty())
    {
        QModelIndex index = list.at(0);
        if( index.parent() == ui->GroupTree->rootIndex())
            return;
        QVariant data = ui->GroupTree->model()->data(index);
        QString strGroup = data.toString();

        if (strGroup.isEmpty())
        {
            return;
        }
        else
        {
            ShowMaterial(strGroup);
        }
    }
}

/*void MatDlg::FillData2()
{

    //QFile file("/home/mccad/Downloads/FreeCAD-Debug/bin/DlgQSS.qss");

    //file.open(QFile::ReadOnly);
    //QTextStream filetext(&file);
    //QString stylesheet = filetext.readAll();
    //ui->treeView->setStyleSheet(stylesheet);

    //file.close();
    ui->treeView->setRootIsDecorated(true);

    QStandardItemModel *model = new QStandardItemModel(ui->treeView);
    model->setHorizontalHeaderLabels(QStringList()<<QString("Group"));

    QStandardItem * itemMat = new QStandardItem(QString("Material Groups"));
    model->appendRow(itemMat);

    QStandardItem * item1 = new QStandardItem(QIcon(QString(":/icons/folder.svg")),"Blanket");
    QStandardItem * item2 = new QStandardItem(QIcon(QString(":/icons/folder.svg")),"Central_solenoid");
    QStandardItem * item3 = new QStandardItem(QIcon(QString(":/icons/folder.svg")),"Cryostat");
    QStandardItem * item4 = new QStandardItem(QIcon(QString(":/icons/folder.svg")),"Divertor1Layer");
    QStandardItem * item5 = new QStandardItem(QIcon(QString(":/icons/folder.svg")),"Divertor2Layer");
    QStandardItem * item6 = new QStandardItem(QIcon(QString(":/icons/folder.svg")),"Divertor3Layer");
    QStandardItem * item7 = new QStandardItem(QIcon(QString(":/icons/folder.svg")),"Divertor4Layer");
    QStandardItem * item8 = new QStandardItem(QIcon(QString(":/icons/folder.svg")),"PFC");
    QStandardItem * item9 = new QStandardItem(QIcon(QString(":/icons/folder.svg")),"Shield Port");
    QStandardItem * item10 = new QStandardItem(QIcon(QString(":/icons/folder.svg")),"TF Casing");
    QStandardItem * item11 = new QStandardItem(QIcon(QString(":/icons/folder.svg")),"TF Magnet");
    QStandardItem * item12 = new QStandardItem(QIcon(QString(":/icons/folder.svg")),"VV Body");
    QStandardItem * item13 = new QStandardItem(QIcon(QString(":/icons/folder.svg")),"VV Shell");

    itemMat->appendRow(item1);
    itemMat->appendRow(item2);
    itemMat->appendRow(item3);
    itemMat->appendRow(item4);
    itemMat->appendRow(item5);
    itemMat->appendRow(item6);
    itemMat->appendRow(item7);
    itemMat->appendRow(item8);
    itemMat->appendRow(item9);
    itemMat->appendRow(item10);
    itemMat->appendRow(item11);
    itemMat->appendRow(item12);
    itemMat->appendRow(item13);

    ui->treeView->setModel(model);
    ui->treeView->expandAll();


    QStandardItemModel *matmodel = new QStandardItemModel(ui->listView);
    matmodel->setHorizontalHeaderLabels(QStringList()<<QString("Materials"));

    QStandardItem * itemM1 = new QStandardItem(QString("Mat.1"));
    QStandardItem * itemM2 = new QStandardItem(QString("Mat.2"));
    QStandardItem * itemM3 = new QStandardItem(QString("Mat.3"));
    QStandardItem * itemM4 = new QStandardItem(QString("Mat.4"));
    QStandardItem * itemM5 = new QStandardItem(QString("Mat.5"));
    QStandardItem * itemM6 = new QStandardItem(QString("Mat.6"));

    matmodel->appendRow(itemM1);
    matmodel->appendRow(itemM2);
    matmodel->appendRow(itemM3);
    matmodel->appendRow(itemM4);
    matmodel->appendRow(itemM5);
    matmodel->appendRow(itemM6);


    ui->listView->setModel(matmodel);

    QStringList items;
    items<<QString("Water");
    items<<QString("Steel");
    ui->comboBoxMatDb->addItems(items);


}*/





          //QString sss = tab->objectName();


            //QString t = QString(QLatin1String("My tab"));
            //tab->addTab(dw,t);
            //tab->setCurrentIndex(1);
            /*QTreeView *tre =  tab->findChild<QTreeView*>();

            if (tre)
            {
                QStandardItemModel *model = qobject_cast<QStandardItemModel*>(tre->model());

                QString t = QString(QLatin1String("My tab"));
                //model->setHorizontalHeaderLabels(QStringList()<<t);

                // model->appendRow(itemMat);


*/

#include "moc_MatDlg.cpp"
