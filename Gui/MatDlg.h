#ifndef MATDLG_H
#define MATDLG_H

#include <App/Document.h>
#include <Gui/Document.h>
#include <vector>

#include <QDialog>
#include <QTreeWidgetItem>
#include "../App/Material/MaterialManager.h"
#include "../App/McCadData.h"

enum ItemType{ GROUP = 1, SOLID  = 2,ROOT = 0,DOC = 0,OTHER = 0};
using namespace std;

namespace Ui {
class MatDlg;
}

class MatDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MatDlg(QWidget *parent = 0);
    ~MatDlg();

private:

    ItemType CheckItem(QTreeWidgetItem *&item); // Check the type of item in the comb viewer, is solid or group

    void LoadGroups();                          // When open the material dialogbox, load the groups of current document
    void LoadMaterials();                       // When open the material dialogbox, load the materials of current document

    void ShowMaterial(QString strSelectedGrp);  // Select the group and show the material of group
    void DisplayMaterial(int &iItem);           // Display materials when select the material item

    void Refresh();                             // Refresh the data
    void ClearEditor();                         // Clear all editors
    bool CheckInput(QString &strErrorMsg);      // Check if the input data has right format

    bool CheckBeforeSave();                     // Check if all groups are assigend materials
    void CrtSolidGroupMap(QTreeWidgetItem *&pGroupItem, unsigned int &iSolid);

public slots:

    void AddMaterial();                         // Add the material in the matarial list
    void DelMaterial();                         // Delete the material from the material list
    void ModifyMaterial();                      // Modify the selected material
    void ShowSelectedGroup();                   // Click the group item, show the material
    void ConnectGroupMat();                     // Connect the group and material
    void ShowSelectedMaterial(QTreeWidgetItem* item,int colume); // Click the material item, show the details

    void LoadXMLFile();                         // Load the xml and read the materials
    void SaveXMLFile();                         // Save the XML File
    void AutoSetMaterial();                     // According to the material groups, set the materials automatically

private:
    Ui::MatDlg *ui;

    unsigned int  m_ID;                         // Material ID
    double        m_Density;                    // Material Density
    QString       m_Name;                       // Material Name
    QString       m_Card;                       // Material Card   

    MaterialManager * m_MatManager;             // Material manager for the current document
    McCadData * m_Data;                         // Some data of current document
};

#endif // MATDLG_H
