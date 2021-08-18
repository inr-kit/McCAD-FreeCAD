/********************************************************************************
** Form generated from reading UI file 'MatEditordT1814.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MATEDITORDT1814_H
#define MATEDITORDT1814_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QToolButton>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtGui/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_MatDlg
{
public:
    QLabel *labelLink;
    QDialogButtonBox *buttonBox;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *HLayoutIDDensity;
    QLabel *labelID;
    QLineEdit *lineEditID;
    QLabel *labelDensity;
    QLineEdit *lineEditDensity;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *VLayoutComp;
    QLabel *labelComp;
    QPlainTextEdit *TextEditCmp;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *HLayoutMatBnt;
    QPushButton *BnRvMatList;
    QSpacerItem *horizontalSpacer;
    QPushButton *BnAddMatList;
    QPushButton *BnAddMatDb;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *HLayoutMatName;
    QLabel *labelMatName;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEditMatName;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *HLayoutDatabase;
    QLabel *labelDatabase;
    QComboBox *comboBoxMatDb;
    QSplitter *splitterGroupMat;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *VLayoutGrouplist;
    QHBoxLayout *HLayoutFiles;
    QToolButton *BnLoad;
    QToolButton *toolButton;
    QTreeView *GroupTree;
    QGridLayout *GLayoutGroup;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *VLayoutMatlist;
    QSpacerItem *verticalSpacer;
    QLabel *labelMat;
    QTreeWidget *MatTree;
    QToolButton *BnConnect;
    QToolButton *BnAuto;

    void setupUi(QDialog *MatDlg)
    {
        if (MatDlg->objectName().isEmpty())
            MatDlg->setObjectName(QString::fromUtf8("MatDlg"));
        MatDlg->setEnabled(true);
        MatDlg->resize(994, 713);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MatDlg->sizePolicy().hasHeightForWidth());
        MatDlg->setSizePolicy(sizePolicy);
        MatDlg->setMinimumSize(QSize(20, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/atoms.svg"), QSize(), QIcon::Normal, QIcon::Off);
        MatDlg->setWindowIcon(icon);
        MatDlg->setSizeGripEnabled(false);
        labelLink = new QLabel(MatDlg);
        labelLink->setObjectName(QString::fromUtf8("labelLink"));
        labelLink->setGeometry(QRect(780, 630, 195, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(true);
        font.setWeight(50);
        labelLink->setFont(font);
        //labelLink->setTextFormat(Qt::PlainText);
        labelLink->setOpenExternalLinks(true);
        labelLink->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
        buttonBox = new QDialogButtonBox(MatDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(800, 670, 176, 28));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        buttonBox->setFont(font1);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        horizontalLayoutWidget_2 = new QWidget(MatDlg);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(430, 50, 461, 35));
        HLayoutIDDensity = new QHBoxLayout(horizontalLayoutWidget_2);
        HLayoutIDDensity->setObjectName(QString::fromUtf8("HLayoutIDDensity"));
        HLayoutIDDensity->setContentsMargins(0, 0, 0, 0);
        labelID = new QLabel(horizontalLayoutWidget_2);
        labelID->setObjectName(QString::fromUtf8("labelID"));
        labelID->setFont(font1);
        labelID->setTextFormat(Qt::AutoText);

        HLayoutIDDensity->addWidget(labelID);

        lineEditID = new QLineEdit(horizontalLayoutWidget_2);
        lineEditID->setObjectName(QString::fromUtf8("lineEditID"));

        HLayoutIDDensity->addWidget(lineEditID);

        labelDensity = new QLabel(horizontalLayoutWidget_2);
        labelDensity->setObjectName(QString::fromUtf8("labelDensity"));
        labelDensity->setFont(font1);
        labelDensity->setTextFormat(Qt::AutoText);

        HLayoutIDDensity->addWidget(labelDensity);

        lineEditDensity = new QLineEdit(horizontalLayoutWidget_2);
        lineEditDensity->setObjectName(QString::fromUtf8("lineEditDensity"));
        QFont font2;
        font2.setPointSize(12);
        lineEditDensity->setFont(font2);

        HLayoutIDDensity->addWidget(lineEditDensity);

        verticalLayoutWidget_3 = new QWidget(MatDlg);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(430, 180, 551, 401));
        VLayoutComp = new QVBoxLayout(verticalLayoutWidget_3);
        VLayoutComp->setObjectName(QString::fromUtf8("VLayoutComp"));
        VLayoutComp->setContentsMargins(0, 0, 0, 0);
        labelComp = new QLabel(verticalLayoutWidget_3);
        labelComp->setObjectName(QString::fromUtf8("labelComp"));
        labelComp->setFont(font1);
        labelComp->setTextFormat(Qt::AutoText);

        VLayoutComp->addWidget(labelComp);

        TextEditCmp = new QPlainTextEdit(verticalLayoutWidget_3);
        TextEditCmp->setObjectName(QString::fromUtf8("TextEditCmp"));
        TextEditCmp->setFont(font1);

        VLayoutComp->addWidget(TextEditCmp);

        horizontalLayoutWidget_5 = new QWidget(MatDlg);
        horizontalLayoutWidget_5->setObjectName(QString::fromUtf8("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(430, 590, 551, 31));
        HLayoutMatBnt = new QHBoxLayout(horizontalLayoutWidget_5);
        HLayoutMatBnt->setObjectName(QString::fromUtf8("HLayoutMatBnt"));
        HLayoutMatBnt->setContentsMargins(0, 0, 0, 0);

        BnAddMatDb = new QPushButton(horizontalLayoutWidget_5);
        BnAddMatDb->setObjectName(QString::fromUtf8("BnAddMatDb"));
        BnAddMatDb->setFont(font1);

        HLayoutMatBnt->addWidget(BnAddMatDb);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        HLayoutMatBnt->addItem(horizontalSpacer);

        BnAddMatList = new QPushButton(horizontalLayoutWidget_5);
        BnAddMatList->setObjectName(QString::fromUtf8("BnAddMatList"));
        BnAddMatList->setFont(font1);

        HLayoutMatBnt->addWidget(BnAddMatList);

        BnRvMatList = new QPushButton(horizontalLayoutWidget_5);
        BnRvMatList->setObjectName(QString::fromUtf8("BnRvMatList"));
        BnRvMatList->setFont(font1);

        HLayoutMatBnt->addWidget(BnRvMatList);



        horizontalLayoutWidget_3 = new QWidget(MatDlg);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(430, 90, 461, 35));
        HLayoutMatName = new QHBoxLayout(horizontalLayoutWidget_3);
        HLayoutMatName->setObjectName(QString::fromUtf8("HLayoutMatName"));
        HLayoutMatName->setContentsMargins(0, 0, 0, 0);
        labelMatName = new QLabel(horizontalLayoutWidget_3);
        labelMatName->setObjectName(QString::fromUtf8("labelMatName"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(labelMatName->sizePolicy().hasHeightForWidth());
        labelMatName->setSizePolicy(sizePolicy1);
        labelMatName->setFont(font1);
        labelMatName->setTextFormat(Qt::AutoText);

        HLayoutMatName->addWidget(labelMatName);

        horizontalSpacer_2 = new QSpacerItem(54, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        HLayoutMatName->addItem(horizontalSpacer_2);

        lineEditMatName = new QLineEdit(horizontalLayoutWidget_3);
        lineEditMatName->setObjectName(QString::fromUtf8("lineEditMatName"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lineEditMatName->sizePolicy().hasHeightForWidth());
        lineEditMatName->setSizePolicy(sizePolicy2);
        lineEditMatName->setFont(font2);

        HLayoutMatName->addWidget(lineEditMatName);

        horizontalLayoutWidget_4 = new QWidget(MatDlg);
        horizontalLayoutWidget_4->setObjectName(QString::fromUtf8("horizontalLayoutWidget_4"));
        horizontalLayoutWidget_4->setGeometry(QRect(430, 130, 461, 33));
        HLayoutDatabase = new QHBoxLayout(horizontalLayoutWidget_4);
        HLayoutDatabase->setObjectName(QString::fromUtf8("HLayoutDatabase"));
        HLayoutDatabase->setContentsMargins(0, 0, 0, 0);
        labelDatabase = new QLabel(horizontalLayoutWidget_4);
        labelDatabase->setObjectName(QString::fromUtf8("labelDatabase"));
        sizePolicy1.setHeightForWidth(labelDatabase->sizePolicy().hasHeightForWidth());
        labelDatabase->setSizePolicy(sizePolicy1);
        labelDatabase->setFont(font1);
        labelDatabase->setTextFormat(Qt::AutoText);

        HLayoutDatabase->addWidget(labelDatabase);

        comboBoxMatDb = new QComboBox(horizontalLayoutWidget_4);
        comboBoxMatDb->setObjectName(QString::fromUtf8("comboBoxMatDb"));
        comboBoxMatDb->setFont(font1);
        comboBoxMatDb->setEditable(false);
        comboBoxMatDb->setMaxVisibleItems(20);

        HLayoutDatabase->addWidget(comboBoxMatDb);

        splitterGroupMat = new QSplitter(MatDlg);
        splitterGroupMat->setObjectName(QString::fromUtf8("splitterGroupMat"));
        splitterGroupMat->setGeometry(QRect(10, 10, 411, 691));
        splitterGroupMat->setOrientation(Qt::Horizontal);
        verticalLayoutWidget = new QWidget(splitterGroupMat);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        VLayoutGrouplist = new QVBoxLayout(verticalLayoutWidget);
        VLayoutGrouplist->setObjectName(QString::fromUtf8("VLayoutGrouplist"));
        VLayoutGrouplist->setContentsMargins(0, 0, 0, 0);
        HLayoutFiles = new QHBoxLayout();
        HLayoutFiles->setObjectName(QString::fromUtf8("HLayoutFiles"));
        BnLoad = new QToolButton(verticalLayoutWidget);
        BnLoad->setObjectName(QString::fromUtf8("BnLoad"));
        BnLoad->setFont(font1);

        HLayoutFiles->addWidget(BnLoad);

        toolButton = new QToolButton(verticalLayoutWidget);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setFont(font1);

        HLayoutFiles->addWidget(toolButton);


        VLayoutGrouplist->addLayout(HLayoutFiles);

        GroupTree = new QTreeView(verticalLayoutWidget);
        GroupTree->setObjectName(QString::fromUtf8("GroupTree"));
        GroupTree->setMinimumSize(QSize(20, 0));
        GroupTree->setMaximumSize(QSize(1677, 1677));
        GroupTree->setFont(font1);
        GroupTree->setFrameShape(QFrame::StyledPanel);
        GroupTree->setFrameShadow(QFrame::Sunken);
        GroupTree->setMidLineWidth(1);
        GroupTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
        GroupTree->setUniformRowHeights(false);
        GroupTree->header()->setCascadingSectionResizes(false);

        VLayoutGrouplist->addWidget(GroupTree);

        GLayoutGroup = new QGridLayout();
        GLayoutGroup->setObjectName(QString::fromUtf8("GLayoutGroup"));

        VLayoutGrouplist->addLayout(GLayoutGroup);

        splitterGroupMat->addWidget(verticalLayoutWidget);
        verticalLayoutWidget_2 = new QWidget(splitterGroupMat);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        VLayoutMatlist = new QVBoxLayout(verticalLayoutWidget_2);
        VLayoutMatlist->setObjectName(QString::fromUtf8("VLayoutMatlist"));
        VLayoutMatlist->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        VLayoutMatlist->addItem(verticalSpacer);

        labelMat = new QLabel(verticalLayoutWidget_2);
        labelMat->setObjectName(QString::fromUtf8("labelMat"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(labelMat->sizePolicy().hasHeightForWidth());
        labelMat->setSizePolicy(sizePolicy3);
        labelMat->setFont(font1);
        labelMat->setTextFormat(Qt::AutoText);

        VLayoutMatlist->addWidget(labelMat);

        MatTree = new QTreeWidget(verticalLayoutWidget_2);
        MatTree->setObjectName(QString::fromUtf8("MatTree"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(MatTree->sizePolicy().hasHeightForWidth());
        MatTree->setSizePolicy(sizePolicy4);
        QFont font3;
        font3.setBold(false);
        font3.setItalic(false);
        font3.setWeight(50);
        MatTree->setFont(font3);

        VLayoutMatlist->addWidget(MatTree);

        BnConnect = new QToolButton(verticalLayoutWidget_2);
        BnConnect->setObjectName(QString::fromUtf8("BnConnect"));
        BnConnect->setEnabled(true);
        sizePolicy2.setHeightForWidth(BnConnect->sizePolicy().hasHeightForWidth());
        BnConnect->setSizePolicy(sizePolicy2);
        BnConnect->setFont(font1);

        VLayoutMatlist->addWidget(BnConnect);

        BnAuto = new QToolButton(verticalLayoutWidget_2);
        BnAuto->setObjectName(QString::fromUtf8("BnAuto"));
        sizePolicy2.setHeightForWidth(BnAuto->sizePolicy().hasHeightForWidth());
        BnAuto->setSizePolicy(sizePolicy2);
        BnAuto->setFont(font1);

        VLayoutMatlist->addWidget(BnAuto);

        splitterGroupMat->addWidget(verticalLayoutWidget_2);
        splitterGroupMat->raise();
        labelLink->raise();
        buttonBox->raise();
        horizontalLayoutWidget_2->raise();
        verticalLayoutWidget_3->raise();
        horizontalLayoutWidget_5->raise();
        horizontalLayoutWidget_3->raise();
        horizontalLayoutWidget_4->raise();
        QWidget::setTabOrder(BnAddMatList, BnRvMatList);

        retranslateUi(MatDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), MatDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), MatDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(MatDlg);
    } // setupUi

    void retranslateUi(QDialog *MatDlg)
    {
        MatDlg->setWindowTitle(QApplication::translate("MatDlg", "Material Dialog", 0, QApplication::UnicodeUTF8));
        labelLink->setText(QString::fromLatin1("<a href=\"http://www.google.de\">Link to material website>>></a><br>"));
        labelID->setText(QApplication::translate("MatDlg", "ID:", 0, QApplication::UnicodeUTF8));
        labelDensity->setText(QApplication::translate("MatDlg", "Density:", 0, QApplication::UnicodeUTF8));
        labelComp->setText(QApplication::translate("MatDlg", "Composition:", 0, QApplication::UnicodeUTF8));
        BnRvMatList->setText(QApplication::translate("MatDlg", "Remove from Mat. list", 0, QApplication::UnicodeUTF8));
        BnAddMatList->setText(QApplication::translate("MatDlg", "Add into Mat. list", 0, QApplication::UnicodeUTF8));
        BnAddMatDb->setText(QApplication::translate("MatDlg", "Add into Mat. Database", 0, QApplication::UnicodeUTF8));
        labelMatName->setText(QApplication::translate("MatDlg", "Name:", 0, QApplication::UnicodeUTF8));
        labelDatabase->setText(QApplication::translate("MatDlg", "Mat.Database:", 0, QApplication::UnicodeUTF8));
        BnLoad->setText(QApplication::translate("MatDlg", "Load Mat. XML", 0, QApplication::UnicodeUTF8));
        toolButton->setText(QApplication::translate("MatDlg", "Load Mat. MCNP", 0, QApplication::UnicodeUTF8));
        labelMat->setText(QApplication::translate("MatDlg", "Materials:", 0, QApplication::UnicodeUTF8));
        BnConnect->setText(QApplication::translate("MatDlg", "Connect", 0, QApplication::UnicodeUTF8));
        BnAuto->setText(QApplication::translate("MatDlg", "Automatic", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MatDlg: public Ui_MatDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MATEDITORDT1814_H



