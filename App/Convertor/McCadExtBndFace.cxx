#include "McCadExtBndFace.hxx"
#include "../Tools/McCadGeomTool.hxx"

#include <Geom_Surface.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <gp_Pnt.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Wire.hxx>

#include <BRepBndLib.hxx>
#include <omp.h>
#include <BRepMesh_IncrementalMesh.hxx>

#include <BRepAdaptor_Curve2d.hxx>
#include <BRepClass_FaceClassifier.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <Precision.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <Poly_Triangulation.hxx>

#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepAlgoAPI_Section.hxx>

#include "McCadAddAstSurface.hxx"

#include "../Tools/McCadConvertConfig.hxx"


McCadExtBndFace::McCadExtBndFace()
{
}

McCadExtBndFace::McCadExtBndFace(const TopoDS_Face &theFace):McCadExtFace(theFace)
{
    m_bHaveAstFace = Standard_False;
    m_DiscPntList = new TColgp_HSequenceOfPnt;
    m_EdgePntList = new TColgp_HSequenceOfPnt;
    m_bFusedFace = Standard_False;

    msg = McCadMessager_Signal::Instance();
}


/** ********************************************************************
* @brief Remove the auxiliary face
*
* @param Standard_Integer i
* @return
*
* @date 31/8/2012
* @author  Lei Lu
************************************************************************/
McCadExtBndFace::~McCadExtBndFace()
{
    for (unsigned int i = 0; i < m_AstFaceList.size(); i++)
    {
      McCadExtAstFace *pFace = m_AstFaceList.at(i);
      delete pFace;
      pFace = NULL;
    }

    for (unsigned int i = 0; i < m_SameFaceList.size(); i++)
    {
      McCadExtBndFace *pFace = m_SameFaceList.at(i);
      delete pFace;
      pFace = NULL;
    }

    for (unsigned int i = 0; i < m_TriangleList.size(); i++)
    {
      McCadTriangle *pTri = m_TriangleList.at(i);
      delete pTri;
      pTri = NULL;
    }

    m_DiscPntList->Clear();
    m_EdgePntList->Clear();
}


/** ********************************************************************
* @brief Judge the face has auxiliary face or not.
*
* @param
* @return Standard_Boolean
*
* @date 31/8/2012
* @author  Lei Lu
************************************************************************/
Standard_Boolean McCadExtBndFace::HaveAstSurf()
{    
    return m_bHaveAstFace;
}



/** ********************************************************************
* @brief The surface is concave surface or not
*
* @param
* @return Standard_Boolean
*
* @date 31/8/2012
* @author  Lei Lu
************************************************************************/
Standard_Boolean McCadExtBndFace::IsConcaveCurvedFace()
{
    if(GetFaceType() == GeomAbs_Plane)
    {
        return Standard_False;
    }

    /* If the curved surface is forward, no need to add auxiliary face */
    if(this->Orientation() == TopAbs_FORWARD)
    {
        return Standard_False;
    }
    else if(this->Orientation() == TopAbs_REVERSED)
    {
        /* If the max and min UV parameter of the face is same, the face is unaviliable. */
        Standard_Real UMin,UMax, VMin, VMax;
        BRepTools::UVBounds(*this, UMin,UMax, VMin, VMax);

        /* gp::Resolution is tolerance to judge the points is same or not,
           So judge the curved face is close or not. */
        if ( fabs(UMax - UMin)<= gp::Resolution() || fabs(VMax - VMin)<= gp::Resolution())
        {
            return Standard_False;
        }
        return Standard_True;
    }
    return Standard_False;
}



/** ********************************************************************
* @brief Get the discreted sample points of face
*
* @param
* @return Handle_TColgp_HSequenceOfPnt
*
* @date 31/8/2012
* @author  Lei Lu
************************************************************************/
Handle_TColgp_HSequenceOfPnt McCadExtBndFace::GetDiscPntList()
{
    if (m_DiscPntList->Length() == 0)
    {
        m_DiscPntList = new TColgp_HSequenceOfPnt;
        m_DiscPntList = McCadGeomTool::GetFaceSamplePnt(*this);
        //cout<<"Sample points of face ----------------------------- "<<m_DiscPntList->Length()<<endl;
        //QString strMsg = QString::fromLatin1(">>> Create %1 sample points of face").arg(m_DiscPntList->Length());
        //msg->sig_message_append(strMsg);
        return m_DiscPntList;
    }
    else
    {
        return m_DiscPntList;
    }
}




