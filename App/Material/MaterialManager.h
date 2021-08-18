#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <map>
#include <vector>

#include <QtXml/QDomComment>
#include <QFile>
#include <QTextStream>
#include "Material.h"

using namespace std;

class MaterialManager
{
public:
    MaterialManager();
    ~MaterialManager();

public:
    void SaveXMLfile(QString theFileName);
    bool LoadXMLfile(QString theXMLfile, bool bRdSldGrpMap);

    bool InsertNewMaterial(int, double, QString, QString);  // Insert Materia into map
    bool DeleteMaterial(int index);                         // Delete Material from map
    bool UpdateMaterial(int,int,double,QString,QString);    // Change the material related to input key
    Material* GetMaterial(const int index);                           // Get the material with key
    vector<int> GetMatIDList();                                         // Get the already stored material id

    // Get material number and group information based on the input index of solids.
    void GetMatInfo(const int index, int&, QString&, double&, QString&);

    QString GetGroupName(const int index);
    int GetMatNum(const QString groupName);
    float GetMatDensity(const int index);
    QString GetMatName(const int index);

    MaterialGroup* GetMaterialGroup(const int index);

    bool InsertMatGroup(QString &, int&);                                               // Insert a new material group
    bool GenSolidGroupMap(int iSolidIDBegin, int iSolidIDEnd, QString strMatGroup);     // Generate a map connect the solid id and material group

    void GenGroupData();
    MaterialGroup * GetGroupData(const int index);
    vector<Material *> GetMatList();

    bool RemoveMaterials();
    bool ClearSolidGroupMap();

private:

    map<int,Material*> m_mapMaterial;       // map stores the material number and corresponding material
    map<QString,int> m_mapGroupMaterial;    // map stores the group and corresponding material number
    map<int,QString> m_mapSolidGroup;       // map stores the solid id and corresponding group name
    map<int,MaterialGroup*> m_groupInfo;    // map stores the solid number and material group

    vector<MaterialGroup *> m_Group;        // record information of each group

    void SaveSolidGroupMap(QDomElement &, QDomDocument &);
    void SaveMaterialGroupMap(QDomElement &, QDomDocument &);
    void SaveMaterialMap(QDomElement &, QDomDocument &);

    void LoadSolidGroupMap(QDomNodeList &theList);
    void LoadMaterialGroupMap(QDomNodeList &theList);
    void LoadMaterialMap(QDomNodeList &theList);


};

#endif // MATERIALMANAGER_H
