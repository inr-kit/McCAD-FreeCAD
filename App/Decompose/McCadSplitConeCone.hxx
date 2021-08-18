#ifndef MCCADSPLITCONECONE_HXX
#define MCCADSPLITCONECONE_HXX

#include "McCadDcompSolid.hxx"
#include "McCadAstSurface.hxx"
#include "McCadBndSurfCone.hxx"
#include "McCadAstSurfPlane.hxx"

class McCadSplitConeCone
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

    McCadSplitConeCone();
    ~McCadSplitConeCone();

public:
    /**< Given the solid, trace the cone surfaces and generate the splitting surface */
    void GenSplitSurfaces(McCadDcompSolid *& pSolid);

private:
    /**< Two cones have common circle edges */
    Standard_Boolean HasComCircleEdge(McCadBndSurfCone *& pSurfA,
                                      McCadBndSurfCone *& pSurfB,
                                      McCadEdge *&pEdge);

    /**< Two cones have common straight edges */
    Standard_Boolean HasComLineEdge(McCadBndSurfCone *& pSurfA,
                                    McCadBndSurfCone *& pSurfB,
                                    McCadEdge *& pEdge);

    /** Generate splitting surface through the circle */
    McCadAstSurfPlane * GenSurfThroughCircle(McCadEdge *&pEdge);
    /**< Generate splitting surfaces through the straight edge */
    McCadAstSurfPlane * GenSurfThroughLine(TopoDS_Face &theFaceA,
                                           TopoDS_Face &theFaceB,
                                           McCadEdge *&pEdge);

private:

    Standard_Real m_fLength;  /**< The length of input solid which deside the size of created splitting surface */

};

#endif // MCCADSPLITCONECONE_HXX