/** ***************************************************************************
* @brief  Generate the triangles of surface,  and add into the list.
* @param
* @return If the mesh function is sucessful, return true, or it return false
*
* @date 13/05/2015
* @modify 13/05/2015
* @author  Lei Lu
******************************************************************************/
Standard_Boolean McCadExtBndFace::GenTriangles(Standard_Real aDeflection)
{
    Handle(Poly_Triangulation) mesh;
    BRepAdaptor_Surface BS(*this,Standard_True);
    gp_Trsf T = BS.Trsf();

    /** Generate the meshes of face **/
    try
    {
        TopLoc_Location loc;
        BRepMesh_IncrementalMesh(*this, aDeflection);
        mesh = BRep_Tool::Triangulation(*this,loc);
    }
    catch(...)
    {
      //   McCadMessenger_Singleton *msgr = McCadMessenger_Singleton::Instance();
      //  msgr->Message("McCadBndSurface::GenTriangles :: Error: Face meshing has errors",
      //                McCadMessenger_DefaultMsg);
        return Standard_False;
    }

    if (!mesh.IsNull())
    {
        Standard_Integer nNodes = mesh->NbNodes();
        TColgp_Array1OfPnt meshPnts(1,nNodes);
        meshPnts = mesh->Nodes();

        Standard_Integer nbTriangles = mesh->NbTriangles();

        Standard_Integer n1, n2, n3;                                // For getting mesh points
        const Poly_Array1OfTriangle& Triangles = mesh->Triangles(); // Get the triangle

        for (int i = 1; i <= nbTriangles; i++)
        {
            Poly_Triangle tri = Triangles(i);
            tri.Get(n1, n2, n3);
            gp_Pnt P1 = (meshPnts(n1)).Transformed(T);
            gp_Pnt P2 = (meshPnts(n2)).Transformed(T);
            gp_Pnt P3 = (meshPnts(n3)).Transformed(T);

            /** Generate a new face using the points of triangle **/
            TopoDS_Wire wire = BRepBuilderAPI_MakePolygon(P1,P2,P3,Standard_True);
            TopoDS_Face face = BRepBuilderAPI_MakeFace(wire, Standard_True);

            McCadTriangle *pTri = new McCadTriangle(face);
            pTri->AddVertex(P1);
            pTri->AddVertex(P2);
            pTri->AddVertex(P3);

            //pTri->SetSurfNum(m_iSurfNum);
            m_TriangleList.push_back(pTri);
        }
    }
    else
    {
        return Standard_False;
    }

    return Standard_True;
}


void McCadExtBndFace::SetDeflection(Standard_Real def)
{
    m_fDeflection = def;	
}



/** ***************************************************************************
* @brief  Return the triangles list of face
* @param
* @return vector<McCadTriangle *>
*
* @date 13/05/2015
* @modify 13/05/2015
* @author  Lei Lu
******************************************************************************/
vector<McCadTriangle *> McCadExtBndFace::GetTriangleList()
{
	if (m_TriangleList.empty())
    {
		GenTriangles(m_fDeflection);
        return m_TriangleList;
    }
    else
    {
        return m_TriangleList;
    }
}


/** ***************************************************************************
* @brief Trace the triangles of a face and add them into the triangle into the
*        list of current face.
* @param McCadBndSurface *& pFace
* @return void
*
* @date 13/03/2016
* @modify 13/03/2015
* @author  Lei Lu
******************************************************************************/
void McCadExtBndFace::AddTriangles(McCadExtBndFace *& pFace)
{
    for (int i = 0; i < pFace->GetTriangleList().size();  i++)
    {
        McCadTriangle *pTriangle = pFace->GetTriangleList().at(i);
        McCadTriangle *pNewTriangle = new McCadTriangle(*pTriangle); // Copy the triangles
        m_TriangleList.push_back(pNewTriangle);
    }
}




/** ********************************************************************
* @brief Get the discreted sample points of the edges
*
* @param
* @return Handle_TColgp_HSequenceOfPnt
*
* @date 31/8/2012
* @author  Lei Lu
************************************************************************/
Handle_TColgp_HSequenceOfPnt McCadExtBndFace::GetEdgePntList()
{
    if (m_EdgePntList->Length() == 0)
    {
        m_EdgePntList = new TColgp_HSequenceOfPnt;
        m_EdgePntList = McCadGeomTool::GetWireSamplePnt(*this);
        //cout<<"Sample points of edge ----------------------------- "<<m_EdgePntList->Length()<<endl;
        //QString strMsg = QString::fromLatin1(">>> Create %1 sample points of edge").arg(m_EdgePntList->Length());
        //msg->sig_message_append(strMsg);
        return m_EdgePntList;
    }
    else
    {
        return m_EdgePntList;
    }
}



