#ifndef WORKINGSPACEDLG_H
#define WORKINGSPACEDLG_H

#include <QDialog>
#include "../App/McCadData.h"

namespace Ui {
class WorkingSpaceDlg;
}

class WorkingSpaceDlg : public QDialog
{
    Q_OBJECT

public:
    explicit WorkingSpaceDlg(QWidget *parent = 0);
    ~WorkingSpaceDlg();

    QString GetWorkingDir();

private Q_SLOTS:
    void SelFileFolder();
    void CheckFiles();          // Check the configuration files in the file folder
    void ConfigFile();

private:
    Ui::WorkingSpaceDlg *ui;
    QString m_strWorkingDir;
    QString m_strPath;

    McCadData *m_Data;


};

#endif // WORKINGSPACEDLG_H
