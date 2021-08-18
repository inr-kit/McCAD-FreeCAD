#include "PreCompiled.h"
#include "McCadConversion.h"

#include <Gui/Application.h>
#include <Gui/Document.h>

#include "Convertor/McCadVoidCellManager.hxx"
#include "Tools/McCadConvertConfig.hxx"
#include "Tools/McCadInputModelData.hxx"
#include "Tools/McCadGeneTool.hxx"

#include "../App/McCadMessager_Signal.h"

using namespace Gui;

McCadConversion::McCadConversion()
{
    stopped = false;
}

McCadConversion::~McCadConversion()
{
   // pp = nullptr;
}


void McCadConversion::run()
{
    // /home/mccad/Programme/FreeCAD-Debug/bin/McCadConfig
    // this->sig.connect(&ConvertorDlg::Message);
    // this->sig();

    stopped = false;

    McCadMessager_Signal *msg = McCadMessager_Signal::Instance();
    msg->sig_progress_main(0);
    msg->sig_main_label(QString::fromLatin1("Load the configuration"));

    // read parameter file
    if (!McCadConvertConfig::ReadPrmt(m_strConfigFile))
    {
        msg->sig_message_append(QString::fromLatin1("-> Error: Load configurations error."));
        return;
    }
    msg->sig_message_append(QString::fromLatin1(">>> Load the configurations. OK!"));

    msg->sig_main_label(QString::fromLatin1("Read the geometry"));

    McCadInputModelData input_model;
    if (!input_model.LoadSTEPModel(m_strStepFile))
    {
        msg->sig_message_append(QString::fromLatin1(">>> Error: Read geometry data error, please check the file name."));
        return;
    }
    msg->sig_message_append(QString::fromLatin1(">>> Input STEP file: %1").arg(QString::fromStdString(m_strStepFile.ToCString())));
    msg->sig_progress_main(5);   // finish the parameter reading progress

    McCadVoidCellManager * pVoidCellManager = new McCadVoidCellManager();
    Handle(TopTools_HSequenceOfShape) hInputShape = input_model.GetModelData();
    pVoidCellManager->ReadGeomData( hInputShape );

    msg->sig_progress_main(45);
    msg->sig_main_label(QString::fromLatin1("Read material data"));

    pVoidCellManager->ReadMatData(m_strMatFile);
    TCollection_AsciiString outName = m_strStepFile;        // Set the output file name
    outName.Split(outName.SearchFromEnd(".")-1);            // Remove the file extension.

    if(m_strCodeOption.IsEqual("-t") || m_strCodeOption.IsEqual("--tripoli"))
    {
        pVoidCellManager->SetConvetor("TRIPOLI");
        outName += "_Tripoli.txt";
    }
    else if(m_strCodeOption.IsEqual("-m") || m_strCodeOption.IsEqual("--mcnp"))
    {
        pVoidCellManager->SetConvetor("MCNP");
        outName += "_MCNP.txt";
    }
    else if(m_strCodeOption.IsEqual("-s") || m_strCodeOption.IsEqual("--serpent")) // Serpent is on plan
    {
        pVoidCellManager->SetConvetor("SERPENT");
        outName += "_Serpent.txt";
    }

    QString outMsg = QString::fromLatin1(">>> Output: %1")
                              .arg(QString::fromStdString(outName.ToCString()));
    msg->sig_message_append(outMsg);

    msg->sig_progress_main(50); // Finish the conversion;
    msg->sig_main_label(QString::fromLatin1("Generate the void description"));

    pVoidCellManager->SetOutFileName(outName);
    pVoidCellManager->Process();                    // Process the conversion work

    delete pVoidCellManager;
    pVoidCellManager = NULL;

    msg->sig_progress_main(100); // Finish the conversion;
    msg->sig_main_label(QString::fromLatin1("Finish"));
}


void McCadConversion::Setting(const QString strFile,
                              const QString strConfig,
                              const QString strMat,
                              const QString strOpt)
{
    m_strStepFile   = McCadGeneTool::ToAsciiString(strFile);
    m_strConfigFile = McCadGeneTool::ToAsciiString(strConfig);
    m_strMatFile    = McCadGeneTool::ToAsciiString(strMat);
    m_strCodeOption = McCadGeneTool::ToAsciiString(strOpt);
}


TCollection_AsciiString McCadConversion::getCodeOpt()
{
    if(!m_strCodeOption.IsEmpty())
        return m_strCodeOption;
}


void McCadConversion::stop()
{
    stopped = true;
}
