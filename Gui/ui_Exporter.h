/********************************************************************************
** Form generated from reading UI file 'Export.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef EXPORTTH8752_H
#define EXPORTTH8752_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExportModel
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QTextEdit *textEditDir;
    QLabel *labelSteps;
    QProgressBar *progressBar;

    void setupUi(QWidget *ExportModel)
    {
        if (ExportModel->objectName().isEmpty())
            ExportModel->setObjectName(QString::fromUtf8("ExportModel"));
        ExportModel->resize(360, 203);
        gridLayout = new QGridLayout(ExportModel);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox = new QGroupBox(ExportModel);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 9, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        textEditDir = new QTextEdit(groupBox);
        textEditDir->setObjectName(QString::fromUtf8("textEditDir"));
        textEditDir->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(textEditDir->sizePolicy().hasHeightForWidth());
        textEditDir->setSizePolicy(sizePolicy1);
        textEditDir->setMaximumSize(QSize(16777215, 100));
        QPalette palette;
        QBrush brush(QColor(232, 232, 231, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        textEditDir->setPalette(palette);
        textEditDir->setMouseTracking(false);
        textEditDir->setUndoRedoEnabled(true);
        textEditDir->setReadOnly(true);

        verticalLayout->addWidget(textEditDir);

        labelSteps = new QLabel(groupBox);
        labelSteps->setObjectName(QString::fromUtf8("labelSteps"));
        QFont font;
        font.setPointSize(11);
        font.setBold(false);
        font.setItalic(true);
        font.setWeight(50);
        labelSteps->setFont(font);

        verticalLayout->addWidget(labelSteps);

        progressBar = new QProgressBar(groupBox);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(20);
        sizePolicy2.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy2);
        progressBar->setMinimumSize(QSize(0, 20));
        progressBar->setMaximumSize(QSize(16777215, 40));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Cantarell"));
        progressBar->setFont(font1);
        progressBar->setValue(0);
        progressBar->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(progressBar);


        gridLayout_2->addLayout(verticalLayout, 1, 0, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);


        retranslateUi(ExportModel);

        QMetaObject::connectSlotsByName(ExportModel);
    } // setupUi

    void retranslateUi(QWidget *ExportModel)
    {
        ExportModel->setWindowTitle(QApplication::translate("ExportModel", "Export Solids", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ExportModel", "Working Directory", 0, QApplication::UnicodeUTF8));
        labelSteps->setText(QApplication::translate("ExportModel", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};


namespace Ui {
    class ExportModel: public Ui_ExportModel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTER_H
