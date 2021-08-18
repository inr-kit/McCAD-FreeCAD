#ifndef MCCADMCNPWRITER_HXX
#define MCCADMCNPWRITER_HXX

#include "IMcCadWriter.hxx"

class McCadMcnpWriter : public IMcCadWriter
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


    McCadMcnpWriter();
    McCadMcnpWriter(Standard_Boolean bMat, Standard_Boolean bVoid);
    ~McCadMcnpWriter();

public:
    /** Print the output file, include head information, cell, void, surface, transform card */
    void PrintFile();                                   /**< Print the file */
    void PrintCellDesc(Standard_OStream& theStream);    /**< Print the cell card */
    void PrintVoidDesc(Standard_OStream& theStream);    /**< Print the void card */

    void PrintHeadDesc(Standard_OStream& theStream)const;   /**< Print the head */
    void PrintSurfDesc(Standard_OStream& theStream);        /**< Print the surfaces list */
    void PrintTrsfDesc(Standard_OStream& theStream)const;   /**< Print the transform card */
    void PrintMatCard(Standard_OStream& theStream)const;    /**< Print the material card */
    void PrintVolumeCard(Standard_OStream& theStream)const; /**< Print the volume card */

    /** Print the group information, including material name, group name, material id and density */
    virtual void PrintGroupInfo(const int iSolid, Standard_OStream& theStream);

protected:

    /**< A new cell expression writor, which generate a cell number list first, then
         remove the repeated surfaces */
    TCollection_AsciiString GetCellExpn(McCadConvexSolid *& pSolid);
    /**< Find in the cell number list, are there any repeated cells */
    Standard_Boolean FindRepeatCell(int iFaceNum, vector<Standard_Integer> &list);
    /**< Get the void expression */
    TCollection_AsciiString GetVoidExpn(McCadVoidCell *& pVoidCell);

//  TCollection_AsciiString GetCellExpnOld(McCadConvexSolid *& pSolid); /**< Get the cell expression */
};

#endif // MCCADMCNPWRITER_HXX
