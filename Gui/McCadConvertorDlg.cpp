#include "PreCompiled.h"

#include <iostream>
#include <qscrollbar.h>
#include <qtextcursor.h>

#include "McCadConvertorDlg.h"
#include "ui_Convertor.h"

#include <App/Document.h>
#include <Gui/Document.h>
#include <Gui/WorkbenchManager.h>
#include <Gui/BitmapFactory.h>
#include <qdir.h>

#include "../App/McCadMessager_Signal.h"
#include "Workbench.h"
#include <qprocess.h>

using namespace std;
using namespace McCadGui;

McCadConvertorDlg::McCadConvertorDlg(const QString opt, QWidget *parent):
    QDialog(parent),
    ui(new Ui::McCadConvertorDlg)
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
    connect_progress_part  = msg->sig_progress_part.connect(boost::bind(&McCadConvertorDlg::SetProgressPart,this,_1));
    connect_progress_main = msg->sig_progress_main.connect(boost::bind(&McCadConvertorDlg::SetProgressMain,this,_1));
    connect_message_append = msg->sig_message_append.connect(boost::bind(&McCadConvertorDlg::AppendMessage,this,_1));
    connect_message_insert = msg->sig_message_insert.connect(boost::bind(&McCadConvertorDlg::InsertMessage,this,_1));

    connect_part_label = msg->sig_part_label.connect(boost::bind(&McCadConvertorDlg::SetPartLabel,this,_1));
    connect_main_label = msg->sig_main_label.connect(boost::bind(&McCadConvertorDlg::SetMainLabel,this,_1));

    m_strCodeOption = opt;

    ui->BntStop->setEnabled(false);
    pConvertor =  new McCadConversion();

    LoadPrmt();
}

McCadConvertorDlg::~McCadConvertorDlg()
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
        //pConvertor->terminate();
        //pConvertor->wait();
        delete pConvertor;
        pConvertor = NULL;
    }
}


void McCadConvertorDlg::closeEvent(QCloseEvent * event)
{
    pConvertor->stop();
    pConvertor->wait();
    event->accept();
}

void McCadConvertorDlg::LoadPrmt()
{
    /** Get the material manager of current document */
    McCadGui::Workbench *wb = (McCadGui::Workbench *)Gui::WorkbenchManager::instance()->active();
    App::Document *doc = App::GetApplication().getActiveDocument();
    string doc_name = doc->getName();
    McCadData *m_Data = wb->GetDataManager()->getData(doc_name);
    m_strWorkingDir = m_Data->GetWorkingDir();

    if(!CheckFiles())
    {
        ui->BntStart->setEnabled(false);
        ui->BntStop->setEnabled(false);
        ui->lineEdit_CellNum->setEnabled(false);
        ui->lineEdit_SurfNum->setEnabled(false);
        ui->comboBox_Precision->setEnabled(false);
        ui->checkBox_Void->setEnabled(false);
        ui->textEdit_Message->setEnabled(false);
        ui->progressBar_Main->setEnabled(false);
        ui->progressBar_Part->setEnabled(false);
        return;
    }
    else
    {
        m_strStepFile = m_strWorkingDir + QString::fromLatin1("/ConvertedModel.stp");
        m_strConfigFile = m_strWorkingDir + QString::fromLatin1("/McCadConfig.txt");
        m_strMatFile = m_strWorkingDir + QString::fromLatin1("/Materials.xml");
        //m_strCodeOption = QString::fromLatin1("-m");
    }

    pConvertor->Setting(m_strStepFile,m_strConfigFile,m_strMatFile,m_strCodeOption);

    ui->lineEdit_CellNum->clear();
    ui->lineEdit_SurfNum->clear();
    ui->checkBox_Void->setChecked(true);

    QStringList items;
    items<<QString::fromLatin1("High Precsion");
    items<<QString::fromLatin1("Middle Precsion");
    items<<QString::fromLatin1("Low Precsion");
    ui->comboBox_Precision->addItems(items);

    //cout<<"WorkingDirectory"<<m_strWorkingDir.toStdString()<<endl;

    if(m_strWorkingDir.isEmpty())
    {
        QString strInfo = QString::fromAscii("Please set working directory!\n");
        ui->textEdit_Message->insertPlainText(strInfo);
        return;
    }
    else
    {
        QFile file(m_strConfigFile);
        file.open(QIODevice::ReadOnly);
        QTextStream input(&file);
        QString line;
        do{
            line = input.readLine();
            line.trimmed();
            if(line.isEmpty() || line.left(1) == tr("#")){
                continue;
            }

            QString prmt = tr("");

            if(line.contains(tr("InitSurfNb"),Qt::CaseSensitive)){
                prmt = SplitString(line);
                ui->lineEdit_SurfNum->setText(prmt);
            }
            else if(line.contains(tr("InitCellNb"),Qt::CaseSensitive)){
                prmt = SplitString(line);
                ui->lineEdit_CellNum->setText(prmt);
            }
            else if(line.contains(tr("VoidGenerate"),Qt::CaseSensitive)){
                prmt = SplitString(line);
                ui->checkBox_Void->setChecked(prmt == tr("Yes") ? true : false );
            }
            else if(line.contains(tr("MinimumNumberOfSamplePoints"),Qt::CaseSensitive))
            {
                prmt = SplitString(line);
                if(!prmt.compare(tr("50")))
                {
                    ui->comboBox_Precision->setCurrentIndex(0);
                }
                else if(!prmt.compare(tr("20")))
                {
                    ui->comboBox_Precision->setCurrentIndex(1);
                }
                else if(!prmt.compare(tr("10")))
                {
                    ui->comboBox_Precision->setCurrentIndex(2);
                }
            }
        }while(!line.isNull());

        file.close();
    }
}


