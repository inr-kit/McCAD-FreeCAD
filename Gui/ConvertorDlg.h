#ifndef CONVERTORDLG_H
#define CONVERTORDLG_H

#include <QDialog>
#include <TCollection_AsciiString.hxx>

#include <iostream>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>

#include "../App/McCadConversion.h"

namespace Ui {
class ConvertorDlg;
}

using namespace boost::signals2;


class ConvertorDlg : public QDialog
{
    Q_OBJECT

public:     

     explicit ConvertorDlg(QWidget *parent = 0);
     ~ConvertorDlg();

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
    Ui::ConvertorDlg *ui;
    void LoadData();

    boost::signals2::connection connect_progress_main;
    boost::signals2::connection connect_progress_part;   
    boost::signals2::connection connect_message_append;
    boost::signals2::connection connect_message_insert;

    boost::signals2::connection connect_main_label;
    boost::signals2::connection connect_part_label;

    TCollection_AsciiString m_strStepFile;              // The converted model file
    TCollection_AsciiString m_strMatFile;               // The material file
    TCollection_AsciiString m_strConfigFile;            // The config file
    TCollection_AsciiString m_strCodeOption;            // option of MC codes, tripoli or mcnp

    McCadConversion * pConvertor;
};

#endif // CONVERTORDLG_H
