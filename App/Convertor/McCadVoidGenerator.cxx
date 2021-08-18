#include "McCadVoidGenerator.hxx"

#include <assert.h>
#include <BRepBndLib.hxx>
#include <BRepPrimAPI_MakeBox.hxx>

#include "../Tools/McCadMathTool.hxx"
#include "../Tools/McCadConvertConfig.hxx"

McCadVoidGenerator::McCadVoidGenerator()
{
}

McCadVoidGenerator::~McCadVoidGenerator()
{
}

McCadVoidGenerator::McCadVoidGenerator(McCadVoidCellManager * pManager)
{
    m_pManager = pManager;
}

/** **********************************************************************
* @brief Generate the void boxes based on the dimension of material solids
*
* @param const vector<McCadExtFace*> & theExtFaceList
* @return void
*
* @date 31/8/2012
* @modify  10/12/2013
* @author  Lei Lu
**************************************************************************/
void McCadVoidGenerator::GenVoidCells()
{
    m_pManager->SetPartProgressBar(0);

    Bnd_Box bnd_box;
    McCadGeomData * pData = m_pManager->GetGeomData();

    unsigned int iSolidSize = pData->m_ConvexSolidList.size();
    float fs = 100.0/iSolidSize;

    m_pManager->PrintInfo(".",true);
    for (unsigned int i = 0; i < iSolidSize; i++)
    {        
        m_pManager->PrintInfo(".",false);
        m_pManager->SetPartProgressBar(int((i+1)*fs));         // Move the part progress bar
        McCadConvexSolid  *pSolid = pData->m_ConvexSolidList[i];
        assert(pSolid);
        BRepBndLib::Add((TopoDS_Shape)(*pSolid),bnd_box); // Add each convex solid into bundary box
    }
    //cout<<endl;

    bnd_box.SetGap(100.0); // Set the gap between the boundary box and material solids
    Standard_Real dXmin, dYmin, dZmin, dXmax, dYmax, dZmax;
    bnd_box.Get(dXmin, dYmin, dZmin, dXmax, dYmax, dZmax);

    // Interger the dimension parameters of boundary box.
    McCadMathTool::Integer(dXmin);
    McCadMathTool::Integer(dYmin);
    McCadMathTool::Integer(dZmin);
    McCadMathTool::Integer(dXmax);
    McCadMathTool::Integer(dYmax);
    McCadMathTool::Integer(dZmax);

    gp_Pnt bnd_max_pnt(dXmax,dYmax,dZmax);
    gp_Pnt bnd_min_pnt(dXmin,dYmin,dZmin);

    // Create the boundary box.
    McCadVoidCell *pVoid = new McCadVoidCell(BRepPrimAPI_MakeBox(bnd_min_pnt,bnd_max_pnt).Solid());
    pVoid->SetBntBox(dXmin,dYmin,dZmin,dXmax,dYmax,dZmax);
    pData->m_pOutVoid = pVoid;

    /* Split the boundar box into several parts */
    int iSplit = McCadConvertConfig::GetInitVoidBoxNum();
    for (int iX = 0 ; iX < iSplit ; iX++)
    {
        Standard_Real xMin = dXmin+(dXmax-dXmin)*iX/iSplit;
        Standard_Real xMax = dXmin+(dXmax-dXmin)*(iX+1)/iSplit;
        for (int iY = 0; iY < iSplit; iY++ )
        {
           Standard_Real yMin = dYmin+(dYmax-dYmin)*iY/iSplit;
           Standard_Real yMax = dYmin+(dYmax-dYmin)*(iY+1)/iSplit;
           for (int iZ = 0; iZ < iSplit; iZ++ )
           {
              Standard_Real zMin = dZmin +(dZmax-dZmin)*iZ/iSplit;
              Standard_Real zMax = dZmin +(dZmax-dZmin)*(iZ+1)/iSplit;

              McCadMathTool::Integer(xMin);
              McCadMathTool::Integer(yMin);
              McCadMathTool::Integer(zMin);
              McCadMathTool::Integer(xMax);
              McCadMathTool::Integer(yMax);
              McCadMathTool::Integer(zMax);

              gp_Pnt max_pnt(xMax,yMax,zMax);
              gp_Pnt min_pnt(xMin,yMin,zMin);

              pData->m_listVoidCell->Append(BRepPrimAPI_MakeBox(min_pnt,max_pnt).Shape());

              McCadVoidCell * pVoid = new McCadVoidCell(BRepPrimAPI_MakeBox(min_pnt,max_pnt).Solid());
              pVoid->SetBntBox(xMin,yMin,zMin,xMax,yMax,zMax);
              pData->m_VoidCellList.push_back(pVoid);

              //cout<<".";
           }
        }
    }
    //cout<<endl;

    m_pManager->SetPartProgressBar(100);
}


