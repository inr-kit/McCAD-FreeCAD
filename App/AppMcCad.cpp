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
# include <Python.h>
#endif

#include <Base/Console.h>

#include <CXX/Extensions.hxx>
#include <CXX/Objects.hxx>

#include <Base/PyObjectBase.h>
#include <Base/Console.h>
#include <Base/Interpreter.h>

#include <Gui/Command.h>
#include <Gui/Document.h>
#include "CreateBox.h"
#include "../Gui/McCadConvertorDlg.h"
#include "../Gui/McCadUtils.h"

namespace McCad {
class Module : public Py::ExtensionModule<Module>
{
public:
    Module() : Py::ExtensionModule<Module>("McCad")
    {
        initialize("This module is the McCad module."); // register with Python
    }
    virtual ~Module() {}

private:

};

static PyObject * createBox(PyObject *self, PyObject *args)
{
    std::cerr << "Creat a box!" << std::endl;

    Base::Console().Message("Create a Box!!!\n");

    CreateBox pBox;
    pBox.PrintBox();

    //Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"import ImportGui");

    Py_Return;
}

struct PyMethodDef McCad_CreateBox[] = {
    {"createBox",createBox, METH_NOARGS, "Trace the selected solid and count the number of boundary surfaces"},
    {NULL,NULL}
};


static PyObject * convertor(PyObject *self, PyObject *args)
{
    std::cerr << "convert to MCNP file!" << std::endl;

    Base::Console().Message("Call the MCNP convertor!\n");

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

    //Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"import ImportGui");

    Py_Return;
}

struct PyMethodDef McCad_Convertor[] = {
    {"convertor",convertor, METH_NOARGS, "Launch the MCNP convertor"},
    {NULL,NULL}
};


static PyObject * importor(PyObject *self, PyObject *args)
{
    std::cerr << "Import the solids!" << std::endl;

    Base::Console().Message("Import the solids!\n");

    McCadUtils::importStepFile();
    //Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"import ImportGui");

    Py_Return;
}

struct PyMethodDef McCad_Import[] = {
    {"importor",importor, METH_NOARGS, "Import the solids"},
    {NULL,NULL}
};



} // namespace McCad

/* Python entry */
PyMODINIT_FUNC initMcCad() {

    // ADD YOUR CODE HERE
    //
    //
    new McCad::Module();
    Base::Console().Log("Loading McCad module... done\n");   
}

PyObject * createBox = Py_InitModule3("McCad",McCad::McCad_CreateBox,"Description");
PyObject * convertor = Py_InitModule3("McCad",McCad::McCad_Convertor,"Description");
PyObject * importor = Py_InitModule3("McCad",McCad::McCad_Import,"Description");
