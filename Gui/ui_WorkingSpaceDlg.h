/********************************************************************************
** Form generated from reading UI file 'WorkingSpace.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORKINGSPACEDLG_H
#define UI_WORKINGSPACEDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_WorkSettingDlg
{
public:
    QDialogButtonBox *buttonBox;
    QToolButton *FolderButton;
    QToolButton *FolderButton_2;
    QToolButton *FolderButton_3;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QDialog *SettingDlg)
    {
        if (SettingDlg->objectName().isEmpty())
            SettingDlg->setObjectName(QString::fromUtf8("SettingDlg"));
        SettingDlg->resize(502, 371);
        QFont font;
        font.setPointSize(12);
        SettingDlg->setFont(font);
        buttonBox = new QDialogButtonBox(SettingDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(150, 330, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        FolderButton = new QToolButton(SettingDlg);
        FolderButton->setObjectName(QString::fromUtf8("FolderButton"));
        FolderButton->setGeometry(QRect(10, 10, 70, 70));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/folder.svg"), QSize(), QIcon::Normal, QIcon::Off);
        FolderButton->setIcon(icon);
        FolderButton->setCheckable(false);
        FolderButton_2 = new QToolButton(SettingDlg);
        FolderButton_2->setObjectName(QString::fromUtf8("FolderButton_2"));
        FolderButton_2->setGeometry(QRect(170, 10, 70, 70));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/structure.svg"), QSize(), QIcon::Normal, QIcon::Off);
        FolderButton_2->setIcon(icon1);
        FolderButton_3 = new QToolButton(SettingDlg);
        FolderButton_3->setObjectName(QString::fromUtf8("FolderButton_3"));
        FolderButton_3->setGeometry(QRect(90, 10, 70, 70));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/setting.svg"), QSize(), QIcon::Normal, QIcon::Off);
        FolderButton_3->setIcon(icon2);
        plainTextEdit = new QPlainTextEdit(SettingDlg);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 90, 481, 231));
        plainTextEdit->setAutoFillBackground(false);
        plainTextEdit->setReadOnly(true);

        retranslateUi(SettingDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), SettingDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SettingDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(SettingDlg);
    } // setupUi

    void retranslateUi(QDialog *SettingDlg)
    {
        SettingDlg->setWindowTitle(QApplication::translate("SettingDlg", "Setting", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        SettingDlg->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        FolderButton->setToolTip(QApplication::translate("SettingDlg", "Set Working Directory", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        FolderButton->setWhatsThis(QApplication::translate("SettingDlg", "Set", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        FolderButton->setText(QApplication::translate("SettingDlg", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        FolderButton_2->setToolTip(QApplication::translate("SettingDlg", "Set Group & Material", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        FolderButton_2->setText(QApplication::translate("SettingDlg", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        FolderButton_3->setToolTip(QApplication::translate("SettingDlg", "Set Configuration", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        FolderButton_3->setText(QApplication::translate("SettingDlg", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WorkingSpaceDlg: public Ui_WorkSettingDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORKINGSPACEDLG_H
