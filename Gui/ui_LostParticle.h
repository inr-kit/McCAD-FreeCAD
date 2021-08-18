/********************************************************************************
** Form generated from reading UI file 'LostParticlek12214.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef LOSTPARTICLEK12214_H
#define LOSTPARTICLEK12214_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QToolButton>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DlgParticleLost
{
public:
    QDialogButtonBox *buttonBox;
    QTreeView *treeView;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lbFrom;
    QLineEdit *lEFrom;
    QLabel *lbto;
    QLineEdit *lEto;
    QToolButton *BnDisplay;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QToolButton *BnLoadFile;
    QLineEdit *lineEditFile;
    QFrame *line;

    void setupUi(QDialog *DlgParticleLost)
    {
        if (DlgParticleLost->objectName().isEmpty())
            DlgParticleLost->setObjectName(QString::fromUtf8("DlgParticleLost"));
        DlgParticleLost->resize(551, 672);
        buttonBox = new QDialogButtonBox(DlgParticleLost);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(200, 630, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        treeView = new QTreeView(DlgParticleLost);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setGeometry(QRect(10, 110, 531, 511));
        horizontalLayoutWidget_2 = new QWidget(DlgParticleLost);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(10, 60, 531, 41));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        lbFrom = new QLabel(horizontalLayoutWidget_2);
        lbFrom->setObjectName(QString::fromUtf8("lbFrom"));

        horizontalLayout_2->addWidget(lbFrom);

        lEFrom = new QLineEdit(horizontalLayoutWidget_2);
        lEFrom->setObjectName(QString::fromUtf8("lEFrom"));

        horizontalLayout_2->addWidget(lEFrom);

        lbto = new QLabel(horizontalLayoutWidget_2);
        lbto->setObjectName(QString::fromUtf8("lbto"));

        horizontalLayout_2->addWidget(lbto);

        lEto = new QLineEdit(horizontalLayoutWidget_2);
        lEto->setObjectName(QString::fromUtf8("lEto"));

        horizontalLayout_2->addWidget(lEto);

        BnDisplay = new QToolButton(horizontalLayoutWidget_2);
        BnDisplay->setObjectName(QString::fromUtf8("BnDisplay"));
        BnDisplay->setMinimumSize(QSize(150, 0));
        BnDisplay->setMaximumSize(QSize(16777215, 32));

        horizontalLayout_2->addWidget(BnDisplay);

        horizontalLayoutWidget = new QWidget(DlgParticleLost);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 531, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        BnLoadFile = new QToolButton(horizontalLayoutWidget);
        BnLoadFile->setObjectName(QString::fromUtf8("BnLoadFile"));
        BnLoadFile->setMinimumSize(QSize(100, 0));
        BnLoadFile->setMaximumSize(QSize(2000, 32));

        horizontalLayout->addWidget(BnLoadFile);

        lineEditFile = new QLineEdit(horizontalLayoutWidget);
        lineEditFile->setObjectName(QString::fromUtf8("lineEditFile"));
        lineEditFile->setMaximumSize(QSize(16777215, 34));

        horizontalLayout->addWidget(lineEditFile);

        line = new QFrame(DlgParticleLost);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(10, 50, 531, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        retranslateUi(DlgParticleLost);
        QObject::connect(buttonBox, SIGNAL(accepted()), DlgParticleLost, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DlgParticleLost, SLOT(reject()));

        QMetaObject::connectSlotsByName(DlgParticleLost);
    } // setupUi

    void retranslateUi(QDialog *DlgParticleLost)
    {
        DlgParticleLost->setWindowTitle(QApplication::translate("DlgParticleLost", "Display Lost Particles", 0, QApplication::UnicodeUTF8));
        lbFrom->setText(QApplication::translate("DlgParticleLost", "From", 0, QApplication::UnicodeUTF8));
        lbto->setText(QApplication::translate("DlgParticleLost", "To", 0, QApplication::UnicodeUTF8));
        BnDisplay->setText(QApplication::translate("DlgParticleLost", "Display Particles", 0, QApplication::UnicodeUTF8));
        BnLoadFile->setText(QApplication::translate("DlgParticleLost", "Load File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class McCadLostParticle: public Ui_DlgParticleLost {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOSTPARTICLE_H