bool McCadConvertorDlg::CheckFiles()
{
    if(m_strWorkingDir.isEmpty())
    {
        return false;
    }
    else
    {
        QDir dir(m_strWorkingDir);
        QString fnConfig = QString::fromAscii("McCadConfig.txt");      

        QStringList listConfig = dir.entryList(QStringList(fnConfig), QDir::Files, QDir::Name);
        if (!listConfig.isEmpty())
        {
            return true;
        }
        else
        {
            return false;
        }       
    }
}


void McCadConvertorDlg::SavePrmt()
{
    QString strInitSurfNum = ui->lineEdit_SurfNum->text();
    QString strInitCellNum = ui->lineEdit_CellNum->text();

    QString strPrecsion = ui->comboBox_Precision->currentText();
    bool bVoid = ui->checkBox_Void->isChecked();

    int iMinSamplePoint = 50;
    int iMaxSamplePoint = 100;

    if (!strPrecsion.compare(tr("High Precsion"))){
        iMinSamplePoint = 50;
        iMaxSamplePoint = 100;
    }
    else if (!strPrecsion.compare(tr("Middle Precsion"))){
        iMinSamplePoint = 20;
        iMaxSamplePoint = 50;
    }
    else if (!strPrecsion.compare(tr("Low Precsion"))){
        iMinSamplePoint = 10;
        iMaxSamplePoint = 20;
    }

    QString strVoid = bVoid ? tr("Yes"):tr("No");

    QString strConfigFile = m_strWorkingDir+QLatin1String("/McCadConfig.txt");
    QString strBackupFile = m_strWorkingDir+QLatin1String("/ConfigBackUp.txt");

    QFile file_backup(strBackupFile);
    file_backup.open(QIODevice::ReadWrite);
    QTextStream output(&file_backup);

    QFile file(strConfigFile);
    file.open(QIODevice::ReadWrite);
    QTextStream input(&file);
    QString line;
    do{
        line = input.readLine();
        line.trimmed();
        if(line.isEmpty()){
            continue;
        }

        QString strNewPmt = tr("");

        if(line.contains(tr("InitSurfNb"),Qt::CaseSensitive)){
            strNewPmt = tr("InitSurfNb               ") + strInitSurfNum;
        }
        else if(line.contains(tr("InitCellNb"),Qt::CaseSensitive)){
            strNewPmt = tr("InitCellNb               ") + strInitCellNum;
        }
        else if(line.contains(tr("VoidGenerate"),Qt::CaseSensitive)){
            strNewPmt = tr("VoidGenerate            ") + strVoid;
        }
        else if(line.contains(tr("MinimumNumberOfSamplePoints"),Qt::CaseSensitive))
        {
            strNewPmt = tr("MinimumNumberOfSamplePoints          ") + QString::number(iMinSamplePoint);
        }
        else if(line.contains(tr("MaximumNumberOfSamplePoints"),Qt::CaseSensitive))
        {
            strNewPmt = tr("MaximumNumberOfSamplePoints          ") + QString::number(iMaxSamplePoint);
        }
        else
        {
            strNewPmt = line;
        }

        output<<strNewPmt<<endl;

    }while(!line.isNull());

    file.close();
    file_backup.close();

    bool bDel = file.remove();
    if(bDel){
        file_backup.rename(strConfigFile);
    }
}


