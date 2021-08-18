/** @file McCadVoidCellManager.hxx
  *
  * @brief  The class manage the void generation and translate the CAD solids into MCNP file.
  *         There are two main functions:
  *         1. Store the data including the CAD solid the void cells generated and the surfaces.
  *         2. Control the main working flow including model reading, void filling and mcnp file printing.
  *
  * @author Lei Lu
  * @date   1st.Aug.2012
  */

#ifndef MCCADVOIDCELLMANAGER_HXX
#define MCCADVOIDCELLMANAGER_HXX

#include "McCadGeomData.hxx"
#include "McCadVoidGenerator.hxx"

#include <Standard_Integer.hxx>
#include <vector>
#include <memory>

#include "../McCadMessager_Signal.h"
#include "../Material/MaterialManager.h"

using namespace std;

class McCadVoidCellManager
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

    McCadVoidCellManager();
    ~McCadVoidCellManager();   

private:

    Standard_Boolean m_bHaveMaterial;       /**< If the material file is existed, set true, else set false */
    Standard_Boolean m_bGenerateVoid;       /**< The switch of void generation */

    auto_ptr<McCadGeomData> m_pGeomData;    /**< Smart pointer of geometry data*/
    auto_ptr<MaterialManager> m_pMat;       /**< Smart pointer of materailManager */

    TCollection_AsciiString m_Convertor;    /**< The type of output file, e.g. Tripoli or Mcnp */
    TCollection_AsciiString m_OutFileName;  /**< The name of output file */

    McCadMessager_Signal *msg;              /**< Output the messages of conversion */

public:

    /** Read the model, create the McCadGeomData object to store the geometry data. */
    void ReadGeomData(Handle_TopTools_HSequenceOfShape & solid_list);   
    /** Read the material xml file and store the material information */
    void ReadMatData(TCollection_AsciiString &theMatFile);

    McCadGeomData * GetGeomData();          /**< Get the pointer of geometry data */
    MaterialManager * GetMatManager();      /**< Get the material manager pointer */    

    Standard_Boolean GenVoid() const;       /**< Generate void or not */
    void Process();                         /**< The main procesure */

    void SetOutFileName(TCollection_AsciiString theFileName);   /**< Set the output file name */
    void SetConvetor(TCollection_AsciiString theConvertor);     /**< Set the convetor to tripoli or mcnp, or other codes */
    void PrintInfo(const char * , bool opt);                    /**< Print the messages during the conversion */
    void SetMainProgressBar(int fStep);                         /**< Set the main progress bar */
    void SetPartProgressBar(int fStep);                         /**< Set the part progress bar */
    void SetMainLabel(const char *info);                        /**< Set the text of main progress bar */
    void SetPartLabel(const char *info);                        /**< Set the text of part progress bar */

private:

    void AddMatOnSolid();                   /**< Add the material information on each solid */
};


#endif // MCCADVOIDCELLMANAGER_HXX