/** ********************************************************************
* @brief Calculate the collision between faces of solid and void cell
*
* @param const vector<McCadExtFace*> & theExtFaceList
* @return void
*
* @date 31/8/2012
* @modify  10/12/2013
* @author  Lei Lu
***********************************************************************/
void McCadVoidGenerator::VoidSolidCollision()
{   
    McCadGeomData * pData = m_pManager->GetGeomData();

    m_pManager->SetPartProgressBar(0);
    m_pManager->PrintInfo("Check solid-solid collision",true);
    int iSolidSize = pData->m_ConvexSolidList.size();
    float fs = 100.0/iSolidSize;

    try
    {
        for (unsigned int i = 0; i < pData->m_VoidCellList.size(); i++)
        {
            McCadVoidCell *pVoid = pData->m_VoidCellList.at(i);
            assert(pVoid);           

            m_pManager->PrintInfo(QString::fromLatin1("Processing No. %1 void cell:").arg(i+1).toStdString().c_str(),true);
            m_pManager->SetPartProgressBar(int((i+1)*fs));            

            for (unsigned int j = 0; j < pData->m_ConvexSolidList.size(); j++)
            {
                McCadConvexSolid  *pSolid = pData->m_ConvexSolidList.at(j);
                assert(pSolid);

                Bnd_Box bbox_solid = pSolid->GetBntBox();
                Bnd_Box bbox_void = pVoid->GetBntBox();               

                if(bbox_void.IsOut(bbox_solid))     // Detect the boundary boxes are collied or not
                {
                    continue;
                }
                else
                {
                    // Add the collided material solid's number into the void cell's list.
                    pVoid->AddColliedSolidNum(j);
                }
            }

        }

    }
    catch(...)
    {
        cout << "#McCadVoidCellManage_Void & Face Collision Error" << endl;
    }

    m_pManager->SetPartProgressBar(100);
}




/** ********************************************************************
* @brief Calculate the collision between faces of solid and void cell
*
* @param const vector<McCadExtFace*> & theExtFaceList
* @return void
*
* @date 31/8/2012
* @modify  10/12/2013
* @author  Lei Lu
***********************************************************************/
void McCadVoidGenerator::VoidFaceCollision()
{
    McCadGeomData * pData = m_pManager->GetGeomData();

    m_pManager->SetPartProgressBar(0);
    m_pManager->PrintInfo("Check solid-face collision",true);
    int iSize = pData->m_VoidCellList.size();
    float fs = 100.0/iSize;    

    try
    {
        for (unsigned int i = 0; i < pData->m_VoidCellList.size(); i++)
        {
            McCadVoidCell *pVoid = pData->m_VoidCellList.at(i);
            assert(pVoid);
            //cout<<"Void cell " << i+1 << " is collided with:";

            m_pManager->SetPartProgressBar(int((i+1)*fs));

            pVoid->CalColliedFaces(m_pManager->GetGeomData()); // Calculate the collision between the void cell and boundary surface.
            m_pManager->PrintInfo(QString::fromLatin1("Processing No. %1 void cell:").arg(i+1).toStdString().c_str(),true);
        }
        //cout<<endl<<endl;
    }
    catch(...)
    {
        cout << "#McCadVoidCellManage_Void & Face Collision Error" << endl;
    }

    m_pManager->SetPartProgressBar(100);
}


