#ifndef MCCADTRIPOLICELL_HXX
#define MCCADTRIPOLICELL_HXX

#include <TCollection_AsciiString.hxx>
#include <vector>

using namespace std;

class McCadTripoliCell
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

    McCadTripoliCell();
    ~McCadTripoliCell();

private:

    bool m_IsVirtual;                       /**< is virtual cell */
    int m_boolOp;                           /**< boolean operations */
    int m_CellNum;                          /**< cell number */
    vector<McCadTripoliCell *> m_childCellList; /**< the child cells included */

    vector<int> plus_list;                  /**< the plus surfaces list */
    vector<int> minus_list;                 /**< the minus surfaces list */
    TCollection_AsciiString m_Equation;     /**< the generated TRIPOLI equation */

public:

    TCollection_AsciiString GetEqua();      /**< Get the equation of tripoli cell */
    void SetBoolOp(int theBoolOp);          /**< Set the Boolean opearation of the virtual cell */
    void AddSurface(int theSurfNum);        /**< Add a surface number in the surface number list */

    void AddChildCell(McCadTripoliCell* pCell);     /**< Add a tricell as a child cell */
    bool hasVirtualCell();                          /**< Has virtual cell or not */

    McCadTripoliCell * GetVirtualCell(int index);   /**< Get virtual cells */
    int GetNumVirCell();                            /**< Get the number of virtual cell */
};

#endif // MCCADTRIPOLICELL_HXX
