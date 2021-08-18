#ifndef MCCADINPUTMODELDATA_HXX
#define MCCADINPUTMODELDATA_HXX


#include <Handle_TopTools_HSequenceOfShape.hxx>
#include <TCollection_AsciiString.hxx>

class McCadInputModelData
{
public:
    McCadInputModelData();

public:
    bool LoadSTEPModel(TCollection_AsciiString inputModelName);     // Load STEP file
    Handle_TopTools_HSequenceOfShape GetModelData();            // Get the loaded step file
    Handle_TopTools_HSequenceOfShape ImportStepFile();          // Import the Step file
    bool AddFile(TCollection_AsciiString & fileName);           // Add the step file into the solid sequence

private:
    Handle_TopTools_HSequenceOfShape m_listModelData; // Add by Lei, 13/08/2012
    TCollection_AsciiString myFileName;                         // The file name of STEP file


};

#endif // MCCADINPUTMODELDATA_HXX
