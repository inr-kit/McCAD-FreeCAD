#include "PreCompiled.h"

#include "McCadSettingDlg.h"
#include "ui_Setting.h"

#include <fstream>
#include <iostream>

#include <qfile.h>
#include <qiodevice.h>
#include <qmessagebox.h>

#include <Gui/WorkbenchManager.h>
#include <App/Document.h>
#include <Gui/Document.h>
#include <qfiledialog.h>

#include "Workbench.h"
#include "McCadComboDelegate.h"

using namespace std;
using namespace McCadGui;

McCadSettingDlg::McCadSettingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::McCadSettingDlg)
{
    ui->setupUi(this);
    QObject::connect(ui->BtnFilePath,SIGNAL(clicked()),this,SLOT(SelFileFolder()));
    QObject::connect(ui->BtnSetting,SIGNAL(clicked()),this,SLOT(CreateConfigFile()));
    QObject::connect(ui->BtnDefault,SIGNAL(clicked()),this,SLOT(SetDefaultConfig()));
    QObject::connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(SaveData()));

    m_pModel = new QStandardItemModel();

    LoadData();
}


McCadSettingDlg::~McCadSettingDlg()
{
    delete ui;
    if(m_pModel)
    {
        delete m_pModel;
        m_pModel = NULL;
    }
}


void McCadSettingDlg::SetDefaultPrmt()
{
    m_vecPrmt.clear();
    m_vecPrmt.push_back(make_pair(tr("InitCellNb"),tr("1")));
    m_vecPrmt.push_back(make_pair(tr("InitSurfNb"),tr("1")));
    m_vecPrmt.push_back(make_pair(tr("WriteCollisionFile"),tr("Yes")));    
    m_vecPrmt.push_back(make_pair(tr("InitialVoidBoxNumber"),tr("1")));
    m_vecPrmt.push_back(make_pair(tr("MinimumInputSolidVolume"),tr("1.00")));
    m_vecPrmt.push_back(make_pair(tr("MinimumVoidVolume"),tr("1.25e2")));
    m_vecPrmt.push_back(make_pair(tr("MinimumSizeOfDecompositionFaceArea"),tr("50")));
    m_vecPrmt.push_back(make_pair(tr("MaximumNumberOfComplementedCells"),tr("10")));
    m_vecPrmt.push_back(make_pair(tr("MaximumNumberOfPreDecompositionCells"),tr("100")));
    m_vecPrmt.push_back(make_pair(tr("MinimumSizeOfRedecompositionFaceArea"),tr("2")));
    m_vecPrmt.push_back(make_pair(tr("MinimumNumberOfSamplePoints"),tr("50")));
    m_vecPrmt.push_back(make_pair(tr("MaximumNumberOfSamplePoints"),tr("100")));
    m_vecPrmt.push_back(make_pair(tr("XResolution"),tr("0.001")));
    m_vecPrmt.push_back(make_pair(tr("YResolution"),tr("0.001")));
    m_vecPrmt.push_back(make_pair(tr("RResolution"),tr("0.00314")));
    m_vecPrmt.push_back(make_pair(tr("MaxDecomposeDepth"),tr("15")));    
    m_vecPrmt.push_back(make_pair(tr("MaxCellExpressionLength"),tr("500")));
    m_vecPrmt.push_back(make_pair(tr("Tolerance"),tr("1e-4")));
    m_vecPrmt.push_back(make_pair(tr("VoidGenerate"),tr("Yes")));
}


void McCadSettingDlg::SaveData()
{
    QFile file(m_strFileName); std::cout<<m_strFileName.toStdString()<<endl;
    if(file.open(QFile::ReadWrite|QIODevice::Text|QIODevice::Truncate))
    {
        QTextStream str(&file);

        str<<"#\n#      McCadInputFile.txt - EXAMPLE\n#"<<endl;
        str<<"# '#' introduces a comment line"<<endl;
        str<<"# The format of this file is 'keyword' 'Value' where value can be a"<<endl;
        str<<"# string, a real, or integer value. Order is irrelevant."<<endl;
        str<<"#"<<endl;
        str<<"# Do not put comments in keyword lines!!!"<<endl;

        str.setFieldAlignment(QTextStream::AlignLeft);
        for (int i=0; i<m_pModel->rowCount(); ++i)
        {
            QVariant prmt = m_pModel->data(m_pModel->index(i,0));
            QVariant data = m_pModel->data(m_pModel->index(i,1));

            QString strPrmt = prmt.toString();
            QString strData = data.toString();

            str<<qSetFieldWidth(40)<<strPrmt<<qSetFieldWidth(20)<<strData<<qSetFieldWidth(0)<<endl;
        }

        str<<"#"<<endl;
    }

    file.close();
}


