#include "McCadBndSurfSphere.hxx"

#include <TColgp_HSequenceOfPnt.hxx>

#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBndLib.hxx>
#include <BRepAdaptor_Surface.hxx>

#include <ElSLib.hxx>
#include <Extrema_ExtPS.hxx>

#include "../McCadTool/McCadEvaluator.hxx"
#include "../McCadTool/McCadGeomTool.hxx"

McCadBndSurfSphere::McCadBndSurfSphere()
{
}

McCadBndSurfSphere::McCadBndSurfSphere(const TopoDS_Face &theFace):McCadBndSurface(theFace)
{
}

McCadBndSurfSphere::~McCadBndSurfSphere()
{
}


/** ***************************************************************************
* @brief  Detect the collision between sphere and triangle
* @param  McCadTriangle *& triangle
*         POSITION & eSide
* @return Standard_Boolean
*
* @date 13/03/2017
* @modify
* @author  Lei Lu
******************************************************************************/
Standard_Boolean McCadBndSurfSphere::TriangleCollision(McCadTriangle *& triangle, POSITION & eSide)
{
    Standard_Boolean bCollision = Standard_False; // The triangle is collied with face or not
    Bnd_Box bndBoxTri  = triangle->GetBndBox();

    if(bndBoxTri.IsOut(m_SphereBndBox))
    {
        eSide = POSITIVE;
        return bCollision;
    }

    Standard_Integer iPosPnt = 0;   // The number of vertex of triangle locating at positive side of face
    Standard_Integer iNegPnt = 0;   // The number of vertex of triangle locating at negative side of face

    Handle_TColgp_HSequenceOfPnt pnt_list = triangle->GetVexList();
    for (Standard_Integer i = 1; i <= pnt_list->Length(); i++)
    {
        // if the point is on the surface, do not calculate the position between them.
        gp_Pnt point = pnt_list->Value(i);
        if (IsPntOnSurf(point,1.0e-1))
        {
            continue;
        }

        /* Distinguish which side does the point located.*/
        Standard_Real aVal = McCadEvaluator::Evaluate(m_AdpSurface, point);

        if (aVal > 1.0e-2)              // Point located on the positive side of face
        {
            iPosPnt ++;
        }
        else if (aVal < -1.0e-2)        // Point located on the negative side of face
        {
            iNegPnt ++;
        }

        if (iPosPnt > 0 && iNegPnt > 0) // The points are on both sides of face
        {
            bCollision = Standard_True;
            break;
        }
        else
        {
            continue;
        }
    }

    if (bCollision)
    {
        return bCollision;
    }

    /// When the vertex of triangle locate at the positive side the cylinder,
    /// it does not mean that the triangle has no collition with surface,
    /// the edges of triangle may collied with surface
    if (iPosPnt > 0 && iNegPnt == 0)
    {
        eSide = POSITIVE;
    }
    else if (iNegPnt > 0 && iPosPnt == 0)   // The triangle on negative side of face
    {
        eSide = NEGATIVE;
    }
    else if(iNegPnt == 0 && iPosPnt == 0)
    {
        eSide = NEGATIVE;   // Need to be checked Lei Lu
    }

    bCollision = Standard_False;
    return bCollision;
}



/** ***************************************************************************
* @brief  The sphere can be fused with the given surface or not?
* @param  McCadBndSurface *& pSurf
* @return Standard_Boolean
*
* @date 11/03/2017
* @modify
* @author  Lei Lu
******************************************************************************/
Standard_Boolean McCadBndSurfSphere::CanFuseSurf(McCadBndSurface *& pSurf)
{
    if(!this->IsSameSurface(pSurf))
    {
       return Standard_False;
    }

    /// Judge the surfaces have the common edge
    for(unsigned int i = 0; i < this->GetEdgeList().size(); i++)
    {
        McCadEdge *pEdgeA = GetEdgeList().at(i);

        for(unsigned int j = 0; j < pSurf->GetEdgeList().size(); j++)
        {
            McCadEdge *pEdgeB = pSurf->GetEdgeList().at(j);
            if(pEdgeA->IsSame(pEdgeB, 1.e-5))
            {
                return Standard_True;
            }
        }
    }

    return Standard_False;

}



