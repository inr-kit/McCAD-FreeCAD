#ifndef MCCADSPLITSPHERECURVESURF_HXX
#define MCCADSPLITSPHERECURVESURF_HXX

#include "McCadDcompSolid.hxx"
#include "McCadBndSurfSphere.hxx"

class McCadSplitSphereCurveSurf
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

    McCadSplitSphereCurveSurf();
    ~McCadSplitSphereCurveSurf();

    void GenSplitSurfaces(McCadDcompSolid *& pSolid);  /**< Generate the splitting surface */

private:

    /**< The sphere has common circle edge with curved surfaces */
    Standard_Boolean HasComCircleEdge(McCadBndSurfSphere *& pSurfA,
                                      McCadBndSurface *& pSurfB,
                                      McCadEdge *&pEdge);
    /**< Add a assisted splitting surfaces separating the sphere and other curved surface */
    void AddAstSplitSurf(McCadBndSurfSphere *& pSurfA,
                         McCadBndSurface *& pSurfB,
                         vector<McCadAstSurface*> & AstFaceList);

    /** Generate splitting surface through the curve */
    McCadAstSurfPlane * GenSurfThroughCurves(McCadEdge *&pEdge);

private:

    Standard_Real m_fLength;            /**< The length of input solid which deside the size of created splitting surface */

};

#endif // MCCADSPLITSPHERECURVESURF_HXX
