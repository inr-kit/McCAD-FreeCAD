#ifndef CONFIGDLG_H
#define CONFIGDLG_H

#include <QDialog>
#include <list>

namespace Ui {
class ConfigDlg;
}

using namespace std;

class ConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDlg(QWidget *parent = 0);
    ~ConfigDlg();

private:
    void LoadData();
    void ReadConfigFile(const QString InputFileName);

    QString m_strFileName;

    list<pair<QString,QString>> m_listPrmt;

    Ui::ConfigDlg *ui;
};

#endif // CONFIGDLG_H