/** ***************************************************************************
* @brief  Fuse the sphere with given sphere and generate a new surface
* @param  McCadBndSurface *& pSurf
* @return TopoDS_Face
*
* @date 11/03/2017
* @modify
* @author  Lei Lu
******************************************************************************/
TopoDS_Face McCadBndSurfSphere::FuseSurfaces(McCadBndSurface *& pSurf)
{
    TopoDS_Face face = McCadGeomTool::FuseSphere(*this, *pSurf);
    return face;
}



/** ***************************************************************************
* @brief  The given point is on the surface or not, the distance between them
*         is less than tolerence, the point is considered as on the surface.
* @param  gp_Pnt &thePnt
*         Standard_Real disTol
* @return Standard_Boolean
*
* @date 11/03/2017
* @modify
* @author  Lei Lu
******************************************************************************/
Standard_Boolean McCadBndSurfSphere::IsPntOnSurf(gp_Pnt &thePnt, Standard_Real disTol)
{
    BRepAdaptor_Surface BS(m_SphereSurf, Standard_True);
    Standard_Real uvTol = BS.Tolerance();

    Standard_Real fu = BS.FirstUParameter();
    Standard_Real lu = BS.LastUParameter();
    Standard_Real fv = BS.FirstVParameter();
    Standard_Real lv = BS.LastVParameter();

    Extrema_ExtPS extPS(thePnt,BS,fu,lu,fv,lv,uvTol,uvTol);
    if(extPS.IsDone() && extPS.NbExt() != 0)
    {
        gp_Pnt pnt  = thePnt;
        gp_Pnt pntSurf = extPS.Point(1).Value(); // The nearest point on the surface
        // Calculate the distance between surface and point
        Standard_Real dis = Sqrt(pow(pnt.X()-pntSurf.X(),2)+pow(pntSurf.Y()-pnt.Y(),2)+pow(pnt.Z()-pntSurf.Z(),2));

        if(dis < disTol)
        {
            return Standard_True;
        }
        else
        {
            return Standard_False;
        }
    }

    return Standard_False;
}



/** ***************************************************************************
* @brief  Extract the radius and center of sphere and create a complete sphere
*         according to these parameters
* @param
* @return void
*
* @date 11/03/2017
* @modify
* @author  Lei Lu
******************************************************************************/
void McCadBndSurfSphere::GenSphere()
{
    TopLoc_Location loc;
    Handle(Geom_Surface) surface = BRep_Tool::Surface(*this,loc);

    gp_Sphere sphere = m_AdpSurface.Sphere();                   // Get the geometry of cone

    m_Center = sphere.Location();               // Get the center of sphere
    m_Radius = sphere.Radius();                 // Get the radius of sphere

    m_SphereSurf = BRepBuilderAPI_MakeFace(sphere,0,2*M_PI,-M_PI,M_PI).Face();

    BRepBndLib::Add(m_SphereSurf, m_SphereBndBox);
    m_SphereBndBox.SetGap(0.0);
}



/** ***************************************************************************
* @brief  Get the radius of sphere
* @param
* @return Standard_Real
*
* @date 11/03/2017
* @modify
* @author  Lei Lu
******************************************************************************/
Standard_Real McCadBndSurfSphere::GetRadius() const
{
    return m_Radius;
}



/** ***************************************************************************
* @brief  Get the center of sphere
* @param
* @return Standard_Real
*
* @date 11/03/2017
* @modify
* @author  Lei Lu
******************************************************************************/
gp_Pnt McCadBndSurfSphere::GetCenter() const
{
    return m_Center;
}




