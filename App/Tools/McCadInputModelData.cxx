#include "McCadInputModelData.hxx"

#include <TopTools_HSequenceOfShape.hxx>

//#include <McCadEXPlug_PluginManager.hxx>
//#include <McCadEXPlug_ExchangePlugin.hxx>
//#include <McCadMessenger_Singleton.hxx>

#include <OSD_Path.hxx>
#include <OSD_File.hxx>
#include <TopoDS_Shape.hxx>

#include <STEPControl_Reader.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS.hxx>

#include "McCadConvertConfig.hxx"
#include "../McCadMessager_Signal.h"

McCadInputModelData::McCadInputModelData()
{
    m_listModelData = new TopTools_HSequenceOfShape();
}


/*bool McCadInputModelData::LoadSTEPModel(TCollection_AsciiString inputModel)
{
    // Get the working directory
    TCollection_AsciiString strDir = McCadConvertConfig::GetDirectory();
    strDir.LeftAdjust();
    strDir.RightAdjust();

    if( strDir.IsEmpty())  // If the working directory is empty
    {
        cout<<"Working directory is empty"<<endl;
        return false;
    }

    // read all files that fit the mask ( beginning with converted, ending on known extension )
    TCollection_AsciiString theCommandString;
    theCommandString = TCollection_AsciiString("ls -1 ") + strDir + TCollection_AsciiString("/") + inputModel;

    static unsigned int BUF_SIZE = 1024;
    char command_string[80];
    strcpy(command_string, std::string( theCommandString.ToCString()).c_str() );
return false;

    FILE *files;
    char in_buf[BUF_SIZE];

    //get list of files in given directory
    files = popen(command_string, "r");
    if (!files) {
        cout<<"Error in open file :"<<endl;
        return false;
    }


    // call AddFile for all files in current directory
    while (fgets(in_buf, BUF_SIZE, files)) {
        //handling line breaks
        char *w;
        w = strpbrk(in_buf, "\t\n");
        if (w)
            *w='\0';

        TCollection_AsciiString tmpName(in_buf);
        TCollection_AsciiString fileFilter(tmpName);
        fileFilter = fileFilter.Split( fileFilter.SearchFromEnd(".") -1 );
//        if(! pluginManager->CheckExtension(fileFilter)) {
//            //TCollection_AsciiString message("_#_McCadIOHelper_InputFileParser :: skipping file with unknown extension : ");
//            //message.AssignCat(tmpName);
//            //Lei2018msgr->Message( message.ToCString(), McCadMessenger_WarningMsg );
//            continue;
//        }

        cout << " -- " << tmpName.ToCString() << endl;

        //Append Files beginning with converted
        if(tmpName.Search(".stp") >= 0 || tmpName.Search(".step") >= 0) {
            if( !AddFile(tmpName) ){
//                TCollection_AsciiString message("_#_McCadIOHelper_InputFileParser :: Failed adding file : ");
//                message.AssignCat(tmpName);
//                msgr->Message( message.ToCString(), McCadMessenger_WarningMsg );
            }
        }
        else{
//            TCollection_AsciiString message("_#_McCadIOHelper_InputFileParser :: file doesn't contain \'converted\' prefix : ");
//            message.AssignCat(tmpName);
//            msgr->Message(message.ToCString(), McCadMessenger_WarningMsg);
        }
    }
    return true;
}*/


Handle_TopTools_HSequenceOfShape McCadInputModelData::ImportStepFile()
{
    Handle(TopTools_HSequenceOfShape) theEntitySequence = new TopTools_HSequenceOfShape;

    if (!myFileName.IsEmpty())
    {
        OSD_Path thePath(myFileName);
        OSD_File theFile(thePath);
        if (theFile.Exists())
        {
            STEPControl_Reader myReader;
            IFSelect_ReturnStatus myReturnStatus = myReader.ReadFile(myFileName.ToCString());

            if ( myReturnStatus == IFSelect_RetDone)
            {
                Standard_Boolean failsonly = Standard_False;
                myReader.PrintCheckLoad(failsonly, IFSelect_ItemsByEntity);

                Standard_Integer theNumberOfRoots = myReader.NbRootsForTransfer();
                myReader.PrintCheckTransfer (failsonly, IFSelect_ItemsByEntity);
                if (theNumberOfRoots != 0)
                {
                    TopoDS_Shape theShape;
                    for ( Standard_Integer i = 1; i <= 	theNumberOfRoots; i++ )
                    {
                        Standard_Boolean ok = myReader.TransferRoot(i);

                        if(ok)
                        {
                            theShape = myReader.Shape(i);

                            TopExp_Explorer ex;

                            int i = 0;

                            TopoDS_CompSolid cmpSld;
                            TopoDS_Builder builder;

                            for (ex.Init(theShape, TopAbs_SOLID); ex.More(); ex.Next())
                            {
                                TopoDS_Solid tmpSol = TopoDS::Solid(ex.Current());
                                //theEntitySequence->Append(tmpSol);
                                builder.MakeCompSolid(cmpSld);
                                builder.Add(cmpSld,tmpSol);
                                theEntitySequence->Append(cmpSld);
                            }
                        }
                        else
                            cout << "_#_McCadExDllStep_Step.cxx :: a shape could not be read!" << endl;
                    }
                    return theEntitySequence;
                }
            }
            else
            {
                return theEntitySequence;
            }
            theFile.Close();
        }
        else
        {
            return theEntitySequence;
        }
    }
    else
    {
        return theEntitySequence;
    }
}


bool McCadInputModelData::LoadSTEPModel(TCollection_AsciiString inputModelName)
{    

    // Get the working directory
    /*TCollection_AsciiString strDir = McCadConvertConfig::GetDirectory();
    strDir.LeftAdjust();
    strDir.RightAdjust();

    if( strDir.IsEmpty() )  // If the working directory is empty
    {
        cout<<"Working directory is empty"<<endl;
        return false;
    }*/

    //Append Files beginning with converted
    if(inputModelName.Search(".stp") >= 0 || inputModelName.Search(".step") >= 0)
    {
        if( !AddFile(inputModelName) )
        {
            cout<<"The file is not loaded! "<<endl;
            return false;
        }
    }
    else
    {
        cout<<"There are no step file"<<endl;
        return false;
    }
    return true;
}


bool McCadInputModelData::AddFile(TCollection_AsciiString & fileName)
{
    cout<<"FileName"<<fileName<<endl;
    myFileName = fileName;
    m_listModelData = ImportStepFile();

    if( m_listModelData->Length() < 1 )
    {
        return false;
    }
    else
    {
        return true;
    }
}


Handle_TopTools_HSequenceOfShape McCadInputModelData::GetModelData()
{
    return m_listModelData;
}
