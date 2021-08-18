#include "Material.h"

Material::Material()
{
}

Material::~Material()
{
}

/** *****************************************************************************
*
* @brief Set material
* @param int theID,                  material ID
*        QString theName,            material name
*        double theDensity,          material density
*        QString theMatCard          material card include compositions
* @return
* @date 12/11/2013
* @author  Lei Lu
*
*********************************************************************************/
Material::Material(int theID, QString theName, double theDensity, QString theMatCard)
{
    SetMaterial(theID,theName,theDensity,theMatCard);
}


/**
  * @brief Set material
  *
  * @param int theID,                  material ID
  *        QString theName,            material name
  *        double theDensity,          material density
  *        QString theMatCard          material card include compositions
  * @return
  *
  * @date 12/11/2013
  * @author  Lei Lu
*/
void Material::SetMaterial(int theID, QString theName, double theDensity, QString theMatCard)
{
    m_ID = theID;
    m_Name = theName;
    m_Density = theDensity;
    m_MatCard = theMatCard;
}






/////////////////////////  Material Group Class  /////////////////////////////

MaterialGroup::MaterialGroup()
{
}

MaterialGroup::MaterialGroup(int iStartNum, int iEndNum, QString theName, Material theMaterial)
{
    m_iStartNum = iStartNum;
    m_iEndNum = iEndNum;
    m_iCellNum = iEndNum - iStartNum + 1;
    m_strGroupName = theName;
    m_Material = theMaterial;
}

MaterialGroup::~MaterialGroup()
{
}

QString MaterialGroup::GetMaterialName()
{
    return m_Material.GetMatName();
}

double MaterialGroup::GetDensity()
{
    return m_Material.GetMatDensity();
}

int MaterialGroup::GetMaterialID()
{
    return m_Material.GetMatID();
}

QString MaterialGroup::GetMaterialCard()
{
    return m_Material.GetMatCard();
}

