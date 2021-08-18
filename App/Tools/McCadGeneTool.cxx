#include "McCadGeneTool.hxx"

McCadGeneTool::McCadGeneTool()
{
}

TCollection_AsciiString McCadGeneTool::ToAsciiString(const QString& theStr)
{
    char aStr[theStr.length()];
    QByteArray ba = theStr.toAscii();
    strcpy(aStr, ba.data());
    TCollection_AsciiString anAsciiStr(aStr);
    return anAsciiStr;
}


void McCadGeneTool::WriteFile( TCollection_AsciiString outFileName,
                               Handle_TopTools_HSequenceOfShape shapes)
{
    //get writer plugin
//    TCollection_AsciiString fileExtension = outFileName;
//    fileExtension = fileExtension.Split(fileExtension.SearchFromEnd(".")-1);
//    McCadEXPlug_PluginManager* pluginManager = McCadEXPlug_PluginManager::Instance();
//    McCadEXPlug_ExchangePlugin* writerPlugin = pluginManager->GetPlugin(fileExtension);
//    // write file
//    writerPlugin->SetFilename(outFileName);
//    writerPlugin->Export(shapes);
}
