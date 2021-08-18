#include "McCadSplitConeCone.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "gp_Pln.hxx"

#include "../McCadTool/McCadGeomTool.hxx"

McCadSplitConeCone::McCadSplitConeCone()
{
}

McCadSplitConeCone::~McCadSplitConeCone()
{
}


/** ***************************************************************************
* @brief Given a solid, trace the cones and cones and generate the splitting
*        surfaces separating the cones
* @param McCadDcompSolid *& pSolid
* @return void
*
* @date 10/03/2017
* @modify
* @author  Lei Lu
******************************************************************************/
void McCadSplitConeCone::GenSplitSurfaces( McCadDcompSolid *& pSolid)
{
    vector<McCadBndSurface *>  &cone_list    = pSolid->m_ConeList;
    vector<McCadAstSurface*>   &AstFaceList  = pSolid->m_AstFaceList;

    m_fLength = pSolid->m_fBoxSqLength;

    for(unsigned int i = 0; i < cone_list.size()-1; i++ )
    {
        McCadBndSurfCone *pBndSurfA = (McCadBndSurfCone*)cone_list.at(i);
        for(unsigned int j = i+1; j < cone_list.size(); j++ )
        {
            McCadBndSurfCone *pBndSurfB = (McCadBndSurfCone*)cone_list.at(j);

            // They are not same cones
            if (pBndSurfA->GetSurfNum() == pBndSurfB->GetSurfNum())
            {
                continue;
            }

            // If they are not splitting surfaces neither
            if(pBndSurfA->GetSplitSurfNum() == 0 && pBndSurfB->GetSplitSurfNum() == 0)
            {
                continue;
            }

            // If the two cones have different oritations
            if(pBndSurfA->Orientation() != pBndSurfB->Orientation())
            {
                continue;
            }


            McCadEdge *pLineEdge = NULL;
            if(HasComLineEdge(pBndSurfA,pBndSurfB,pLineEdge))
            {
               McCadAstSurfPlane * pAstFace = GenSurfThroughLine(*pBndSurfA,*pBndSurfB,pLineEdge);
               AstFaceList.push_back(pAstFace);

               //pBndSurfA->AddCylnCylnSplitEdge(pEdge);//20170311
               //pBndSurfB->AddCylnCylnSplitEdge(pEdge);
            }

            delete pLineEdge;
            pLineEdge = NULL;

            ///< Detect the two cylinders are connected with ellipse circle curve
            McCadEdge *pCurveEdge = NULL;
            if(HasComCircleEdge(pBndSurfA,pBndSurfB,pCurveEdge))
            {
                McCadAstSurfPlane * pAstFace = GenSurfThroughCircle(pCurveEdge);
                AstFaceList.push_back(pAstFace);
            }
        }
    }

    // Merge the generated assisted splitting surfaces
    //if(AstFaceList.size() >= 2)
    //{
        //CombAndMergeSurfaces(AstFaceList);
    //}
}


/** ***************************************************************************
* @brief The two cones have common curve edges, including the circle, hyperbola,
*        and parabola
* @param McCadBndSurfCone *&pSurfA,
         McCadBndSurfCone *&pSurfB,
         McCadEdge *& pEdge
* @return Standard_Boolean
*
* @date 10/03/2017
* @modify //
* @author  Lei Lu
******************************************************************************/
Standard_Boolean McCadSplitConeCone::HasComCircleEdge(McCadBndSurfCone *&pSurfA,
                                                      McCadBndSurfCone *&pSurfB,
                                                      McCadEdge *& pEdge)
{
    for(unsigned i = 0; i < pSurfA->GetEdgeList().size(); i++)
    {
        McCadEdge *pEdgeA = pSurfA->GetEdgeList().at(i);        
        if(pEdgeA->GetType() == Line && pEdgeA->GetType() == Spline )
        {
            continue;
        }

        for(unsigned j = 0; j < pSurfB->GetEdgeList().size(); j++)
        {
            McCadEdge *pEdgeB = pSurfB->GetEdgeList().at(j);

            if( pEdgeB->GetType() != pEdgeA->GetType() )
            {
                continue;
            }

            /// If the connect edge is circle or ellipse circle
            if(pEdgeB->GetType() == Circle || pEdgeB->GetType() == Ellipse)
            {
                if (pEdgeA->IsSame(pEdgeB,1.0e-5))
                {
                    pEdgeA->SetConvexity(concave);  // Set the convexity of two edges
                    pEdgeB->SetConvexity(concave);
                    pEdge = pEdgeA;
                    return Standard_True;
                }
            }

            /// If the connect edge is hyperbola or parabola
            if(pEdgeB->GetType() == Hyperbola || pEdgeB->GetType() == Parabola)
            {
                if (pEdgeA->IsSame(pEdgeB,1.0e-5))
                {
                    pEdgeA->SetConvexity(concave);  // Set the convexity of two edges
                    pEdgeB->SetConvexity(concave);
                    pEdge = pEdgeA;
                    return Standard_True;
                }
            }
        }
    }

    return Standard_False;
}



