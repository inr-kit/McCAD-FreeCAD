#ifndef MCCADDECOMPOSE_HXX
#define MCCADDECOMPOSE_HXX

#include <Handle_TopTools_HSequenceOfShape.hxx>
#include <vector>
#include <TopoDS_Solid.hxx>
#include <TCollection_AsciiString.hxx>

#include "McCadDcompSolid.hxx"

using namespace std;

class McCadDecompose
{    
public:

    void* operator new(size_t,void* anAddress)
    {
        return anAddress;
    }
    void* operator new(size_t size)
    {
        return Standard::Allocate(size);
    }
    void  operator delete(void *anAddress)
    {
        if (anAddress) Standard::Free((Standard_Address&)anAddress);
    }

    McCadDecompose();   
    ~McCadDecompose();

    void InputData(Handle_TopTools_HSequenceOfShape & solid_list,
                   TCollection_AsciiString theFileName);            /**< Load the step file */
    void Perform();                                                 /**< Perform the decomposition and saving */

    void SetFileName( TCollection_AsciiString theFileName);         /**< File name for saving */
    void SetCombine(Standard_Integer iCombSymb);                    /**< Combine the decomposed solids or not */

private:

    Standard_Real m_fDeflection;            /**< The deflection for surface meshing */
    McCadDcompGeomData * m_pGeoData;        /**< Store the geometry data */
    TCollection_AsciiString m_fileName;     /**< File name for saving */
    Standard_Integer m_iCombSymb;           /**< Combine the decomposed solids or not */

private:

    Standard_Real CalMeshDeflection(TopoDS_Solid &theSolid);            /**< Calculate the deflection of surface meshing*/
    void SaveDecomposedSolids(TCollection_AsciiString theFileName);     /**< Save the decomposed solids */
    void Decompose();                                                   /**< Start decomposition */
    void DeleteList(vector<McCadDcompSolid*> *& pSolidList);            /**< Delete the solid list and solids*/

};

#endif // MCCADDECOMPOSE_HXX
