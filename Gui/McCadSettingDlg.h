#ifndef MCCADSETTINGDLG_H
#define MCCADSETTINGDLG_H

#include <QDialog>
#include <list>

#include "../App/McCadData.h"
#include <qstandarditemmodel.h>

using namespace std;

namespace Ui {
class McCadSettingDlg;
}

class McCadSettingDlg : public QDialog
{
    Q_OBJECT

public:

    explicit McCadSettingDlg(QWidget *parent = 0);
    ~McCadSettingDlg();

    QString GetWorkingDir();

private Q_SLOTS:

    void SelFileFolder();       // Set the working directory
    void CreateConfigFile();    // Create a new configuration file
    void SetDefaultConfig();    // Set the default configurations
    void SaveData();            // Close the dialog box, and save the edit data.

private:

    Ui::McCadSettingDlg *ui;

    void CheckFiles();          // Check the configuration files in the file folder.
    void LoadData();            // Click the button of setting, read the configuration data.    

    void SetDefaultPrmt();      // Set the default parameter map.
    void UpdateTable();         // Update the data in the table.
    void ReadConfigFile(const QString InputFileName);  // Read the configuration file.

    QString m_strFileName;                      // The configuration file
    vector<pair<QString,QString>> m_vecPrmt;   // The list for storage the data from configuration file
    QString m_strWorkingDir;                    // Working directory.
    QStandardItemModel * m_pModel;              // The model for the data of TreeView
    map<QString,QString> m_mapConfig;           // The map for storing the default parameters
    McCadData *m_Data;                          // The data of document which includes the working directory

};


#endif // MCCADSETTINGDLG_H
