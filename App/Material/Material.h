#ifndef MATERIAL_H
#define MATERIAL_H

#include <QString>

class Material
{
public:
    Material();
    Material(int theID, QString theName,
             double theDensity, QString theMatCard);
    ~Material();

private:
    int m_ID;               // Material ID
    QString m_Name;         // Material Name
    double m_Density;       // Material Density
    QString m_MatCard;      // Composition of Material

public:

    int GetMatID(){return m_ID;};
    QString GetMatName(){return m_Name;};
    QString GetMatCard(){return m_MatCard;};
    double GetMatDensity(){return m_Density;};

    void SetMaterial(int theID, QString theName, double theDensity, QString theMatCard);

};

class MaterialGroup
{
public:
    MaterialGroup();
    MaterialGroup(int iStartNum, int iEndNum, QString theName, Material theMaterial);
    ~MaterialGroup();

private:
    int m_iStartNum;        // the start number of material group
    int m_iEndNum;          // the end number of material group

    int m_iCellNum;         // the number of cells in this group
    Material m_Material;    // the material of this group

    QString m_strGroupName; // the name of this group


public:
    int GetStartNum(){return m_iStartNum;};
    int GetEndNum(){return m_iEndNum;};
    int GetCellNum(){return m_iCellNum;};
    QString GetGroupName(){return m_strGroupName;};

    QString GetMaterialName();
    QString GetMaterialCard();
    double GetDensity();
    int GetMaterialID();
};


#endif // MATERIAL_H