/** ********************************************************************
* @brief Get assisted face list
*
* @param
* @return vector<McCadExtAstFace *>
*
* @date 31/8/2012
* @author  Lei Lu
************************************************************************/
vector<McCadExtAstFace *> McCadExtBndFace::GetAstFaces()
{
    if (m_AstFaceList.empty())
    {
        Handle_TopTools_HSequenceOfShape AstFaceList = new TopTools_HSequenceOfShape();
        const McCadExtBndFace *pFace = this;

        McCadAddAstSurface::AddAstSurf(pFace,AstFaceList);
        if (!AstFaceList->IsEmpty())      // Calculate the auxiliary faces
        {
            for (int i = 1; i <= AstFaceList->Length(); i++)
            {
               TopoDS_Face tmpFace = TopoDS::Face(AstFaceList->Value(i));
               McCadExtAstFace * pExtFace = new McCadExtAstFace(tmpFace);
               m_AstFaceList.push_back(pExtFace);
            }

            AstFaceList->Clear();
            m_bHaveAstFace = Standard_True;                 // The face has auxiliary faces
            return m_AstFaceList;
        }
        else
        {
            return m_AstFaceList;
        }
    }
    else
    {
        return m_AstFaceList;
    }
}



/** ********************************************************************
* @brief Add the assisted faces
*
* @param
* @return vector<McCadExtFace *>
*
* @date 31/8/2012
* @author  Lei Lu
************************************************************************/
void McCadExtBndFace::AddAstFaces(vector< McCadExtAstFace* > faces)
{
    // Whether the two faces have same auxilary surfaces
    Standard_Boolean bRepeat = Standard_False;
    for(Standard_Integer i = 0; i < faces.size(); i++)
    {
        McCadExtAstFace *pExtFace = faces.at(i);
        for (Standard_Integer j = 0; j < m_AstFaceList.size(); j++)
        {
            Standard_Integer iAstFaceNum = m_AstFaceList.at(j)->GetFaceNum();
            // if the two assisted faces are same surfaces with different oritation,
            // remove them, because (-1 : 1) means whole space.
            if(pExtFace->GetFaceNum() == -iAstFaceNum)
            {
                McCadExtAstFace *pAuxFace = m_AstFaceList.at(j);
                m_AstFaceList.erase(m_AstFaceList.begin()+j);
                delete pAuxFace;
                pAuxFace = NULL;
                j--;
                bRepeat = Standard_True;
                break;
            }
            // if the assisted faces are same, do not add
            else if (pExtFace->GetFaceNum() == iAstFaceNum)
            {
                bRepeat = Standard_True;
                break;
            }

            // if the assisted faces are different faces, create a new one
            // and add to the auxilary face lis of one face.
            if (!bRepeat)
            {
                //McCadExtAstFace *pExtFace = faces.at(i);
                //McCadExtAstFace *pNewFace = new McCadExtAstFace(*pExtFace);
                m_AstFaceList.push_back(pExtFace);
            }
        }
    }
}




/** ********************************************************************
* @brief Get the same face list
*
* @param
* @return vector<McCadExtFace *>
*
* @date 31/8/2012
* @author  Lei Lu
************************************************************************/
vector< McCadExtBndFace* > McCadExtBndFace::GetSameFaces()
{
    return m_SameFaceList;
}


// Lei Lu 201905
/** ********************************************************************
* @brief If a solid has two same surfaces, add one of them into the
         samefacelist of another's. and update the mesh points list
         and boundary box for collision detection.
*
* @param int iNum
* @return void
*
* @date 31/8/2012
* @author  Lei Lu
************************************************************************/
/*void McCadExtBndFace::AddSameFaces( McCadExtBndFace *& pFace)
{
    m_EdgePntList->Append(pFace->GetEdgePntList());     // Merge the discrete edge points of the two same faces
    if (McCadConvertConfig::GenerateVoid())
    {
       m_DiscPntList->Append(pFace->GetDiscPntList());  // Merge the discrete face points of the two same faces
       GetBndBox();                                     // Merge the boundary box
       m_bBox.Add(pFace->GetBndBox());
    }

    if(pFace->HaveAstSurf())
    {
       AddAstFaces(pFace->GetAstFaces());               // Merge the assisted face
    }
    m_SameFaceList.push_back(pFace);
}*/

void McCadExtBndFace::AddSameFaces( McCadExtBndFace *& pFace)
{
    
	
	if (McCadConvertConfig::GenerateVoid())
    {
       //m_DiscPntList->Append(pFace->GetDiscPntList());  // Merge the discrete face points of the two same faces
       //m_TriangleList.reserve(m_TriangleList.size()+pFace->GetTriangleList().size());
       //m_TriangleList.insert(m_TriangleList.end(), pFace->GetTriangleList().begin(), pFace->GetTriangleList().end());

	   for (int i = 0; i < pFace->GetTriangleList().size(); i++)
	   {
		   McCadTriangle *pTriangle = pFace->GetTriangleList().at(i);
		   McCadTriangle *pNewTriangle = new McCadTriangle(*pTriangle); // Copy the triangles
		   m_TriangleList.push_back(pNewTriangle);
	   }

       GetBndBox();                                     // Merge the boundary box
       m_bBox.Add(pFace->GetBndBox());
    }
    if(pFace->HaveAstSurf())
    {
       AddAstFaces(pFace->GetAstFaces());               // Merge the assisted face
    }
    m_SameFaceList.push_back(pFace);
}




