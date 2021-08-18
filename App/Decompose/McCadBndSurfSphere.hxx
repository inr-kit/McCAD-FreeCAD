#ifndef MCCADBNDSURFSPHERE_HXX
#define MCCADBNDSURFSPHERE_HXX

#include "McCadBndSurface.hxx"

class McCadBndSurfSphere : public McCadBndSurface
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

    McCadBndSurfSphere();
    McCadBndSurfSphere(const TopoDS_Face & theFace);
    ~McCadBndSurfSphere();


private:

    /**< Detect the collision between sphere and triangles */
    virtual Standard_Boolean TriangleCollision(McCadTriangle *& triangle, POSITION & eSide);
    /**< If the two surfaces can be fused or not */
    virtual Standard_Boolean CanFuseSurf(McCadBndSurface *& pSurf);
    /**< Fuse the surfaces which are connected and have same geometries*/
    virtual TopoDS_Face FuseSurfaces(McCadBndSurface *& pSurf);
    /**< The point is on the surface if the distance between them less than tolerence */
    virtual Standard_Boolean IsPntOnSurf(gp_Pnt &thePnt, Standard_Real disTol);

    void GenSphere();                           /**< Generate the complete sphere */
    Standard_Real GetRadius() const;            /**< Get the radius of sphere */
    gp_Pnt GetCenter() const;                   /**< Get the center of sphere */

    gp_Pnt m_Center;                            /**< Top point of cone  */
    Standard_Real m_Radius;                     /**< SemiAngle of cone  */

    Standard_Boolean m_bHasAstSplitSurfaces;    /**< If the sphere has assisted splitting surface */
    Bnd_Box m_SphereBndBox;                     /**< The boundary box of whole cylinder */
    TopoDS_Face m_SphereSurf;                   /**< The created new cone */
};

#endif // MCCADBNDSURFSPHERE_HXX
