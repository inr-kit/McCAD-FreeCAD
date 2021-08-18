#ifndef MCCADTRIPOLIWRITER_HXX
#define MCCADTRIPOLIWRITER_HXX

#include "IMcCadWriter.hxx"

class McCadTripoliWriter : public IMcCadWriter
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

    McCadTripoliWriter();
    McCadTripoliWriter(Standard_Boolean bMat, Standard_Boolean bVoid);
    ~McCadTripoliWriter();

private:

    Standard_Integer m_virtualCellNum;  /**< The initial number of virtual cell */
    map<TCollection_AsciiString,TCollection_AsciiString> m_mapSymb;

public:
    virtual void SetVirtCellNum(Standard_Integer theNum);       /**< Set the initial virtual cell number */

    virtual void PrintCellDesc(Standard_OStream& theStream);         /**< Print the cell card */
    virtual void PrintVoidDesc(Standard_OStream& theStream);         /**< Print the void card */

    virtual void PrintFile();                                        /**< Print the file */
    virtual void PrintHeadDesc(Standard_OStream& theStream)const;    /**< Print the head */
    virtual void PrintSurfDesc(Standard_OStream& theStream);         /**< Print the surfaces list */
    virtual void PrintTrsfDesc(Standard_OStream& theStream)const;    /**< Print the transform card */
    virtual void PrintMatCard(Standard_OStream& theStream)const;     /**< Print the material card */
    virtual void PrintVolumeCard(Standard_OStream& theStream)const;  /**< Print the volume card */

private:

    /**< Print the outer spaces beside void cells and material cells */
    void PrintOutSpace(Standard_OStream& theStream);
    /** Print the group information, including material name, group name, material id and density */
    void PrintGroupInfo(const int iSolid, Standard_OStream& theStream);
    /** Generate the TRIPOLI cell object from a convex solid */
    vector<McCadTripoliCell *> GenTripoliCellList(McCadSolid *& pSolid);
    /** Generate the TRIPOLI cell object from a filling void solid */
    vector<McCadTripoliCell *> GenTripoliVoidCellList(McCadVoidCell *& pVoidCell);
    /** Generate the outer space tripoli cell list */
    vector<McCadTripoliCell *> GenOuterSpaceList(McCadVoidCell *& pVoidCell);
    /**< Find in the cell number list, are there any repeated cells */
    Standard_Boolean FindRepeatCell(int iFaceNum, vector<Standard_Integer> & list);

    vector<Standard_Real> GetSurfPrmt(IGeomFace *& pFace);  /**< Get the parameter list of a surface */
    TCollection_AsciiString GetSurfSymb(IGeomFace *& pFace);/**< Get the type of surface */

};

#endif // MCCADTRIPOLIWRITER_HXX
