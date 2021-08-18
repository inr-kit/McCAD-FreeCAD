/***************************************************************************
 *   Copyright (c) YEAR YOUR NAME         <Your e-mail address>            *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#include "PreCompiled.h"
#ifndef _PreComp_
#endif

#include <Base/Console.h>
#include <App/Document.h>
#include <Gui/Application.h>
#include <Gui/Command.h>
#include <Gui/Document.h>
#include <Gui/MainWindow.h>
#include <Gui/FileDialog.h>
#include <Gui/ViewProviderDocumentObject.h>
#include <Gui/ViewProviderDocumentObjectGroup.h>
#include <Gui/Control.h>
#include <Gui/WorkbenchManager.h>
#include <Gui/View3DInventor.h>
#include <Gui/View3DInventorViewer.h>
#include <Gui/Action.h>
#include <Gui/BitmapFactory.h>

#include <App/DocumentObjectGroup.h>
#include <App/Annotation.h>
#include <QString>
#include <cstdlib>
#include <ctime>

#include <TopoDS_Solid.hxx>

#include <qapplication.h>
#include <qpointer.h>

#include "../App/CreateBox.h"
#include "../App/McCadSurfaceCard.h"

#include "Workbench.h"
#include "McCadLostParticle.h"
#include "McCadExporter.h"
#include "McCadConvertorDlg.h"
#include "MatDlg.h"
#include "McCadSettingDlg.h"
#include "ConfigDlg.h"
#include "McCadUtils.h"
#include "McCadSurfaceCard.h"

using namespace McCadGui;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//===========================================================================
// CmdMcCadTest THIS IS JUST A TEST COMMAND
//===========================================================================
//DEF_STD_CMD(CmdMcCadTest);

//CmdMcCadTest::CmdMcCadTest()
//  :Command("McCad_Test")
//{
//    sAppModule    = "McCad";
//    sGroup        = QT_TR_NOOP("McCad");
//    sMenuText     = QT_TR_NOOP("Hello");
//    sToolTipText  = QT_TR_NOOP("McCad Test function");
//    sWhatsThis    = QT_TR_NOOP("McCad Test function");
//    sStatusTip    = QT_TR_NOOP("McCad Test function");
//    sPixmap       = "Test1";
//    sAccel        = "CTRL+H";
//}

//void CmdMcCadTest::activated(int iMsg)
//{
//    Base::Console().Message("Hello, World!\n");
//}

//void CreateMcCadCommands(void)
//{
//    Gui::CommandManager &rcCmdMgr = Gui::Application::Instance->commandManager();
//    rcCmdMgr.addCommand(new CmdMcCadTest());
//}


bool hasWorkingDir(Gui::Document *doc)
{
    if (doc){
        string doc_name = doc->getDocument()->getName();
        McCadGui::Workbench *wb = (McCadGui::Workbench *)Gui::WorkbenchManager::instance()->active();
        if (!wb) return false;
        if(wb->name() == "McCadWorkbench")
        {
            McCadData *pData = wb->GetDataManager()->getData(doc_name);
            if (!pData) return false;
            if(pData->GetWorkingDir().isEmpty())
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        else
        {
            return false;
        }
    }

    return false;
}

// New Command/Function
DEF_STD_CMD(CmdMcCadSplit)

CmdMcCadSplit::CmdMcCadSplit()
    :Command("McCad_Split")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("McCad");
    sMenuText       = QT_TR_NOOP("Split");
    sToolTipText    = QT_TR_NOOP("Split");
    sWhatsThis      = QT_TR_NOOP("Split");
    sStatusTip      = QT_TR_NOOP("Split");
    sPixmap         = "decompose.svg";
    sAccel          = "CTRL+D";
}

void CmdMcCadSplit::activated(int iMsg)
{
    ///CreateBox pBox;
    //pBox.GetSelection();
    ///pBox.SelectSurface();
    //Base::Console().Message("Select the face!\n");
    //Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,
    //                        "import McCad");
    //Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,
    //                        "McCad.createBox()");
    //Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,
    //                        "Gui.SendMsgToActiveView(\"ViewFit\")");

    Base::Console().Message("McCad Split activated!\n");
    CreateBox pBox;
    pBox.MakeBox();

    Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"import ImportGui");
}




DEF_STD_CMD_ACL(CmdMcCadConvert);

CmdMcCadConvert::CmdMcCadConvert()
  : Command("McCad_Convert")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("McCad");
    sMenuText       = QT_TR_NOOP("Convertor");
    sToolTipText    = QT_TR_NOOP("Convertor");
    sWhatsThis      = "McCad_Convert";
    sStatusTip      = sToolTipText;
}

void CmdMcCadConvert::activated(int iMsg)
{
    Gui::CommandManager &rcCmdMgr = Gui::Application::Instance->commandManager();
    if (iMsg==0)
        rcCmdMgr.runCommandByName("McCad_Mcnp");
    else if (iMsg==1)
        rcCmdMgr.runCommandByName("McCad_Tripoli");
    else if (iMsg==2)
        rcCmdMgr.runCommandByName("McCad_Serpent");
    else
        return;

    // Since the default icon is reset when enabing/disabling the command we have
    // to explicitly set the icon of the used command.
    Gui::ActionGroup* pcAction = qobject_cast<Gui::ActionGroup*>(_pcAction);
    QList<QAction*> a = pcAction->actions();

    assert(iMsg < a.size());
    pcAction->setIcon(a[iMsg]->icon());
}


Gui::Action * CmdMcCadConvert::createAction(void)
{
    Gui::ActionGroup* pcAction = new Gui::ActionGroup(this, Gui::getMainWindow());
    pcAction->setDropDownMenu(true);
    applyCommandData(this->className(), pcAction);

    QAction* cmd0 = pcAction->addAction(QString());
    cmd0->setIcon(Gui::BitmapFactory().pixmap("McCad_Mcnp"));
    QAction* cmd1 = pcAction->addAction(QString());
    cmd1->setIcon(Gui::BitmapFactory().pixmap("McCad_Tripoli"));
    QAction* cmd2 = pcAction->addAction(QString());
    cmd2->setIcon(Gui::BitmapFactory().pixmap("McCad_Serpent"));

    _pcAction = pcAction;
    languageChange();

    pcAction->setIcon(cmd0->icon());
    int defaultId = 0;
    pcAction->setProperty("defaultAction", QVariant(defaultId));

    return pcAction;
}


void CmdMcCadConvert::languageChange()
{
    Command::languageChange();

    if (!_pcAction)
        return;

    Gui::CommandManager &rcCmdMgr = Gui::Application::Instance->commandManager();

    Gui::ActionGroup* pcAction = qobject_cast<Gui::ActionGroup*>(_pcAction);
    QList<QAction*> a = pcAction->actions();

    Gui::Command* joinMcnp = rcCmdMgr.getCommandByName("McCad_Mcnp");
    if (joinMcnp) {
        QAction* cmd0 = a[0];        
        cmd0->setText(QApplication::translate("McCadConvert", joinMcnp->getMenuText()));
        cmd0->setToolTip(QApplication::translate("McCad_Mcnp", joinMcnp->getToolTipText()));
        cmd0->setStatusTip(QApplication::translate("McCad_Mcnp", joinMcnp->getStatusTip()));
    }

    Gui::Command* joinTripoli = rcCmdMgr.getCommandByName("McCad_Tripoli");
    if (joinTripoli) {
        QAction* cmd1 = a[1];
        cmd1->setText(QApplication::translate("McCadConvert", joinTripoli->getMenuText()));
        cmd1->setToolTip(QApplication::translate("McCad_Tripoli", joinTripoli->getToolTipText()));
        cmd1->setStatusTip(QApplication::translate("McCad_Tripoli", joinTripoli->getStatusTip()));
    }

    Gui::Command* joinSerpent = rcCmdMgr.getCommandByName("McCad_Serpent");
    if (joinSerpent) {
        QAction* cmd2 = a[2];
        cmd2->setText(QApplication::translate("McCadConvert", joinSerpent->getMenuText()));
        cmd2->setToolTip(QApplication::translate("McCad_Serpent",  joinSerpent->getToolTipText()));
        cmd2->setStatusTip(QApplication::translate("McCad_Serpent", joinSerpent->getStatusTip()));
    }
}

bool CmdMcCadConvert::isActive(void)
{
    if (getActiveGuiDocument())
        return hasWorkingDir(getActiveGuiDocument());
    else
        return false;
}


DEF_STD_CMD(CmdMcCadConvertMCNP);

CmdMcCadConvertMCNP::CmdMcCadConvertMCNP()
  : Command("McCad_Mcnp")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("MCNP");
    sMenuText       = QT_TR_NOOP("MCNP");
    sToolTipText    = QT_TR_NOOP("Create the MCNP input file");
    sWhatsThis      = "McCad_Mcnp";
    sStatusTip      = sToolTipText;
    sPixmap         = "McCad_Mcnp.svg";
    eType           = ForEdit;
}

void CmdMcCadConvertMCNP::activated(int iMsg)
{
    static QWeakPointer<McCadConvertorDlg>pp;
    if(!pp)
    {
        pp = new McCadConvertorDlg(QString::fromLatin1("-m"));
    }

    McCadConvertorDlg *pDlg = pp.data();
    pDlg->setFixedSize(pDlg->width(),pDlg->height());
    pDlg->setAttribute(Qt::WA_DeleteOnClose);
    pDlg->show();
    pDlg->raise();
    pDlg->activateWindow();
}


DEF_STD_CMD(CmdMcCadConvertTripoli);

CmdMcCadConvertTripoli::CmdMcCadConvertTripoli()
  : Command("McCad_Tripoli")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("Tripoli");
    sMenuText       = QT_TR_NOOP("Tripoli");
    sToolTipText    = QT_TR_NOOP("Create the Tripoli input file");
    sWhatsThis      = "McCad_Tripoli";
    sStatusTip      = sToolTipText;
    sPixmap         = "McCad_Tripoli.svg";
    eType           = ForEdit;
}

void CmdMcCadConvertTripoli::activated(int iMsg)
{
    static QWeakPointer<McCadConvertorDlg>pp;
    if(!pp)
    {
        pp = new McCadConvertorDlg(QString::fromLatin1("-t"));
    }

    McCadConvertorDlg *pDlg = pp.data();
    pDlg->setFixedSize(pDlg->width(),pDlg->height());
    pDlg->setAttribute(Qt::WA_DeleteOnClose);
    pDlg->show();
    pDlg->raise();
    pDlg->activateWindow();
}


DEF_STD_CMD(CmdMcCadConvertSerpent);

CmdMcCadConvertSerpent::CmdMcCadConvertSerpent()
  : Command("McCad_Serpent")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("Serpent");
    sMenuText       = QT_TR_NOOP("Serpent");
    sToolTipText    = QT_TR_NOOP("Create the Serpent input file");
    sWhatsThis      = "McCad_Serpent";
    sStatusTip      = sToolTipText;
    sPixmap         = "McCad_Serpent.svg";
    eType           = ForEdit;
}

void CmdMcCadConvertSerpent::activated(int iMsg)
{

}


DEF_STD_CMD(CmdMcCadDecompose)
//DEF_STD_CMD_A(CmdMcCadColorGroups);

CmdMcCadDecompose::CmdMcCadDecompose()
    : Command("McCad_Decompose")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("McCad");
    sMenuText       = QT_TR_NOOP("Decompose");
    sToolTipText    = QT_TR_NOOP("Decompose");
    sWhatsThis      = QT_TR_NOOP("Decompose");
    sStatusTip      = QT_TR_NOOP("Decompose");
    sPixmap         = "decompose.svg";
    //sAccel          = "CTRL+D";
}


void CmdMcCadDecompose::activated(int iMsg)
{
    MatDlg *pMatDlg = new MatDlg();
    pMatDlg->setFixedSize(pMatDlg->width(),pMatDlg->height());
    pMatDlg->setModal(true);
    pMatDlg->show();
}




DEF_STD_CMD_A(CmdMcCadPythonTest)

CmdMcCadPythonTest::CmdMcCadPythonTest()
    : Command("McCad_PythonTest")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("McCad");
    sMenuText       = QT_TR_NOOP("Auto color for groups");
    sToolTipText    = QT_TR_NOOP("Auto color for groups");
    sWhatsThis      = QT_TR_NOOP("Auto color for groups");
    sStatusTip      = QT_TR_NOOP("Auto color for groups");
    sPixmap         = "setting.svg";
    //sAccel          = "CTRL+I";

}

void CmdMcCadPythonTest::activated(int iMsg)
{
    // McCadUtils::colorAllGroups();

    Gui::Command::openCommand("SelectSurface");

    //Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,
    //                        "import McCad");
    //Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,
    //                        "McCad.createBox()");
    //Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,
    //                        "Gui.SendMsgToActiveView(\"ViewFit\")");

    Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"import SelSurf");
    Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"SelSurf.SelectSurf(\"\")");

    Gui::Command::commitCommand();
    Gui::Command::updateActive();
}

bool CmdMcCadPythonTest::isActive(void)
{
    App::Document* doc = App::GetApplication().getActiveDocument();
    if (!doc || doc->findObjects(App::Annotation::getClassTypeId(),"SurfInfo").size() != 0)
        return false;
    return true;
}

DEF_STD_CMD_A(CmdMcCadSetting)

CmdMcCadSetting::CmdMcCadSetting()
    :Command("McCad_Setting")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("McCad");
    sMenuText       = QT_TR_NOOP("Setting");
    sToolTipText    = QT_TR_NOOP("Setting");
    sWhatsThis      = QT_TR_NOOP("Setting");
    sStatusTip      = QT_TR_NOOP("Settting");
    sPixmap         = "setting.svg";
    sAccel          = "CTRL+S";
}

void CmdMcCadSetting::activated(int iMsg)
{
    McCadSettingDlg *pDlg = new McCadSettingDlg();
    pDlg->setModal(true);
    pDlg->show();
}

bool CmdMcCadSetting::isActive(void)
{
    if (getActiveGuiDocument())
        return true;
    else
        return false;
}


///////////////////////////////////////////////////////////////////////////

DEF_STD_CMD_A(CmdMcCadImport)

CmdMcCadImport::CmdMcCadImport()
    : Command("McCad_Import")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("McCad");
    sMenuText       = QT_TR_NOOP("Import");
    sToolTipText    = QT_TR_NOOP("Import");
    sWhatsThis      = QT_TR_NOOP("Import");
    sStatusTip      = QT_TR_NOOP("Import");
    sPixmap         = "upload.svg";
    sAccel          = "CTRL+I";

}

void CmdMcCadImport::activated(int iMsg)
{
    McCadUtils::importStepFile();
}

bool CmdMcCadImport::isActive(void)
{
    if (getActiveGuiDocument())
        return true;
    else
        return false;
}


///////////////////////////////////////////////////////////////////////////

DEF_STD_CMD_A(CmdMcCadExport)

CmdMcCadExport::CmdMcCadExport()
    : Command("McCad_Export")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("McCad");
    sMenuText       = QT_TR_NOOP("Export");
    sToolTipText    = QT_TR_NOOP("Export");
    sWhatsThis      = QT_TR_NOOP("Export");
    sStatusTip      = QT_TR_NOOP("Export");
    sPixmap         = "Download.svg";
    sAccel          = "CTRL+E";

}

void CmdMcCadExport::activated(int iMsg)
{
    TaskExport* dlg = new TaskExport();
    Gui::Control().showDialog(dlg);
}

bool CmdMcCadExport::isActive(void)
{
    if (getActiveGuiDocument())
        return hasWorkingDir(getActiveGuiDocument());
    else
        return false;
}

///////////////////////////////////////////////////////////////////////////

DEF_STD_CMD_A(CmdMcCadCheck)
//DEF_STD_CMD_A(CmdMcCadColorGroups);

CmdMcCadCheck::CmdMcCadCheck()
    : Command("McCad_Check")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("McCad");
    sMenuText       = QT_TR_NOOP("Check the geometries");
    sToolTipText    = QT_TR_NOOP("Check the geometries");
    sWhatsThis      = QT_TR_NOOP("Check the geometries");
    sStatusTip      = QT_TR_NOOP("Check the geometries");
    sPixmap         = "check.svg";
    sAccel          = "CTRL+C";
}

void CmdMcCadCheck::activated(int iMsg)
{
    Gui::Command::openCommand("CheckSurface");
    Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"import CheckSurf");
    Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"CheckSurf.createTask()");
    Gui::Command::commitCommand();
    Gui::Command::updateActive();
}

bool CmdMcCadCheck::isActive(void)
{
    App::Document* doc = App::GetApplication().getActiveDocument();
    if (!doc || doc->countObjectsOfType(App::GeoFeature::getClassTypeId()) == 0)
        return false;
    else
        return true;
}

///////////////////////////////////////////////////////////////////////////

DEF_STD_CMD_A(CmdMcCadSurfInfo)

CmdMcCadSurfInfo::CmdMcCadSurfInfo()
    : Command("McCad_SurfInfo")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("McCad");
    sMenuText       = QT_TR_NOOP("Select surface and analyze the geometry");
    sToolTipText    = QT_TR_NOOP("Select surface and analyze the geometry");
    sWhatsThis      = QT_TR_NOOP("Select surface and analyze the geometry");
    sStatusTip      = QT_TR_NOOP("Select surface and analyze the geometry");
    sPixmap         = "face.svg";
    //sAccel          = "CTRL+F";
}

void CmdMcCadSurfInfo::activated(int iMsg)
{
//    Gui::Command::openCommand("SelectSurface");
//    Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"import SelSurf");
//    Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"SelSurf.SelectSurf(\"\")");
//    Gui::Command::commitCommand();
//    Gui::Command::updateActive();

    static McCadSurfaceCard *pp;
    static bool bSw = false;   // Switch to open the surface selection mode.

    if(!pp)
    {
        pp = new McCadSurfaceCard();
    }

    if(!bSw)
    {
        pp->startInteractiveCallback();
    }
    else
    {
        pp->stopInteractiveCallback();
    }
    bSw = !bSw;
}



bool CmdMcCadSurfInfo::isActive(void)
{
    App::Document* doc = App::GetApplication().getActiveDocument();
    if (!doc || doc->countObjectsOfType(App::GeoFeature::getClassTypeId()) == 0)
        return false;

    Gui::MDIView* view = Gui::getMainWindow()->activeWindow();
    if (view && view->isDerivedFrom(Gui::View3DInventor::getClassTypeId())) {
        Gui::View3DInventorViewer* viewer = static_cast<Gui::View3DInventor*>(view)->getViewer();
        return !viewer->isEditing();
    }

    return true;
}


///////////////////////////////////////////////////////////////////////////

DEF_STD_CMD_A(CmdMcCadColorGroups)

CmdMcCadColorGroups::CmdMcCadColorGroups()
    : Command("McCad_ColorGroups")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("McCad");
    sMenuText       = QT_TR_NOOP("Auto color for groups");
    sToolTipText    = QT_TR_NOOP("Auto color for groups");
    sWhatsThis      = QT_TR_NOOP("Auto color for groups");
    sStatusTip      = QT_TR_NOOP("Auto color for groups");
    sPixmap         = "color.svg";
    //sAccel          = "CTRL+I";
}

void CmdMcCadColorGroups::activated(int iMsg)
{
    McCadUtils::colorAllGroups();
}

bool CmdMcCadColorGroups::isActive(void)
{
    App::Document* doc = App::GetApplication().getActiveDocument();
    if (!doc || doc->countObjectsOfType(App::GeoFeature::getClassTypeId()) == 0)
        return false;
    else
        return true;
}

///////////////////////////////////////////////////////////////////////////

DEF_STD_CMD_A(CmdMcCadMaterial)

CmdMcCadMaterial::CmdMcCadMaterial()
    : Command("McCad_Material")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("McCad");
    sMenuText       = QT_TR_NOOP("Create material card");
    sToolTipText    = QT_TR_NOOP("Create material card");
    sWhatsThis      = QT_TR_NOOP("Create material card");
    sStatusTip      = QT_TR_NOOP("Create material card");
    sPixmap         = "material.svg";
    sAccel          = "CTRL+M";
}

void CmdMcCadMaterial::activated(int iMsg)
{
    MatDlg *pMatDlg = new MatDlg();
    pMatDlg->setFixedSize(pMatDlg->width(),pMatDlg->height());
    pMatDlg->setModal(true);
    pMatDlg->show();
}

bool CmdMcCadMaterial::isActive(void)
{
    App::Document* doc = App::GetApplication().getActiveDocument();
    if (!doc || doc->countObjectsOfType(App::GeoFeature::getClassTypeId()) == 0)
        return false;
    else
        return true;
}



///////////////////////////////////////////////////////////////////////////

DEF_STD_CMD_A(CmdMcCadFiles)
//DEF_STD_CMD_A(CmdMcCadColorGroups);

CmdMcCadFiles::CmdMcCadFiles()
    : Command("McCad_Files")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("McCad");
    sMenuText       = QT_TR_NOOP("File tools");
    sToolTipText    = QT_TR_NOOP("File tools");
    sWhatsThis      = QT_TR_NOOP("File tools");
    sStatusTip      = QT_TR_NOOP("File tools");
    sPixmap         = "files.svg";
    //sAccel          = "CTRL+F";
}

void CmdMcCadFiles::activated(int iMsg)
{
    Gui::Command::openCommand("LostParticle");
    Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"import MergeFiles");
    Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"MergeFiles.createTask()");
    Gui::Command::commitCommand();
    Gui::Command::updateActive();
}

bool CmdMcCadFiles::isActive(void)
{
    if (getActiveGuiDocument())
        return true;
    else
        return false;
}


///////////////////////////////////////////////////////////////////////////

DEF_STD_CMD_A(CmdMcCadShowSelection)
//DEF_STD_CMD_A(CmdMcCadColorGroups);

CmdMcCadShowSelection::CmdMcCadShowSelection()
    : Command("McCad_ShowSelection")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("McCad");
    sMenuText       = QT_TR_NOOP("Hide selected solids");
    sToolTipText    = QT_TR_NOOP("Hide selected solids");
    sWhatsThis      = QT_TR_NOOP("Hide selected solids");
    sStatusTip      = QT_TR_NOOP("Hide selected solids");
    sPixmap         = "hide.svg";
    //sAccel          = "CTRL+H";
}

void CmdMcCadShowSelection::activated(int iMsg)
{
    Gui::Command::openCommand("ShowSelection");
    Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"import Visibility");
    Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"Visibility.showSelection()");
    Gui::Command::commitCommand();
    Gui::Command::updateActive();
}

bool CmdMcCadShowSelection::isActive(void)
{
    if (getActiveGuiDocument())
        return true;
    else
        return false;
}



///////////////////////////////////////////////////////////////////////////

DEF_STD_CMD_A(CmdMcCadShowAll)
//DEF_STD_CMD_A(CmdMcCadColorGroups);

CmdMcCadShowAll::CmdMcCadShowAll()
    : Command("McCad_ShowAll")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("McCad");
    sMenuText       = QT_TR_NOOP("Show all");
    sToolTipText    = QT_TR_NOOP("Show all");
    sWhatsThis      = QT_TR_NOOP("Show all");
    sStatusTip      = QT_TR_NOOP("Show all");
    sPixmap         = "show.svg";
    //sAccel          = "CTRL+H";
}

void CmdMcCadShowAll::activated(int iMsg)
{
    Gui::Command::openCommand("ShowSelection");
    Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"import Visibility");
    Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"Visibility.showAll()");
    Gui::Command::commitCommand();
    Gui::Command::updateActive();
}

bool CmdMcCadShowAll::isActive(void)
{
    if (getActiveGuiDocument())
        return true;
    else
        return false;
}




///////////////////////////////////////////////////////////////////////////

DEF_STD_CMD_A(CmdMcCadParticle)

CmdMcCadParticle::CmdMcCadParticle()
    : Command("McCad_Particle")
{
    sAppModule      = "McCad";
    sGroup          = QT_TR_NOOP("McCad");
    sMenuText       = QT_TR_NOOP("Lost Particle");
    sToolTipText    = QT_TR_NOOP("Lost Particle");
    sWhatsThis      = QT_TR_NOOP("Lost Particle");
    sStatusTip      = QT_TR_NOOP("Lost Particle");
    sPixmap         = "particle.svg";
    //sAccel          = "CTRL+C";
}


void CmdMcCadParticle::activated(int iMsg)
{
    //Gui::Command::openCommand("LostParticle");
    //Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"import ui_LostParticle");
    //Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"myDlg = ui_LostParticle.run()");
    //Gui::Command::commitCommand();
    //Gui::Command::updateActive();

    Gui::Command::openCommand("LostParticle");
    Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"import LostParticle");
    Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"LostParticle.createTask()");
    Gui::Command::commitCommand();
    Gui::Command::updateActive();
}


bool CmdMcCadParticle::isActive(void)
{
    if (getActiveGuiDocument())
        return true;
    else
        return false;
}


void CreateMcCadCommands(void)
{
    Gui::CommandManager &rcCmdMgr = Gui::Application::Instance->commandManager();
    rcCmdMgr.addCommand(new CmdMcCadSplit());
    rcCmdMgr.addCommand(new CmdMcCadConvert());
    rcCmdMgr.addCommand(new CmdMcCadDecompose());
    rcCmdMgr.addCommand(new CmdMcCadSetting());
    rcCmdMgr.addCommand(new CmdMcCadFiles());
    rcCmdMgr.addCommand(new CmdMcCadCheck());
    rcCmdMgr.addCommand(new CmdMcCadParticle());
    rcCmdMgr.addCommand(new CmdMcCadColorGroups());
    rcCmdMgr.addCommand(new CmdMcCadImport());
    rcCmdMgr.addCommand(new CmdMcCadSurfInfo());
    rcCmdMgr.addCommand(new CmdMcCadPythonTest());
    rcCmdMgr.addCommand(new CmdMcCadConvertMCNP());
    rcCmdMgr.addCommand(new CmdMcCadConvertTripoli());
    rcCmdMgr.addCommand(new CmdMcCadConvertSerpent());
    rcCmdMgr.addCommand(new CmdMcCadExport());
    rcCmdMgr.addCommand(new CmdMcCadMaterial());
    rcCmdMgr.addCommand(new CmdMcCadShowSelection());
    rcCmdMgr.addCommand(new CmdMcCadShowAll());
}

