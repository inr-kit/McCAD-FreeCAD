#ifndef IMCCADWRITER_HXX
#define IMCCADWRITER_HXX

#include "McCadVoidCellManager.hxx"

class IMcCadWriter
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

    IMcCadWriter();
    virtual ~IMcCadWriter();

protected:
    McCadVoidCellManager * m_pManager;          /**< Corresponding manager */
    Standard_Boolean m_bHaveMaterial;           /**< If the material file is existed, set true, else set false */
    Standard_Boolean m_bGenerateVoid;           /**< The switch of void generation */
    TCollection_AsciiString m_OutputFileName;   /**< The name of output file */

    Standard_Integer m_iInitCellNum;            /**< The initial number of cell */
    Standard_Integer m_iInitFaceNum;            /**< The initial number of face */

    Standard_Integer m_iCellNumWidth;           /**< The string length of cell number */
    Standard_Integer m_iMatWidth;               /**< The string length of material number and density */
    Standard_Integer m_iCellMatWidth;           /**< The string length of cell and material info */

public:
    /** Print the output file, include head information, cell, void, surface, transform card */
    virtual void PrintFile() =0;                /**< Print the file */

    virtual void PrintCellDesc(Standard_OStream& theStream) = 0;            /**< Print the cell card */
    virtual void PrintVoidDesc(Standard_OStream& theStream) = 0;            /**< Print the void card */

    virtual void PrintHeadDesc(Standard_OStream& theStream)const = 0;       /**< Print the head */
    virtual void PrintSurfDesc(Standard_OStream& theStream) = 0;            /**< Print the surfaces list */
    virtual void PrintTrsfDesc(Standard_OStream& theStream)const = 0;       /**< Print the transform card */
    virtual void PrintMatCard(Standard_OStream& theStream)const  = 0;       /**< Print the material card */
    virtual void PrintVolumeCard(Standard_OStream& theStream)const = 0;     /**< Print the volume card */

    /** Print the group information, including material name, group name, material id and density */
    virtual void PrintGroupInfo(const int iSolid, Standard_OStream& theStream)=0;

    virtual void SetFileName(TCollection_AsciiString theFileName);    /**< Set the output file name */
    virtual void SetManager(McCadVoidCellManager *pManager);          /**< Connect to the manager */
    virtual void SetMaterial(Standard_Boolean bMat);                  /**< Set the switch of outputing of material*/
    virtual void SetVoid(Standard_Boolean bVoid);                     /**< Set the switch of outputing of void card*/
};

#endif // MCCADWRITER_HXX
