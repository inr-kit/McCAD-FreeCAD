#ifndef MCCADVOIDGENERATOR_HXX
#define MCCADVOIDGENERATOR_HXX

#include "McCadVoidCellManager.hxx"

using namespace std;

class McCadVoidGenerator
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

    McCadVoidGenerator();
    McCadVoidGenerator(McCadVoidCellManager * pManager);
    ~McCadVoidGenerator();

private:

    McCadVoidCellManager * m_pManager;

private:

    void ReadVoidCells();       /**< Read the void models. */
    void GenVoidCells();        /**< Generate the void cells according to the convex solid readed. */
    void VoidSolidCollision();  /**< Detect the collision between the void cell and material solids */
    void VoidFaceCollision();   /**< Detect the collision between the void cell and boundary surface of material solids */
    void GenVoidSurfList();     /**< Generate the void surfaces list */
    void SplitVoidCell();       /**< Split the void box when it collides with too many material solids */

public:

    void Process();             /**< Start the void generation */
};

#endif // MCCADVOIDGENERATOR_HXX
