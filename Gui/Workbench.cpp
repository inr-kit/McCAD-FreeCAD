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

#include "Workbench.h"
#include <Gui/MenuManager.h>
#include <Gui/ToolBarManager.h>
#include "McCadUtils.h"


using namespace McCadGui;

/// @namespace McCadGui @class Workbench
TYPESYSTEM_SOURCE(McCadGui::Workbench, Gui::StdWorkbench)

Workbench::Workbench()
{
    McCadUtils::setSeed();    
}

Workbench::~Workbench()
{
}

void Workbench::activated()
{
    App::Document *doc = App::GetApplication().getActiveDocument();
    if (!doc)
    {
        m_pDataMn = new McCadDataManager();
    }
    else
    {
        m_pDataMn = new McCadDataManager(doc);
    }
}

void Workbench::deactivated()
{
    if(m_pDataMn)
    {
        delete m_pDataMn;
        m_pDataMn = NULL;
    }
}

McCadDataManager * Workbench::GetDataManager() const
{
    if(m_pDataMn)
        return m_pDataMn;
}

Gui::MenuItem* Workbench::setupMenuBar() const
{
    Gui::MenuItem* root = StdWorkbench::setupMenuBar();
    Gui::MenuItem* item = root->findItem("&Windows"); // find menu item in menu bar labelled "Windows"
    Gui::MenuItem* mccad = new Gui::MenuItem;
    root->insertItem(item, mccad);                      // insert new item before item "Windows"
    mccad->setCommand("McCad");                         // name if menu item
    *mccad << "McCad_Setting" << "McCad_Import"<< "McCad_Export"<< "Separator"
           << "McCad_Check"<<  "McCad_ColorGroups"<<"McCad_SurfInfo" <<"McCad_Material"<< "Separator"
           << "McCad_Decompose" << "Separator"
           << "McCad_ShowSelection" << "McCad_ShowAll" << "Separator" << "McCad_Files"<< "McCad_Particle";  // entries in menu


    return root;
}

Gui::ToolBarItem* Workbench::setupToolBars() const
{
    Gui::ToolBarItem* root = StdWorkbench::setupToolBars();
    Gui::ToolBarItem* mccad = new Gui::ToolBarItem(root);
    mccad->setCommand("McCad Tools");
    *mccad << "McCad_Setting" << "McCad_Import"<< "McCad_Export"<< "Separator"
           << "McCad_Check" << "McCad_ColorGroups"<< "McCad_SurfInfo"<<"McCad_Material"<< "Separator"
           << "McCad_Decompose" << "McCad_Convert" << "Separator"
           << "McCad_ShowSelection" << "McCad_ShowAll" << "Separator" << "McCad_Files"<< "McCad_Particle";
    return root;
}