/** ********************************************************************
* @brief Split the void cell if the length of description is beyond the
*        limitation.
*
* @param const vector<McCadExtFace*> & theExtFaceList
* @return void
*
* @date 31/8/2012
* @modify  10/12/2013
* @author  Lei Lu
***********************************************************************/
void McCadVoidGenerator::SplitVoidCell()
{
    m_pManager->SetPartProgressBar(0);

    try
    {
        McCadGeomData * pData = m_pManager->GetGeomData();
        vector <McCadVoidCell *> void_list; // Create a temporary void cell list to store the void cells after splitting.

        int iSize = pData->m_VoidCellList.size();
        float fs = 100.0/iSize;        
        int iStep = 1;

        for (unsigned int i = 0; i < pData->m_VoidCellList.size(); i++)
        {
            McCadVoidCell * pVoid = pData->m_VoidCellList.at(i);
            assert(pVoid);
            pVoid->SetSplitDepth(1);                            // Set the initial split depth.

            //cout<<"Processing the "<<i+1<<" void cell:"<<endl;
            m_pManager->PrintInfo(QString::fromLatin1("Processing the %1 void cell:").arg(iStep++).toStdString().c_str(),true);
            if( pVoid->SplitVoidCell(void_list, m_pManager->GetGeomData()))
            {                
                pData->m_VoidCellList.erase(pData->m_VoidCellList.begin()+i); // Delete the original void cell
                i--;                                            // Move the point to the former position
            }

            m_pManager->SetPartProgressBar(int((iStep)*fs));

            //cout<<endl;
        }

        for (unsigned int j = 0; j < void_list.size(); j++)
        {
            pData->m_VoidCellList.push_back(void_list.at(j));          // Add the new void cell generated into void cell list.
        }
        void_list.clear();
    }
    catch(...)
    {
        cout << "#McCadVoidCellManage_Void & Splitting Void Cell Error" << endl;
    }

}


/** ********************************************************************
* @brief Generate the surface list of void cells.
*
* @param
* @return void
*
* @date 31/8/2012
* @modify  10/12/2013
* @author  Lei Lu
***********************************************************************/
void McCadVoidGenerator::GenVoidSurfList()
{
    McCadGeomData * pData = m_pManager->GetGeomData();

    m_pManager->SetPartProgressBar(0);
    int iSize = pData->m_VoidCellList.size();
    float fs = 100.0/iSize;    

    for (unsigned int i = 0; i < pData->m_VoidCellList.size(); i++)
    {
        McCadVoidCell * pVoid = pData->m_VoidCellList.at(i);
        assert(pVoid);
        vector <McCadExtBndFace *> surf_list = pVoid->GetGeomFaceList();
        pData->AddGeomSurfList(surf_list);     // Merge the sufaces with the surface list of material solids

        m_pManager->SetPartProgressBar(int((i+1)*fs));
    }

    vector <McCadExtBndFace *> out_surf_list = pData->m_pOutVoid->GetGeomFaceList();
    pData->AddGeomSurfList(out_surf_list);     // Add the surfaces of outer space into the surface list.
}




/** ********************************************************************
* @brief The main process control function
* @param null
* @return void
*
* @date 31/10/2012
* @modify  10/12/2013
* @author  Lei Lu
***********************************************************************/
void McCadVoidGenerator::Process()
{ 
    m_pManager->SetMainLabel("Generate the void description");

    //cout<<"=============== Step.3  Generate the void cells ==============="<<endl<<endl;
    m_pManager->PrintInfo("========== Step.3  Generate the void cells =============",1);
    m_pManager->SetPartLabel("Generate the void cells");
    GenVoidCells();                 // Step 1.  Generate the void cells
    m_pManager->SetMainProgressBar(50);

    //cout<<"=============== Step.4  Detect the collisions =============== "<<endl<<endl;
    m_pManager->PrintInfo("========== Step.4  Detect the collisions =============== ",1);
    m_pManager->SetPartLabel("Detect the solid-solid collisions");
    VoidSolidCollision();           // Step 2.  Detect the collision between material solids and void cells    
    m_pManager->SetMainProgressBar(55);
    m_pManager->SetPartLabel("Detect the solid-face collision");
    VoidFaceCollision();            // Step 3.  Detect the collision between void cells and each boundary faces
    m_pManager->SetMainProgressBar(70);

    //cout<<"=============== Step.5  Split the void cell recursively"<<endl<<endl;
    m_pManager->PrintInfo("========== Step.5  Split the void cell recursively ========== ",1);
    m_pManager->SetPartLabel("Split the void cell recursively");
    SplitVoidCell();                // Step 4.  If the discription is long, split the void into small pieces
    m_pManager->SetMainProgressBar(90);

    //cout<<"=============== Step.6  Add the surfaces of void cells into surface list ==============="<<endl<<endl;
    m_pManager->PrintInfo("========== Step.6  Add the surfaces into list ============ ",1);
    m_pManager->SetPartLabel("Generate the surface list");
    GenVoidSurfList();              // Step 5.  Generate the surface list, abstract the parameters
    m_pManager->SetMainProgressBar(95);
}