/** ***************************************************************************
* @brief Generate a surface through the circle, hyperbola and parabola 2D edges
* @param McCadEdge *& pEdge
* @return McCadAstSurfPlane *
*
* @date 10/03/2017
* @modify
* @author  Lei Lu
******************************************************************************/
McCadAstSurfPlane * McCadSplitConeCone::GenSurfThroughCircle(McCadEdge *& pEdge)
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




/** ***************************************************************************
* @brief Generate a splitting surface through straight edge
* @param TopoDS_Face &theFaceA,
         TopoDS_Face &theFaceB,
         McCadEdge *& pEdge
* @return McCadAstSurfPlane *
*
* @date 10/03/2017
* @modify
* @author  Lei Lu
******************************************************************************/
McCadAstSurfPlane * McCadSplitConeCone::GenSurfThroughLine(TopoDS_Face &theFaceA,
                                                           TopoDS_Face &theFaceB,
                                                           McCadEdge *& pEdge)
{
    gp_Pnt pntStart = pEdge->StartPoint();  /// Get the start vertex of edge
    gp_Pnt pntEnd   = pEdge->EndPoint();    /// Get the end vertex of edge

    /// Get the normals of each surface
    gp_Dir normalA = McCadGeomTool::NormalOnFace(theFaceA,pntStart);
    gp_Dir normalB = McCadGeomTool::NormalOnFace(theFaceB,pntStart);

    /// Get the middle normal of two cylinders
    gp_Dir normalMid(0,0,1);
    normalMid.SetX((normalA.X()+normalB.X())/2.0);
    normalMid.SetY((normalA.Y()+normalB.Y())/2.0);
    normalMid.SetZ((normalA.Z()+normalB.Z())/2.0);

    gp_Vec vec(pntStart,pntEnd);
    vec.Normalize();
    gp_Dir normalAB(vec);
    gp_Dir normalSplitSurf = normalAB.Crossed(normalMid); // the normal of splitting plane

    gp_Pln pln(pntStart, normalSplitSurf);

    Standard_Real size = m_fLength;
    TopoDS_Face split_face = BRepBuilderAPI_MakeFace(pln,-size,size,-size,size).Face();

    McCadAstSurfPlane *pAstFace = new McCadAstSurfPlane(split_face);
    pAstFace->SetEdge(*pEdge); // Set the edge throught by assisted splitting surface

    return pAstFace;
}





/** ***************************************************************************
* @brief The given two cones have common lines or not
* @param McCadBndSurfCone *& pSurfA,
         McCadBndSurfCone *& pSurfB,
         McCadEdge *& pEdge
* @return Standard_Boolean
*
* @date 10/03/2017
* @modify
* @author  Lei Lu
******************************************************************************/
Standard_Boolean McCadSplitConeCone::HasComLineEdge(McCadBndSurfCone *& pSurfA,
                                                    McCadBndSurfCone *& pSurfB,
                                                    McCadEdge *& pEdge)
{
    /// Judge the surfaces have the common straight edge
    for(unsigned i = 0; i < pSurfA->GetEdgeList().size(); i++)
    {
        McCadEdge *pEdgeA = pSurfA->GetEdgeList().at(i);
        if(pEdgeA->GetType() != Line || !pEdgeA->CanAddAstSplitSurf())
        {
            continue;
        }

        for(unsigned j = 0; j < pSurfB->GetEdgeList().size(); j++)
        {
            McCadEdge *pEdgeB = pSurfB->GetEdgeList().at(j);
            if(pEdgeB->GetType() != Line || !pEdgeB->CanAddAstSplitSurf())
            {
                continue;
            }

            if (pEdgeA->IsSame(pEdgeB,1.0e-5))
            {
                pEdgeA->SetConvexity(concave);    /// Set the convexities of edgeA and edgeB
                pEdgeB->SetConvexity(concave);

                pEdge = pEdgeA;
            }
        }
    }

    if ( NULL != pEdge)
    {
        return Standard_True;
    }
    else
    {
        return Standard_False;
    }

}
