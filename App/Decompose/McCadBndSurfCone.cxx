#include "McCadBndSurfCone.hxx"

#include <assert.h>

#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBndLib.hxx>
#include <BRepAdaptor_Surface.hxx>

#include <ElSLib.hxx>
#include <Extrema_ExtPS.hxx>

#include <TColgp_HSequenceOfPnt.hxx>

#include "../McCadTool/McCadEvaluator.hxx"
#include "../McCadTool/McCadGeomTool.hxx"

McCadBndSurfCone::McCadBndSurfCone()
{
}

McCadBndSurfCone::McCadBndSurfCone(const TopoDS_Face &theFace):McCadBndSurface(theFace)
{
}

McCadBndSurfCone::~McCadBndSurfCone()
{
}


/** ***************************************************************************
* @brief  Detect the collision between cone and triangle
* @param  McCadTriangle *& triangle
*         POSITION & eSide
* @return Standard_Boolean
*
* @date 26/10/2016
* @modify 26/10/2015
* @author  Lei Lu
******************************************************************************/
Standard_Boolean McCadBndSurfCone::TriangleCollision(McCadTriangle *& triangle, POSITION & eSide)
{
    Standard_Boolean bCollision = Standard_False; // The triangle is collied with face or not
    Bnd_Box bndBoxTri  = triangle->GetBndBox();

    if(bndBoxTri.IsOut(m_ConeBndBox))
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

        BRepAdaptor_Surface BS(m_plnPeak, Standard_True);
        GeomAdaptor_Surface adptPlnPeak = BS.Surface();

        /* Evaluate the point is located at which sides of plane through the peak of cone*/
        Standard_Real aValPln = McCadEvaluator::Evaluate(adptPlnPeak, point);

        /// if the point on the outer side of the plane throughing the peak,
        /// it means that it is on the positive side of cone.
        if(aValPln > 1.0e-3)
        {
            iPosPnt ++;
        }
        else
        {
            /* Evaluate which side does the point located.*/
            Standard_Real aVal = McCadEvaluator::Evaluate(m_AdpSurface, point);

            if (aVal > 1.0e-2)              // Point located on the positive side of face
            {
                iPosPnt ++;
            }
            else if (aVal < -1.0e-2)        // Point located on the negative side of face
            {
                iNegPnt ++;
            }
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
* @brief  Generate a extended cone based on the analyzed parameters of cone
* @param  Standard_Real length
* @return void
*
* @date 26/10/2016
* @modify 26/10/2015
* @author  Lei Lu
******************************************************************************/
void McCadBndSurfCone::GenExtCone(Standard_Real length)
{
    m_ConeLength = length;

    TopLoc_Location loc;
    Handle(Geom_Surface) surface = BRep_Tool::Surface(*this,loc);

    gp_Cone cone = m_AdpSurface.Cone();                  // Get the geometry of cone

    m_Axis = cone.Position();                           // Get the coordinate system Ax3
    m_SemiAngle = cone.SemiAngle();                     // Get the semi angle of cone
    m_Dir = m_Axis.Direction();                         // Get the direction of axis
    m_Apex = cone.Apex();                               // Get the peak point of cone

    Standard_Real UMin,UMax, VMin, VMax;
    BRepTools::UVBounds(*this,UMin,UMax, VMin, VMax);

    m_Radian = Abs(UMax-UMin);   // Set the radian of cylinder

    gp_Cone gCone = m_AdpSurface.Cone();

    Standard_Real UPeak, VPeak;  // The UV values of peak of cone
    ElSLib::Parameters(gCone, m_Apex, UPeak, VPeak);

    UMin = 0.0;
    UMax = 2*M_PI;

    if (VPeak  >= VMax)
    {
        VMax = VPeak;
        VMin -= m_ConeLength/2.0;
    }
    else if (VPeak <= VMin)
    {
        VMax += m_ConeLength/2.0;
        VMin = VPeak;
    }

    m_coneSurf = BRepBuilderAPI_MakeFace(surface,UMin,UMax,VMin,VMax,1.e-7);

    BRepBndLib::Add(m_coneSurf, m_ConeBndBox);
    m_ConeBndBox.SetGap(0.0);

    /// Generate the plane through the peak of cone and the direction is axis of cone.
    gp_Pln pln(m_Apex, m_Dir);
    Standard_Real size = m_ConeLength;
    m_plnPeak = BRepBuilderAPI_MakeFace(pln,-size,size,-size,size).Face();
}



/** ***************************************************************************
* @brief  The cone can be fused with the given surface or not?
* @param  McCadBndSurface *& pSurf
* @return Standard_Boolean
*
* @date 26/10/2016
* @modify 26/10/2015
* @author  Lei Lu
******************************************************************************/
Standard_Boolean McCadBndSurfCone::CanFuseSurf(McCadBndSurface *& pSurf)
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
* @brief  Fuse the cone with given surface and generate a new surface
* @param  McCadBndSurface *& pSurf
* @return TopoDS_Face
*
* @date 26/10/2016
* @modify 26/10/2015
* @author  Lei Lu
******************************************************************************/
TopoDS_Face McCadBndSurfCone::FuseSurfaces(McCadBndSurface *& pSurf)
{
    TopoDS_Face face = McCadGeomTool::FuseCones(*this, *pSurf);
    return face;
}



/** ***************************************************************************
* @brief  The given point is on the surface or not, the distance between them
*         is less than tolerence, the point is considered as on the surface.
* @param  gp_Pnt &thePnt
*         Standard_Real disTol
* @return Standard_Boolean
*
* @date 26/10/2016
* @modify 26/10/2015
* @author  Lei Lu
******************************************************************************/
Standard_Boolean McCadBndSurfCone::IsPntOnSurf(gp_Pnt &thePnt, Standard_Real disTol)
{
    BRepAdaptor_Surface BS(m_coneSurf, Standard_True);
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
* @brief  Add the edge as a splitting edge which could be add splitting surface
* @param  McCadEdge *& pEdge
* @return void
*
* @date 02/06/2016
* @author  Lei Lu
******************************************************************************/
void McCadBndSurfCone::AddConePlnSplitEdge(McCadEdge *& pEdge)
{
    assert(pEdge);
    m_ConePlnSplitEdgeList.push_back(pEdge);
}




/** ******************************************************************************
* @brief Get the splitting edges of cone which conect the cone with plane
* @param
* @return vector<McCadEdge*>
*
* @date 27/06/2016
* @modify
* @author Lei Lu
*********************************************************************************/
vector<McCadEdge*> McCadBndSurfCone::GetConePlnSplitEdgeList() const
{
    if(!m_ConePlnSplitEdgeList.empty())
    {
        return m_ConePlnSplitEdgeList;
    }
}



/** ******************************************************************************
* @brief The cone has assisted splitting surfaces for separating cone and
*        plane.
* @param
* @return Standard_Boolean
*
* @date 15/03/2017
* @modify
* @author Lei Lu
*********************************************************************************/
Standard_Boolean McCadBndSurfCone::HasConePlnSplitSurf()
{
    if(m_ConePlnSplitEdgeList.empty() )
    {
        return Standard_False;
    }
    else
    {
        return Standard_True;
    }
}




/** ******************************************************************************
* @brief Get the peak of cone
* @param
* @return gp_Pnt
*
* @date 27/06/2016
* @modify
* @author Lei Lu
*********************************************************************************/
gp_Pnt McCadBndSurfCone::GetPeak() const
{
    return m_Apex;
}




/** ******************************************************************************
* @brief Get the direction of axis of cone
* @param
* @return gp_Dir
*
* @date 15/03/2017
* @modify
* @author Lei Lu
*********************************************************************************/
gp_Dir McCadBndSurfCone::GetDir() const
{
    return m_Dir;
}



/** ******************************************************************************
* @brief Get the semi angle of cone
* @param
* @return Standard_Real
*
* @date 15/03/2017
* @modify
* @author Lei Lu
*********************************************************************************/
Standard_Real McCadBndSurfCone::GetSemiAngle() const
{
    return m_SemiAngle;
}


/** ******************************************************************************
* @brief Get the radian of cone
* @param
* @return Standard_Real
*
* @date 27/06/2016
* @modify
* @author Lei Lu
*********************************************************************************/
Standard_Real McCadBndSurfCone::GetRadian() const
{
    return m_Radian;
}
