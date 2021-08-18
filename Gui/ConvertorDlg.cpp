#include "PreCompiled.h"

#include <iostream>
#include <qscrollbar.h>
#include <qtextcursor.h>
#include "ConvertorDlg.h"
#include "ui_Convertor.h"

#include "../App/McCadMessager_Signal.h"

using namespace std;

ConvertorDlg::ConvertorDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConvertorDlg)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    flags |= Qt::WindowMinMaxButtonsHint;
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    connect(ui->BntStart, SIGNAL(clicked()), this, SLOT(StartConversion()));
    connect(ui->BntStop, SIGNAL(clicked()), this, SLOT(StopConversion()));

    ui->progressBar_Main->setMaximum(100);
    ui->progressBar_Main->setMinimum(0);

    ui->progressBar_Part->setMaximum(100);
    ui->progressBar_Part->setMinimum(0);

    McCadMessager_Signal *msg = McCadMessager_Signal::Instance();
    connect_progress_part  = msg->sig_progress_part.connect(boost::bind(&ConvertorDlg::SetProgressPart,this,_1));
    connect_progress_main = msg->sig_progress_main.connect(boost::bind(&ConvertorDlg::SetProgressMain,this,_1));
    connect_message_append = msg->sig_message_append.connect(boost::bind(&ConvertorDlg::AppendMessage,this,_1));
    connect_message_insert = msg->sig_message_insert.connect(boost::bind(&ConvertorDlg::InsertMessage,this,_1));

    connect_part_label = msg->sig_part_label.connect(boost::bind(&ConvertorDlg::SetPartLabel,this,_1));
    connect_main_label = msg->sig_main_label.connect(boost::bind(&ConvertorDlg::SetMainLabel,this,_1));

    ui->BntStop->setEnabled(false);
    pConvertor =  new McCadConversion();

}

ConvertorDlg::~ConvertorDlg()
{
    delete ui;
    connect_progress_main.disconnect();
    connect_progress_part.disconnect();
    connect_message_append.disconnect();
    connect_message_insert.disconnect();

    connect_main_label.disconnect();
    connect_part_label.disconnect();

    if(pConvertor)
    {
        pConvertor->terminate();
        pConvertor->wait();
    }

    delete pConvertor;
    pConvertor = NULL;
}

void ConvertorDlg::LoadData()
{
    ui->lineEdit_CellNum->clear();
    ui->lineEdit_CellNum->setText(QString::fromLatin1("Yesssss"));
}

void ConvertorDlg::StartConversion()
{
    ui->textEdit_Message->clear();
    //ui->textEdit_Message->verticalScrollBar()->setValue(200);
    
    ui->textEdit_Message->moveCursor(QTextCursor::End);
    
    //ui->textEdit_Message->verticalScrollBar()->setValue(ui->textEdit_Message->verticalScrollBar()->maximumHeight());

    McCadMessager_Signal *msg = McCadMessager_Signal::Instance();
    msg->sig_message_append(QString::fromLatin1("Start Conversion"));

    clock_t start, end;
    start = clock();
    double runtime; // Time cost of the conversion    

    if(!pConvertor->isRunning())
    {
        ui->BntStart->setEnabled(false);
        ui->BntStop->setEnabled(true);
        pConvertor->run();
    }
    else
    {
        return;
    }

    ui->BntStart->setEnabled(true);
    ui->BntStop->setEnabled(false);

    end = clock();
    runtime = (double)(end-start)/CLOCKS_PER_SEC;    
    msg->sig_message_append(QString::fromLatin1("Run   %1  seconds").arg(runtime));
}

void ConvertorDlg::StopConversion()
{
    pConvertor->terminate();
    pConvertor->wait();
}

void ConvertorDlg::SetProgressPart(const int &i)
{
    ui->progressBar_Part->setValue(i);
}

void ConvertorDlg::AppendMessage(const QString &str)
{
    ui->textEdit_Message->append(str);
}

void ConvertorDlg::InsertMessage(const QString &str)
{
    ui->textEdit_Message->moveCursor(QTextCursor::End);
    ui->textEdit_Message->insertPlainText(str);
}

void ConvertorDlg::SetProgressMain(const int &i)
{    
    ui->progressBar_Main->setValue(i);
}


void ConvertorDlg::SetMainLabel(const QString &str)
{
    ui->label_PrcMain->setText(str);
}

void ConvertorDlg::SetPartLabel(const QString &str)
{
    ui->label_PrcPart->setText(str);
}




#include "moc_ConvertorDlg.cpp"
