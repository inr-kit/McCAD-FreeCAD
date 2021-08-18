#ifndef MCCADSPLITCONECYLN_HXX
#define MCCADSPLITCONECYLN_HXX

#include "McCadDcompSolid.hxx"
#include "McCadAstSurface.hxx"
#include "McCadBndSurfCone.hxx"
#include "McCadBndSurfCylinder.hxx"
#include "McCadAstSurfPlane.hxx"

class McCadSplitConeCyln
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

    McCadSplitConeCyln();
    ~McCadSplitConeCyln();

public:

    /**< Given a solig, generate the splitting surface */
    void GenSplitSurfaces(McCadDcompSolid *& pSolid);

private:
    /**< Detect the common circle edge of cone and cylinder */
    Standard_Boolean HasComCircleEdge(McCadBndSurfCylinder *& pSurfA,
                                      McCadBndSurfCone *& pSurfB,
                                      McCadEdge *&pEdge);

    /** Generate splitting surface through the circle */
    McCadAstSurfPlane * GenSurfThroughCircle(McCadEdge *&pEdge);

private:

    Standard_Real m_fLength;  /**< The length of input solid which deside the size of created splitting surface */

};

#endif // MCCADSPLITCONECYLN_HXX