///** ********************************************************************
//* @brief If a solid has two same surfaces, add one of them into the
//         samefacelist of another's. and update the mesh points list
//         and boundary box for collision detection.
//*
//* @param int iNum
//* @return void
//*
//* @date 31/8/2012
//* @author  Lei Lu
//************************************************************************/
//void McCadExtBndFace::AddSameFaces( McCadExtBndFace *& pFace)
//{
//    m_EdgePntList->Append(pFace->GetEdgePntList());     // Merge the discrete edge points of the two same faces
//    if (McCadConvertConfig::GenerateVoid())
//    {
//       m_DiscPntList->Append(pFace->GetDiscPntList());  // Merge the discrete face points of the two same faces
//       GetBndBox();                                     // Merge the boundary box
//       m_bBox.Add(pFace->GetBndBox());
//    }

//    if(pFace->HaveAstSurf())
//    {
//       AddAstFaces(pFace->GetAstFaces());               // Merge the assisted face
//    }
//    m_SameFaceList.push_back(pFace);
//}


void McCadExtBndFace::AddPntList(Handle_TColgp_HSequenceOfPnt pnt_list)
{
    m_DiscPntList->Append(pnt_list);
}




/** ********************************************************************
* @brief Merge the splited same surfaces, which are connected with a
*        common edge, add this surface into sameface list and combine
*        the sample points of them for collision detecting,
*
* @param McCadExtBndFace *& pExtFace
* @return void
*
* @date 7/2/2014
* @modify 29/07/2016
* @author  Lei Lu
************************************************************************/
void McCadExtBndFace::Merge(McCadExtBndFace *& pExtFace)
{
    AddSameFaces(pExtFace);
    m_bFusedFace = Standard_True;
}



/** ********************************************************************
* @brief Get the boundary box, which are used for collision detecting
*
* @param
* @return Bnd_Box
*
* @date 7/2/2014
* @modify 29/07/2016
* @author  Lei Lu
************************************************************************/
Bnd_Box McCadExtBndFace::GetBndBox()
{
    if(!m_bBox.IsVoid())
    {
        return m_bBox;
    }

    Bnd_Box tmpBBox;
    BRepBndLib::Add(*this, tmpBBox);

    tmpBBox.SetGap(0.0);
    Standard_Real xMin, yMin, zMin, xMax, yMax, zMax;
    tmpBBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);
    gp_Pnt lower(xMin, yMin, zMin), upper(xMax, yMax, zMax);

    McCadGeomTool::SimplifyPoint(lower);
    McCadGeomTool::SimplifyPoint(upper);

    m_bBox.Add(lower);
    m_bBox.Add(upper);

    return m_bBox;
}



/** ********************************************************************
* @brief When merge two surfaces, the boundary box has to be updated
*
* @param
* @return Bnd_Box
*
* @date 7/2/2014
* @modify 29/07/2016
* @author  Lei Lu
************************************************************************/
void McCadExtBndFace::UpdateBndBox(Bnd_Box theBBox)
{
    if(m_bBox.IsVoid())
    {
        GetBndBox();
    }
    m_bBox.Add(theBBox);
}





/** ********************************************************************
* @brief The surface is combined by two splitted same surfaces or not.
*        If it is fuse surface, when do the collision detected, do not
*        use this surface, because this surface is new generated surface
*        according to UV coordinations, which is a little bit larger than
*        the original surfaces.
*
* @param
* @return Standard_Boolean
*
* @date 7/2/2014
* @modify 29/07/2016
* @author  Lei Lu
************************************************************************/
Standard_Boolean McCadExtBndFace::IsFusedFace()
{
    return m_bFusedFace;
}



/** ********************************************************************
* @brief Remove the assisted face from the fused surface
*
* @param Standard_Integer i
* @return
*
* @date 31/8/2012
* @modify 04/09/2014
* @author  Lei Lu
************************************************************************/
void McCadExtBndFace::RemoveAstFace(Standard_Integer index)
{
   McCadExtAstFace * pFace = m_AstFaceList.at(index);
   m_AstFaceList.erase(m_AstFaceList.begin()+index);
   delete pFace;
   pFace = NULL;
}
