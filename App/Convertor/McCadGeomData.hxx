#ifndef MCCADGEOMDATA_HXX
#define MCCADGEOMDATA_HXX

#include "McCadConvexSolid.hxx"
#include "McCadVoidCell.hxx"
#include "McCadTransfCard.hxx"
#include "IGeomFace.hxx"
#include "McCadGeomCone.hxx"
#include "McCadSolid.hxx"

#include <OSD_File.hxx>
#include <vector>

#include "../McCadMessager_Signal.h"

class McCadGeomData
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

    McCadGeomData();
    McCadGeomData(Handle_TopTools_HSequenceOfShape & solid_list, Standard_Boolean bGenVoid);
    ~McCadGeomData();

private:
    const McCadVoidCellManager * m_pManager;                /**< The manager object which data belong to */
    McCadMessager_Signal *msg;                              /**< Output the messages of conversion */

private:
    Standard_Integer m_iNumSolid;                           /**< The number of convex solids */

    Handle(TopTools_HSequenceOfShape) m_listConvexSolid;    /**< The input convex solid after decomposition */
    Handle(TopTools_HSequenceOfShape) m_listVoidCell;       /**< The input convex solid after decomposition */

    vector <McCadSolid *> m_SolidList;                      /**< Solid list */
    vector <McCadConvexSolid *> m_ConvexSolidList;          /**< Convex solid list */
    vector <McCadVoidCell *> m_VoidCellList;                /**< void cell list */
    McCadVoidCell * m_pOutVoid;                             /**< The out box of the whole model */

    vector <IGeomFace *> m_GeomSurfList;                    /**< Geometry surface list */    
    vector <McCadTransfCard *> m_TransfCardList;            /**< Surface transf card List */

    map<Standard_Integer,Standard_Integer> m_mapSurfNum;    /**< When sort the surface list, reset the surface number */

public:

    /**< Input the geometry solid and assign the parameters. */
    void InputData(Handle_TopTools_HSequenceOfShape & solid_list, Standard_Boolean bGenVoid);
    /**< Sort the surface list based on surface type and parameters */
    void SortSurface();
    /**< Update the face number of cell expression after sorting the surface */
    void UpdateFaceNum();
    /**< Connect the manager to data */
    void SetManager(const McCadVoidCellManager *pManager);
    /**< Add Transform card */
    Standard_Integer AddTransfCard(gp_Ax3 theAxis, gp_Pnt theApex);
    /**< Get the convex solid list */
    vector <McCadConvexSolid *> GetConvexSolid();
    /**< Get the transformation cards */
    vector <McCadTransfCard *> GetTransfCard();
    /**< Get the new surface number according to the old surface number */
    Standard_Integer GetNewFaceNum(Standard_Integer iOldNum);

private:
    /**< Traverse the face list and construct geometry surface list.*/
    void AddGeomSurfList(const vector<McCadExtBndFace *> &theExtFaceList);
    /**< Traverse the face list, if the face need to add auxiliary surface,
         generate the auxiliary faces and add them into geometry face list. */
    void AddGeomAstSurfList(const vector<McCadExtBndFace *> &theExtFaceList);

    /**< Generate a map to record the old surface number of new one */
    Standard_Boolean GenMapSurfNum(Standard_Integer,Standard_Integer);
    /**< Generate a geometry face */
    IGeomFace *GenGeomSurface(McCadExtFace *& pExtFace);
    /**< Add the generated geometry face to geometry face list */
    void AddInGeomFaceList(IGeomFace*& pGeomFace, McCadExtFace *& pExtFace);

    /** Friend function which can be used to compare the weight of each surface and sort them */
    friend Standard_Boolean compare(const IGeomFace * surfA, const IGeomFace * surfB);

public:

    friend class McCadVoidGenerator;
    friend class McCadVoidCellManager;
    friend class McCadMcnpWriter;
    friend class McCadTripoliWriter;
};

#endif // MCCADGEOMDATA_HXX
