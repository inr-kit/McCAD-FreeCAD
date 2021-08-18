#include "PreCompiled.h"

#include "McCadLostParticle.h"
#include "ui_LostParticle.h"

#include <qfile.h>
#include <qfiledialog.h>
#include <Gui/FileDialog.h>

#include <Gui/Command.h>
#include <CXX/Objects.hxx>

McCadLostParticle::McCadLostParticle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::McCadLostParticle)
{
    ui->setupUi(this);

    QObject::connect(ui->BnLoadFile,SIGNAL(clicked()),this,SLOT(LoadFile()));
    QObject::connect(ui->BnDisplay,SIGNAL(clicked()),this,SLOT(DisplayParticle()));
}


McCadLostParticle::~McCadLostParticle()
{
    delete ui;
}


void McCadLostParticle::LoadFile()
{
    m_strFileName = QFileDialog::getOpenFileName(this,tr("Load file"),tr("All files (*.*)"));
    if (!m_strFileName.isEmpty())
    {
        ui->lineEditFile->setText(m_strFileName);
        Gui::FileDialog::setWorkingDirectory(m_strFileName);
    }
    else
    {
        return;
    }
}


void McCadLostParticle::DisplayParticle()
{
    this->close();
    Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"import Test");
    string strFn = m_strFileName.toStdString();
    //PyObject* pyParam = Py_BuildValue("s",fn);
    //Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"LostParticle.PrintLost(\"%s\")", strFn.c_str());
    //Gui::Command::doCommand(Gui::Command::DoCmd_Type::Doc,"LostParticle.ext");


    //Py_Initialize();
 //   PyObject *pModule = NULL;
//    PyObject *pClass = NULL;
//    PyObject *pArg = NULL;
//    PyObject *pResult = NULL;
//    PyObject *pFunc = NULL;

//    try {
//       Py::Module module(PyImport_ImportModule("createBox"),true);
//      //  PyObject* module = PyImport_ImportModule("LostParticle");
//    }
//    catch (Py::Exception& e) {
//        e.clear();
//    }

//    if(!(pModule=PyImport_ImportModule("Py_LostParticle")))
//    {
//        cout<<"Load module error."<<endl;
//        return;
//    }

   // if(!(pFunc=PyObject_GetAttrString(pModule,"PrintLost")))
   // {
   //     cout<<"Error"<<endl;
   //     return;
   // }

   // pArg = Py_BuildValue("s",strFn.c_str());
   // pResult = PyObject_CallObject(pFunc,pArg);
    //Py_Finalize();
}

#include "moc_McCadLostParticle.cpp"
