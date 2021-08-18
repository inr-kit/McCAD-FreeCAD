#include "PreCompiled.h"
#include "WorkingSpaceDlg.h"
#include "ui_WorkingSpaceDlg.h"

#include <Gui/WorkbenchManager.h>

#include <App/Document.h>
#include <Gui/Document.h>
#include <qfiledialog.h>

#include "Workbench.h"
#include "ConfigDlg.h"

#include "../App/CreateBox.h"

using namespace std;
using namespace McCadGui;

WorkingSpaceDlg::WorkingSpaceDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkingSpaceDlg)
{
    ui->setupUi(this);
    QObject::connect(ui->FolderButton,SIGNAL(clicked()),this,SLOT(SelFileFolder()));
    QObject::connect(ui->FolderButton_2,SIGNAL(clicked()),this,SLOT(ConfigFile()));   
}

WorkingSpaceDlg::~WorkingSpaceDlg()
{
    delete ui;
}

void WorkingSpaceDlg::SelFileFolder()
{
    QString strDir;

    m_strWorkingDir = QFileDialog::getExistingDirectory(this, tr("Select the Working Directory"), strDir);
    if (!m_strWorkingDir.isEmpty())
    {
        m_strWorkingDir = QDir::fromNativeSeparators(m_strWorkingDir);
        ui->plainTextEdit->clear();
        ui->plainTextEdit->insertPlainText(QString::fromAscii("Working Directory:") + m_strWorkingDir);
        CheckFiles();
    }

    //McCadGui::Workbench *wb = (McCadGui::Workbench *)Gui::WorkbenchManager::instance()->active();
    //App::Document *doc = App::GetApplication().getActiveDocument();


    /** Get the material manager of current document */
    McCadGui::Workbench *wb = (McCadGui::Workbench *)Gui::WorkbenchManager::instance()->active();
    App::Document *doc = App::GetApplication().getActiveDocument();
    string doc_name = doc->getName();
    m_Data = wb->GetDataManager()->getData(doc_name);

    m_Data->SetWorkingDir(m_strWorkingDir);

    //string str = doc->getName();
    //QString docName = QString::fromUtf8(doc->getName());
    //QString sss = wb->GetObserver()->GetData(str);
    //ui->plainTextEdit->insertPlainText(sss);
}


QString WorkingSpaceDlg::GetWorkingDir()
{
    return m_strWorkingDir;
}

void WorkingSpaceDlg::CheckFiles()
{
    QDir dir(m_strWorkingDir);

    QString fnGroupMat = QString::fromAscii("Materials.xml");
    QString fnConfig = QString::fromAscii("McCad.Config");

    QString strInfo;

    bool bHasGroupMatFile = false;
    bool bHasConfigFile = false;

    QStringList listGroupMat = dir.entryList(QStringList(fnGroupMat), QDir::Files, QDir::Name);
    if (!listGroupMat.isEmpty())
    {
        bHasGroupMatFile = true;
        strInfo += QString::fromAscii("Material File: Materials.xml................Yes\n");
    }

    QStringList listConfig = dir.entryList(QStringList(fnConfig), QDir::Files, QDir::Name);
    if (!listConfig.isEmpty())
    {
        bHasConfigFile = true;
        strInfo += QString::fromAscii("Configuration File: McCad.Config................Yes\n");
    }
    //CreateBox pBox;
    //pBox.TestCombiView();
    //QString s = pBox.m_ss;
    //ui->plainTextEdit->insertPlainText(s);
    //strInfo += "...";
    ui->plainTextEdit->insertPlainText(strInfo);
}

void WorkingSpaceDlg::ConfigFile()
{
    ConfigDlg *pDlg = new ConfigDlg();
    pDlg->setModal(true);
    pDlg->show();

//    CreateBox pBox;
//    pBox.TestCombiView();
//    QString s = pBox.m_ss;
//    ui->plainTextEdit->insertPlainText(s);

}

#include "moc_WorkingSpaceDlg.cpp"
