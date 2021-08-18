#ifndef MCCADCONVERTORDLG_H
#define MCCADCONVERTORDLG_H

#include <QDialog>
#include <TCollection_AsciiString.hxx>

#include <iostream>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <QCloseEvent>

#include "../App/McCadConversion.h"

namespace Ui {
class McCadConvertorDlg;
}

using namespace boost::signals2;


class McCadConvertorDlg : public QDialog
{
    Q_OBJECT

public:

     explicit McCadConvertorDlg(const QString opt, QWidget *parent = 0);
     ~McCadConvertorDlg();

    void SetProgressMain(const int &i);
    void SetProgressPart(const int &i);

    void AppendMessage(const QString &str);
    void InsertMessage(const QString &str);

    void SetMainLabel(const QString &str);
    void SetPartLabel(const QString &str);   

private Q_SLOTS:
    void StartConversion();
    void StopConversion();

private:
    Ui::McCadConvertorDlg *ui;
    void LoadPrmt(); // Before the conversion, load the parameters of conversion
    void SavePrmt(); // Save the new parameters for conversion
    QString SplitString(QString &strLine);
    bool CheckFiles();    
    void OpenOutputFile();

    boost::signals2::connection connect_progress_main;
    boost::signals2::connection connect_progress_part;
    boost::signals2::connection connect_message_append;
    boost::signals2::connection connect_message_insert;

    boost::signals2::connection connect_main_label;
    boost::signals2::connection connect_part_label;

    QString m_strStepFile;              // The converted model file
    QString m_strMatFile;               // The material file
    QString m_strConfigFile;            // The config file
    QString m_strCodeOption;            // option of MC codes, tripoli or mcnp

    McCadConversion * pConvertor;
    QString m_strWorkingDir;

protected:
    void closeEvent(QCloseEvent * event);

};

#endif // MCCADCONVERTORDLG_H
