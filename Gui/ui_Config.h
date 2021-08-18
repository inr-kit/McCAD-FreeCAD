/********************************************************************************
** Form generated from reading UI file 'Config.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeView>

QT_BEGIN_NAMESPACE

class Ui_ParameterDlg
{
public:
    QDialogButtonBox *buttonBox;
    QTreeView *treeView;

    void setupUi(QDialog *ParameterDlg)
    {
        if (ParameterDlg->objectName().isEmpty())
            ParameterDlg->setObjectName(QString::fromUtf8("ParameterDlg"));
        ParameterDlg->resize(584, 541);
        buttonBox = new QDialogButtonBox(ParameterDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(230, 500, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        treeView = new QTreeView(ParameterDlg);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setGeometry(QRect(10, 10, 561, 481));

        retranslateUi(ParameterDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), ParameterDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ParameterDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(ParameterDlg);
    } // setupUi

    void retranslateUi(QDialog *ParameterDlg)
    {
        ParameterDlg->setWindowTitle(QApplication::translate("ParameterDlg", "Parameter Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ConfigDlg: public Ui_ParameterDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIG_H