QString McCadConvertorDlg::SplitString(QString &strLine)
{
    strLine.simplified();                                                           // Remove the '/t'
    strLine.replace(QRegExp(QString::fromAscii("[\\s]+")),QString::fromAscii(" ")); // Remove the empty spaces
    QStringList strList = strLine.split(QString::fromAscii(" "));                   // Split the string with space

    QString prmt = tr("");
    if(strList.length() > 1)
    {
        prmt = strList.at(1);
        prmt.trimmed();
        return prmt;
    }
    else
    {
        return prmt;
    }
}



void McCadConvertorDlg::StartConversion()
{
    SavePrmt();

    ui->textEdit_Message->clear();
    ui->textEdit_Message->moveCursor(QTextCursor::End);

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

    OpenOutputFile();
}


void McCadConvertorDlg::OpenOutputFile()
{
    QDir dir(m_strWorkingDir);
    QString fnConfig;
    if (pConvertor->getCodeOpt().IsEqual("-m"))
    {
        fnConfig = QString::fromAscii("ConvertedModel_MCNP.txt");
    }
    else if (pConvertor->getCodeOpt().IsEqual("-t"))
    {
        fnConfig = QString::fromAscii("ConvertedModel_Tripoli.txt");
    }
    else if (pConvertor->getCodeOpt().IsEqual("-s"))
    {
        fnConfig = QString::fromAscii("ConvertedModel_Serpent.txt");
    }

    QStringList listConfig = dir.entryList(QStringList(fnConfig), QDir::Files, QDir::Name);
    if (!listConfig.isEmpty())
    {        
        QProcess *proc = new QProcess();
        QString path = m_strWorkingDir + tr("/");
        path = path + fnConfig;        
        proc->start(QString::fromLatin1("gedit"), QStringList()<<path);
        proc->waitForBytesWritten();
        proc->waitForFinished();
    }
}


void McCadConvertorDlg::StopConversion()
{
    if(pConvertor->isRunning())
    {
        pConvertor->stop();

        //pConvertor->terminate();
        //pConvertor->wait();
    }



}

void McCadConvertorDlg::SetProgressPart(const int &i)
{
    ui->progressBar_Part->setValue(i);
}

void McCadConvertorDlg::AppendMessage(const QString &str)
{
    ui->textEdit_Message->append(str);
}

void McCadConvertorDlg::InsertMessage(const QString &str)
{
    ui->textEdit_Message->moveCursor(QTextCursor::End);
    ui->textEdit_Message->insertPlainText(str);
}

void McCadConvertorDlg::SetProgressMain(const int &i)
{
    ui->progressBar_Main->setValue(i);
}


void McCadConvertorDlg::SetMainLabel(const QString &str)
{
    ui->label_PrcMain->setText(str);
}

void McCadConvertorDlg::SetPartLabel(const QString &str)
{
    ui->label_PrcPart->setText(str);
}

#include "moc_McCadConvertorDlg.cpp"
