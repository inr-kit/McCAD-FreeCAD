/********************************************************************************
** Form generated from reading UI file 'WorkingSpaceh18101.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef WORKINGSPACEH18101_H
#define WORKINGSPACEH18101_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QTextEdit>
#include <QtGui/QToolButton>
#include <QtGui/QTreeView>

QT_BEGIN_NAMESPACE

class Ui_SettingDlg
{
public:
    QDialogButtonBox *buttonBox;
    QToolButton *BtnSetting;
    QToolButton *BtnFilePath;
    QFrame *line;
    QTreeView *treeViewConfig;
    QTextEdit *textEditFiles;
    QToolButton *BtnDefault;

    void setupUi(QDialog *SettingDlg)
    {
        if (SettingDlg->objectName().isEmpty())
            SettingDlg->setObjectName(QString::fromUtf8("SettingDlg"));
        SettingDlg->resize(633, 692);
        QFont font;
        font.setPointSize(12);
        SettingDlg->setFont(font);
        buttonBox = new QDialogButtonBox(SettingDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(280, 650, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        BtnSetting = new QToolButton(SettingDlg);
        BtnSetting->setObjectName(QString::fromUtf8("BtnSetting"));
        BtnSetting->setEnabled(true);
        BtnSetting->setGeometry(QRect(10, 160, 50, 50));
        BtnSetting->setMinimumSize(QSize(20, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/SettingFile.svg"), QSize(), QIcon::Normal, QIcon::Off);
        BtnSetting->setIcon(icon);
        BtnFilePath = new QToolButton(SettingDlg);
        BtnFilePath->setObjectName(QString::fromUtf8("BtnFilePath"));
        BtnFilePath->setGeometry(QRect(10, 10, 50, 50));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/folder.svg"), QSize(), QIcon::Normal, QIcon::Off);
        BtnFilePath->setIcon(icon1);
        BtnFilePath->setCheckable(false);
        line = new QFrame(SettingDlg);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(10, 140, 611, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        treeViewConfig = new QTreeView(SettingDlg);
        treeViewConfig->setObjectName(QString::fromUtf8("treeViewConfig"));
        treeViewConfig->setGeometry(QRect(70, 160, 551, 481));
        textEditFiles = new QTextEdit(SettingDlg);
        textEditFiles->setObjectName(QString::fromUtf8("textEditFiles"));
        textEditFiles->setGeometry(QRect(70, 10, 551, 131));
        QPalette palette;
        QBrush brush(QColor(232, 232, 231, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        textEditFiles->setPalette(palette);
        textEditFiles->setReadOnly(true);
        BtnDefault = new QToolButton(SettingDlg);
        BtnDefault->setObjectName(QString::fromUtf8("BtnDefault"));
        BtnDefault->setEnabled(true);
        BtnDefault->setGeometry(QRect(10, 220, 50, 50));
        BtnDefault->setMinimumSize(QSize(20, 20));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/Refresh.svg"), QSize(), QIcon::Normal, QIcon::Off);
        BtnDefault->setIcon(icon2);

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
        BtnSetting->setToolTip(QApplication::translate("SettingDlg", "Set Configuration", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        BtnSetting->setText(QApplication::translate("SettingDlg", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        BtnFilePath->setToolTip(QApplication::translate("SettingDlg", "Set Working Directory", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        BtnFilePath->setWhatsThis(QApplication::translate("SettingDlg", "Set", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        BtnFilePath->setText(QApplication::translate("SettingDlg", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        BtnDefault->setToolTip(QApplication::translate("SettingDlg", "Set Configuration", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        BtnDefault->setText(QApplication::translate("SettingDlg", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};


namespace Ui {
    class McCadSettingDlg: public Ui_SettingDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTING_H
