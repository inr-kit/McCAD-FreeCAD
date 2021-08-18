#include "McCadSplitSphereCurveSurf.hxx"

#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Pln.hxx>

McCadSplitSphereCurveSurf::McCadSplitSphereCurveSurf()
{
}

McCadSplitSphereCurveSurf::~McCadSplitSphereCurveSurf()
{
}


/** ***************************************************************************
* @brief  Input the solid, trace the boundary surfaces and find the common edges
*         of the sphere and other curved surfaces, then generate the assisted
*         splitting surfaces.
* @param  McCadDcompSolid     ( Input Solid )
* @return void
*
* @date 14/03/2017
* @modify
* @author  Lei Lu
******************************************************************************/
void McCadSplitSphereCurveSurf::GenSplitSurfaces( McCadDcompSolid *& pSolid)
{
    vector<McCadBndSurface *>  &sphere_list    = pSolid->m_SphereList;
    vector<McCadBndSurface *>  &cone_list      = pSolid->m_ConeList;
    vector<McCadBndSurface *>  &cylinder_list  = pSolid->m_CylinderList;

    vector<McCadAstSurface*>   &AstFaceList  = pSolid->m_AstFaceList;

    m_fLength = pSolid->m_fBoxSqLength;

    for(unsigned int i = 0; i < sphere_list.size()-1; i++ )
    {
        McCadBndSurfSphere *pBndSurfA = (McCadBndSurfSphere*)sphere_list.at(i);
        for(unsigned int j = i+1; j < sphere_list.size(); j++ )
        {
            McCadBndSurface *pBndSurfB = sphere_list.at(j);
            AddAstSplitSurf(pBndSurfA,pBndSurfB,AstFaceList);
        }
    }

    for(unsigned int i = 0; i < sphere_list.size(); i++ )
    {
        McCadBndSurfSphere *pBndSurfA = (McCadBndSurfSphere*)sphere_list.at(i);
        for(unsigned int j = 0; j < cone_list.size(); j++ )
        {
            McCadBndSurface *pBndSurfB = cone_list.at(j);
            AddAstSplitSurf(pBndSurfA,pBndSurfB,AstFaceList);
        }
    }

    for(unsigned int i = 0; i < sphere_list.size(); i++ )
    {
        McCadBndSurfSphere *pBndSurfA = (McCadBndSurfSphere*)sphere_list.at(i);
        for(unsigned int j = 0; j < cylinder_list.size(); j++ )
        {
            McCadBndSurface *pBndSurfB = cylinder_list.at(j);
            AddAstSplitSurf(pBndSurfA,pBndSurfB,AstFaceList);
        }
    }
}




/** ***************************************************************************
* @brief If a sphere connect with the neighbor curved surface, add a splitting
*        surface through the circle curve.
* @param
* @return void
*
* @date 14/03/2017
* @modify
* @author  Lei Lu
******************************************************************************/
void McCadSplitSphereCurveSurf::AddAstSplitSurf(McCadBndSurfSphere *& pSurfA,
                                                McCadBndSurface *& pSurfB,
                                                vector<McCadAstSurface*> & AstFaceList)
{
    ///< Detect the oritations of the sphere and curved surfaces
    if(pSurfA->Orientation() != pSurfB->Orientation() )
    {
        return;
    }

    ///< If the two surfaces are not splitting surface, no need to add splittin surfaces
    if(pSurfA->GetSplitSurfNum() == 0 && pSurfB->GetSplitSurfNum() == 0)
    {
        return;
    }
    else
    {
        ///< Detect the two curved surfaces are connected with circle curve
        McCadEdge *pCurveEdge = NULL;
        if(HasComCircleEdge(pSurfA,pSurfB,pCurveEdge))
        {
            McCadAstSurfPlane * pAstFace = GenSurfThroughCurves(pCurveEdge);
            pAstFace->SetCombined(Standard_True); // The assisted surface can not be merged
            AstFaceList.push_back(pAstFace);
        }
    }
}





/** ***************************************************************************
* @brief  The two surfaces have common circle edge or not, if yes then return\
*         the edge.
* @param  McCadBndSurfSphere * pSurfA,
          McCadBndSurface * pSurfB,
          McCadEdge *& pEdge
* @return Bool
*
* @date 14/03/2017
* @modify
* @author  Lei Lu
******************************************************************************/
Standard_Boolean McCadSplitSphereCurveSurf::HasComCircleEdge(McCadBndSurfSphere *&pSurfA,
                                                             McCadBndSurface *&pSurfB,
                                                             McCadEdge *& pEdge)
{
    for(unsigned i = 0; i < pSurfA->GetEdgeList().size(); i++)
    {
        McCadEdge *pEdgeA = pSurfA->GetEdgeList().at(i);
        if(pEdgeA->GetType() != Circle)
        {
            continue;
        }

        for(unsigned j = 0; j < pSurfB->GetEdgeList().size(); j++)
        {
            McCadEdge *pEdgeB = pSurfB->GetEdgeList().at(j);
            if(pEdgeB->GetType() != Circle)
            {
                continue;
            }

            if (pEdgeA->IsSame(pEdgeB,1.0e-5))
            {
                pEdgeA->SetConvexity(concave);  // Set the convexity of two edges
                pEdgeB->SetConvexity(concave);
                pEdge = pEdgeA;
                return Standard_True;
            }
        }
    }
    return Standard_False;
}





/** ***************************************************************************
* @brief Generate a splitting surface through the circle
* @param TopoDS_Edge &edge
* @return McCadAstSurfPlane
*
* @date 08/04/2015
* @modify 08/04/2016
* @author  Lei Lu
******************************************************************************/
McCadAstSurfPlane * McCadSplitSphereCurveSurf::GenSurfThroughCurves(McCadEdge *& pEdge)
{
    gp_Pnt pntStart = pEdge->StartPoint();
    gp_Pnt pntEnd = pEdge->EndPoint();
    gp_Pnt pntMid = pEdge->MidPoint();

    /// Calculate the normal vector or plane through the two edges (three points)
    gp_Vec vec1(pntStart, pntMid), vec2(pntEnd, pntMid);
    gp_Vec vec = vec1 ^ vec2;
    vec.Normalize();
    gp_Dir dir(vec);

    gp_Pln pln(pntMid, dir);

    Standard_Real size = m_fLength;

    /// Generate the surface according to the normal vector
    TopoDS_Face face = BRepBuilderAPI_MakeFace(pln,-size,size,-size,size).Face();
    McCadAstSurfPlane *pSurf = new McCadAstSurfPlane(face);
    pSurf->SetCombined(Standard_True);

    return pSurf;
}
