#include "McCadSplitConeCyln.hxx"

#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Pln.hxx>

McCadSplitConeCyln::McCadSplitConeCyln()
{
}

McCadSplitConeCyln::~McCadSplitConeCyln()
{
}

/** ***************************************************************************
* @brief  Trace the cone surfaces and cylinder surfaces and generate the
*         splitting surfaces
* @param  McCadDcompSolid     ( Input Solid )
* @return void
*
* @date 10/03/2017
* @modify 10/03/2017
* @author  Lei Lu
******************************************************************************/
void McCadSplitConeCyln::GenSplitSurfaces( McCadDcompSolid *& pSolid)
{
    vector<McCadBndSurface *>  &cyln_list    = pSolid->m_CylinderList;
    vector<McCadBndSurface *>  &cone_list    = pSolid->m_ConeList;
    vector<McCadAstSurface*>   &AstFaceList  = pSolid->m_AstFaceList;

    m_fLength = pSolid->m_fBoxSqLength;

    //STEPControl_Writer wrt2;
    for(unsigned int i = 0; i < cyln_list.size()-1; i++ )
    {
        McCadBndSurfCylinder *pBndSurfA = (McCadBndSurfCylinder*)cyln_list.at(i);
        for(unsigned int j = i+1; j < cone_list.size(); j++ )
        {
            McCadBndSurfCone *pBndSurfB = (McCadBndSurfCone*)cone_list.at(j);

            if(pBndSurfA->GetSplitSurfNum() == 0 && pBndSurfB->GetSplitSurfNum() == 0)
            {
                continue;
            }

            ///< Detect the two cylinders are connected with circle curve
            McCadEdge *pCurveEdge = NULL;
            if(HasComCircleEdge(pBndSurfA,pBndSurfB,pCurveEdge))
            {
                McCadAstSurfPlane * pAstFace = GenSurfThroughCircle(pCurveEdge);
                pAstFace->SetCombined(Standard_True); // The assisted surface can not be merged
                AstFaceList.push_back(pAstFace);

                pBndSurfA->AddCylnCylnSplitEdge(pCurveEdge);
                //20170309 pBndSurfB->AddConeCylnSplitEdge(pCurveEdge);
            }
        }
    }
}



/** ***************************************************************************
* @brief The cone and cylinder have common circle edge or not
* @param McCadBndSurfCylinder *&pSurfA,
         McCadBndSurfCone *&pSurfB,
         McCadEdge *& pEdge        (Common circle edge)
* @return Standard_Boolean
*
* @date 10/03/2017
* @modify
* @author  Lei Lu
******************************************************************************/
Standard_Boolean McCadSplitConeCyln::HasComCircleEdge(McCadBndSurfCylinder *&pSurfA,
                                                      McCadBndSurfCone *&pSurfB,
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
                pEdgeA->SetConvexity(flat);  // Set the convexity of two edges
                pEdgeB->SetConvexity(flat);
                pEdge = pEdgeA;
                return Standard_True;
            }
        }
    }
    return Standard_False;
}



/** ***************************************************************************
* @brief Generate a splitting surface through the connected circle
* @param TopoDS_Edge &edge
* @return McCadAstSurfPlane
*
* @date 10/03/2017
* @modify
* @author  Lei Lu
******************************************************************************/
McCadAstSurfPlane * McCadSplitConeCyln::GenSurfThroughCircle(McCadEdge *& pEdge)
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








