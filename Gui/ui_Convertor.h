/********************************************************************************
** Form generated from reading UI file 'Convertor.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONVERTOR_H
#define UI_CONVERTOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_DlgConversion
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_PrcMain;
    QProgressBar *progressBar_Main;
    QLabel *label_PrcPart;
    QProgressBar *progressBar_Part;
    QFrame *line;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QComboBox *comboBox_Precision;
    QLabel *label_StartCell;
    QLineEdit *lineEdit_CellNum;
    QLineEdit *lineEdit_SurfNum;
    QLabel *label_precision;
    QLabel *label_GenVoid;
    QCheckBox *checkBox_Void;
    QLabel *label_StartSurf;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QToolButton *BntStart;
    QToolButton *BntStop;
    QTextEdit *textEdit_Message;

    void setupUi(QDialog *DlgConversion)
    {
        if (DlgConversion->objectName().isEmpty())
            DlgConversion->setObjectName(QString::fromUtf8("DlgConversion"));
        DlgConversion->resize(520, 744);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DlgConversion->sizePolicy().hasHeightForWidth());
        DlgConversion->setSizePolicy(sizePolicy);
        DlgConversion->setSizeGripEnabled(true);
        DlgConversion->setModal(false);
        verticalLayoutWidget = new QWidget(DlgConversion);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 160, 501, 116));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_PrcMain = new QLabel(verticalLayoutWidget);
        label_PrcMain->setObjectName(QString::fromUtf8("label_PrcMain"));

        verticalLayout->addWidget(label_PrcMain);

        progressBar_Main = new QProgressBar(verticalLayoutWidget);
        progressBar_Main->setObjectName(QString::fromUtf8("progressBar_Main"));
        progressBar_Main->setValue(0);

        verticalLayout->addWidget(progressBar_Main);

        label_PrcPart = new QLabel(verticalLayoutWidget);
        label_PrcPart->setObjectName(QString::fromUtf8("label_PrcPart"));

        verticalLayout->addWidget(label_PrcPart);

        progressBar_Part = new QProgressBar(verticalLayoutWidget);
        progressBar_Part->setObjectName(QString::fromUtf8("progressBar_Part"));
        progressBar_Part->setValue(0);

        verticalLayout->addWidget(progressBar_Part);

        line = new QFrame(DlgConversion);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(10, 140, 571, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        gridLayoutWidget = new QWidget(DlgConversion);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 401, 131));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        comboBox_Precision = new QComboBox(gridLayoutWidget);
        comboBox_Precision->setObjectName(QString::fromUtf8("comboBox_Precision"));

        gridLayout->addWidget(comboBox_Precision, 2, 1, 1, 1);

        label_StartCell = new QLabel(gridLayoutWidget);
        label_StartCell->setObjectName(QString::fromUtf8("label_StartCell"));

        gridLayout->addWidget(label_StartCell, 0, 0, 1, 1);

        lineEdit_CellNum = new QLineEdit(gridLayoutWidget);
        lineEdit_CellNum->setObjectName(QString::fromUtf8("lineEdit_CellNum"));

        gridLayout->addWidget(lineEdit_CellNum, 0, 1, 1, 1);

        lineEdit_SurfNum = new QLineEdit(gridLayoutWidget);
        lineEdit_SurfNum->setObjectName(QString::fromUtf8("lineEdit_SurfNum"));

        gridLayout->addWidget(lineEdit_SurfNum, 1, 1, 1, 1);

        label_precision = new QLabel(gridLayoutWidget);
        label_precision->setObjectName(QString::fromUtf8("label_precision"));

        gridLayout->addWidget(label_precision, 2, 0, 1, 1);

        label_GenVoid = new QLabel(gridLayoutWidget);
        label_GenVoid->setObjectName(QString::fromUtf8("label_GenVoid"));

        gridLayout->addWidget(label_GenVoid, 3, 0, 1, 1);

        checkBox_Void = new QCheckBox(gridLayoutWidget);
        checkBox_Void->setObjectName(QString::fromUtf8("checkBox_Void"));

        gridLayout->addWidget(checkBox_Void, 3, 1, 1, 1);

        label_StartSurf = new QLabel(gridLayoutWidget);
        label_StartSurf->setObjectName(QString::fromUtf8("label_StartSurf"));

        gridLayout->addWidget(label_StartSurf, 1, 0, 1, 1);

        verticalLayoutWidget_2 = new QWidget(DlgConversion);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(420, 10, 92, 133));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        BntStart = new QToolButton(verticalLayoutWidget_2);
        BntStart->setObjectName(QString::fromUtf8("BntStart"));
        BntStart->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(BntStart->sizePolicy().hasHeightForWidth());
        BntStart->setSizePolicy(sizePolicy1);
        BntStart->setMinimumSize(QSize(90, 90));

        verticalLayout_2->addWidget(BntStart);

        BntStop = new QToolButton(verticalLayoutWidget_2);
        BntStop->setObjectName(QString::fromUtf8("BntStop"));
        sizePolicy.setHeightForWidth(BntStop->sizePolicy().hasHeightForWidth());
        BntStop->setSizePolicy(sizePolicy);
        BntStop->setMinimumSize(QSize(90, 35));

        verticalLayout_2->addWidget(BntStop);

        textEdit_Message = new QTextEdit(DlgConversion);
        textEdit_Message->setObjectName(QString::fromUtf8("textEdit_Message"));
        textEdit_Message->setGeometry(QRect(10, 290, 501, 441));
        verticalLayoutWidget->raise();
        line->raise();
        gridLayoutWidget->raise();
        verticalLayoutWidget_2->raise();
        label_PrcPart->raise();
        textEdit_Message->raise();

        retranslateUi(DlgConversion);

        QMetaObject::connectSlotsByName(DlgConversion);
    } // setupUi

    void retranslateUi(QDialog *DlgConversion)
    {
        DlgConversion->setWindowTitle(QApplication::translate("DlgConversion", "CAD-MC Convertor", 0, QApplication::UnicodeUTF8));
        label_PrcMain->setText(QApplication::translate("DlgConversion", "Main Progress", 0, QApplication::UnicodeUTF8));
        label_PrcPart->setText(QApplication::translate("DlgConversion", "Part Progress", 0, QApplication::UnicodeUTF8));
        label_StartCell->setText(QApplication::translate("DlgConversion", "Starting Cell Number:        ", 0, QApplication::UnicodeUTF8));
        label_precision->setText(QApplication::translate("DlgConversion", "Precsion of Conversion:", 0, QApplication::UnicodeUTF8));
        label_GenVoid->setText(QApplication::translate("DlgConversion", "Generate Void:", 0, QApplication::UnicodeUTF8));
        checkBox_Void->setText(QString());
        label_StartSurf->setText(QApplication::translate("DlgConversion", "Starting Surface Number:     ", 0, QApplication::UnicodeUTF8));
        BntStart->setText(QApplication::translate("DlgConversion", "Start", 0, QApplication::UnicodeUTF8));
        BntStop->setText(QApplication::translate("DlgConversion", "Stop", 0, QApplication::UnicodeUTF8));
    } // retranslateUi
};

namespace Ui {
    class McCadConvertorDlg: public Ui_DlgConversion {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONVERTOR_H
