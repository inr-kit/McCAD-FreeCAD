/** @file McCadSolid.hxx
  *
  * @brief  This solid is represent a compound solid of CAD file, which will be wrote as a MCNP or Tripoli
  *         cell. Normally, it contains a convex solid list.
  *
  * @author Lei Lu
  * @date   1st.Aug.2012
  */


#ifndef MCCADSOLID_HXX
#define MCCADSOLID_HXX

#include <TCollection_AsciiString.hxx>
#include "McCadConvexSolid.hxx"

class McCadSolid
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

    McCadSolid();
    ~McCadSolid();

private:

    vector <McCadConvexSolid *> m_ConvexSolidList;  /**< Convex solid list included */
    Standard_Integer m_matID;                       /**< The material ID of this solid */
    Standard_Real m_density;                        /**< The desity of the solid */
    TCollection_AsciiString m_name;                 /**< The name of the solid */

public:

    void AddConvexSolid(McCadConvexSolid *& pConvexSolid);  /**< Set convex solid list */
    vector<McCadConvexSolid *> GetConvexSolidList();        /**< Get convex solid list */

    void SetMaterial(int theMatID, double theDensity);      /**< Set the material include matid and desity */
    Standard_Integer GetMatID();                            /**< Get the solid id */
    Standard_Real GetDensity();                             /**< Get the density */
    void SetName(TCollection_AsciiString theName);          /**< Set the name of solid */
    TCollection_AsciiString GetName();                      /**< Get the name of solid */

    vector<McCadTripoliCell *> GetTripoliCellList();        /**< Get the tripoli cell list */

};

#endif // MCCADSOLID_HXX