void McCadSettingDlg::LoadData()
{
    /** Get the material manager of current document */
    McCadGui::Workbench *wb = (McCadGui::Workbench *)Gui::WorkbenchManager::instance()->active();
    App::Document *doc = App::GetApplication().getActiveDocument();
    string doc_name = doc->getName();
    m_Data = wb->GetDataManager()->getData(doc_name);
    m_strWorkingDir = m_Data->GetWorkingDir();

    if(m_strWorkingDir.isEmpty())
    {
        QString strInfo = QString::fromAscii("Please set working directory!\n");
        ui->textEditFiles->insertPlainText(strInfo);
        ui->BtnSetting->setEnabled(false);
        return;
    }
    else
    {
        m_strFileName = m_strWorkingDir+QLatin1String("/McCadConfig.txt");
        ReadConfigFile(m_strFileName);
        CheckFiles();
    }
}


void McCadSettingDlg::CreateConfigFile()
{
    /// Warning message box for creating the configuration file.
    QMessageBox::StandardButton bn = QMessageBox::warning(NULL,tr("Warning"),
                                                          tr("Create McCadConfig.txt in this file folder?"),
                                                          QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(bn == QMessageBox::No)
    {
        return;
    }

    SetDefaultPrmt();

    QFile file(m_strFileName);
    if(file.open(QFile::WriteOnly|QIODevice::Text))
    {
        QTextStream str(&file);

        str<<"#\n#      McCadInputFile.txt - EXAMPLE\n#"<<endl;
        str<<"# '#' introduces a comment line"<<endl;
        str<<"# The format of this file is 'keyword' 'Value' where value can be a"<<endl;
        str<<"# string, a real, or integer value. Order is irrelevant."<<endl;
        str<<"#"<<endl;
        str<<"# Do not put comments in keyword lines!!!"<<endl;
        str<<"#"<<endl;

        str.setFieldAlignment(QTextStream::AlignLeft);

        vector<pair<QString,QString>>::iterator iter;
        for(iter = m_vecPrmt.begin(); iter!= m_vecPrmt.end(); ++iter)
        {
            QString pmrt = iter->first;
            QString data = iter->second;

            str<<qSetFieldWidth(40)<<pmrt<<qSetFieldWidth(20)<<data<<qSetFieldWidth(0)<<endl;
        }

        str<<"# "<<endl;
    }
    file.close();

    ui->BtnSetting->setEnabled(false);
    CheckFiles();    
}


void McCadSettingDlg::SetDefaultConfig()
{
    SetDefaultPrmt();
    UpdateTable();
}


void McCadSettingDlg::ReadConfigFile(const QString InputFileName)
{
    // open parameter file
    std::ifstream inFile(InputFileName.toStdString());
    if (!inFile)
    {
        return;
    }

    m_vecPrmt.clear();

    // read file line by line
    while(inFile)
    {
        char str[255];
        inFile.getline(str,255);
        QString strCmd= QString(QLatin1String(str));
        strCmd.trimmed();

        if(strCmd.isEmpty() || strCmd.left(1) == QString(QLatin1String("#")))
            continue;

        strCmd.simplified();                        // Remove the '/t'
        strCmd.replace(QRegExp(QString::fromAscii("[\\s]+")),QString::fromAscii(" "));      // Remove the empty spaces
        QStringList strList = strCmd.split(QString::fromAscii(" ")); // Split the string with space

        if(strList.length() > 1)
        {
            QString prmt = strList.at(0);
            QString numb = strList.at(1);

            prmt.trimmed();
            numb.trimmed();

            m_vecPrmt.push_back(make_pair(prmt,numb));
        }
    }

    UpdateTable();
}


void McCadSettingDlg::UpdateTable()
{
    m_pModel->clear();
    m_pModel->setHorizontalHeaderLabels(QStringList()<<QString::fromLatin1("Parameters")<<QString::fromLatin1("Data"));

    vector<pair<QString,QString>>::iterator iter;
    int iRow = 0;
    for(iter = m_vecPrmt.begin(); iter!= m_vecPrmt.end(); ++iter)
    {
        QString pmrt = iter->first;
        QString data = iter->second;

        QStandardItem * itemPrmt = new QStandardItem(pmrt);
        itemPrmt->setEditable(false);
        QStandardItem * itemData = new QStandardItem(data);

        m_pModel->appendRow(itemPrmt);
        m_pModel->setItem(itemPrmt->row(),1,itemData);

        if(pmrt.compare(tr("VoidGenerate")) == 0 || pmrt.compare(tr("WriteCollisionFile")) == 0)
        {
            ComboDelegate* delegate = new ComboDelegate();
            ui->treeViewConfig->setItemDelegateForRow(iRow, delegate);
        }
        iRow++;
    }

    ui->treeViewConfig->setModel(m_pModel);
    ui->treeViewConfig->setColumnWidth(0,400);
}


void McCadSettingDlg::SelFileFolder()
{
    QString strDir;

    m_strWorkingDir = QFileDialog::getExistingDirectory(this, tr("Select the Working Directory"), strDir);
    if (!m_strWorkingDir.isEmpty())
    {
        m_strWorkingDir = QDir::fromNativeSeparators(m_strWorkingDir);
        CheckFiles();

        /** Get the material manager of current document */
        McCadGui::Workbench *wb = (McCadGui::Workbench *)Gui::WorkbenchManager::instance()->active();
        App::Document *doc = App::GetApplication().getActiveDocument();
        string doc_name = doc->getName();
        m_Data = wb->GetDataManager()->getData(doc_name);

        m_Data->SetWorkingDir(m_strWorkingDir); // Set the working directory.
        m_strFileName = m_strWorkingDir+QLatin1String("/McCadConfig.txt");        
    }
    else
    {
        return;
    }    
}


QString McCadSettingDlg::GetWorkingDir()
{
    return m_strWorkingDir;
}

void McCadSettingDlg::CheckFiles()
{
    QDir dir(m_strWorkingDir);

    ui->textEditFiles->clear();
    ui->textEditFiles->insertPlainText(QString::fromAscii("Working Directory: ") + m_strWorkingDir + QString::fromAscii("\n\n"));

    QString fnGroupMat = QString::fromAscii("Materials.xml");
    QString fnConfig = QString::fromAscii("McCadConfig.txt");

    QString strInfo;

    bool bHasConfigFile = false;

    QStringList listGroupMat = dir.entryList(QStringList(fnGroupMat), QDir::Files, QDir::Name);
    if (!listGroupMat.isEmpty())
    {       
        strInfo += QString::fromAscii("Material File: Material.xml..........Yes\n");        
    }
    else
    {        
        strInfo += QString::fromAscii("Material File: Material.xml..........No\n");
    }

    QStringList listConfig = dir.entryList(QStringList(fnConfig), QDir::Files, QDir::Name);
    if (!listConfig.isEmpty())
    {
        bHasConfigFile = true;      
        strInfo += QString::fromAscii("Configuration File: McCadConfig.txt..........Yes\n");
    }
    else
    {
        bHasConfigFile = false;        
        strInfo += QString::fromAscii("Configuration File: McCadConfig.txt..........No\n");
    }

    ui->textEditFiles->insertPlainText(strInfo);

    if(bHasConfigFile)
    {
        QString strConfigFile = m_strWorkingDir+QLatin1String("/McCadConfig.txt");
        ReadConfigFile(strConfigFile);
        ui->BtnSetting->setEnabled(false);
    }
    else
    {
        ui->BtnSetting->setEnabled(true);
        ui->treeViewConfig->setModel(nullptr);
    }
}

#include "moc_McCadSettingDlg.cpp"
