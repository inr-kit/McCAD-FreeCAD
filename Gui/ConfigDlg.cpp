#include "ConfigDlg.h"
#include "ui_Config.h"
#include <qstandarditemmodel.h>

#include <fstream>
#include <iostream>

ConfigDlg::ConfigDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDlg)
{
    ui->setupUi(this);
    LoadData();
}

ConfigDlg::~ConfigDlg()
{
    delete ui;
}


void ConfigDlg::LoadData()
{
    m_strFileName = QLatin1String("/home/mccad/Documents/McCadConfig.txt");
    ReadConfigFile(m_strFileName);

    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels(QStringList()<<QString("Parameters")<<("Data"));

    list<pair<QString,QString>>::iterator iter;
    for(iter = m_listPrmt.begin(); iter!= m_listPrmt.end(); ++iter)
    {
        QString pmrt = iter->first;
        QString data = iter->second;

        QStandardItem * itemPrmt = new QStandardItem(pmrt);
        itemPrmt->setEditable(false);
        QStandardItem * itemData = new QStandardItem(data);

        model->appendRow(itemPrmt);
        model->setItem(itemPrmt->row(),1,itemData);
    }

    ui->treeView->setModel(model);
    ui->treeView->setColumnWidth(0,400);
}

void ConfigDlg::ReadConfigFile(const QString InputFileName)
{
    // open parameter file
    std::ifstream inFile(InputFileName.toStdString());
    if (!inFile)
    {
        return;
    }

    // read file line by line
    while(inFile)
    {
        char str[255];
        inFile.getline(str,255);
        QString strCmd= QString(QLatin1String(str));
        strCmd.trimmed();

        if(strCmd.isEmpty())
            continue;

        if(strCmd.left(1) == QString(QLatin1String("#")))
            continue;

        strCmd.simplified();                        // Remove the '/t'
        strCmd.replace(QRegExp("[\\s]+")," ");      // Remove the empty spaces

        QStringList strList = strCmd.split(" ");    // Split the string with space

        if(strList.length() > 1)
        {
            QString prmt = strList.at(0);
            QString numb = strList.at(1);

            prmt.trimmed();
            numb.trimmed();

            m_listPrmt.push_back(make_pair(prmt,numb));
        }
    }
}

#include "moc_ConfigDlg.cpp"
