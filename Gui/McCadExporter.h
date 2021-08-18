#ifndef MCCADEXPORTER_H
#define MCCADEXPORTER_H

#include <Gui/TaskView/TaskView.h>
#include <Gui/TaskView/TaskDialog.h>
#include <Gui/Tree.h>

#include <string.h>

using namespace std;

namespace Ui {
class ExportModel;
}

class ExportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExportWidget(QWidget* parent = 0);
    ~ExportWidget();

    bool accept();
    bool reject();

private Q_SLOTS:

private:
    class Private;
    Private* d;

    void renameTreenode(QTreeWidgetItem *&treeItem, string strHead, int iStartNum);
    void saveSolids(QTreeWidgetItem *& treeItem);

    QString m_strDir;

};


class TaskExport : public Gui::TaskView::TaskDialog
{
    Q_OBJECT

public:
    TaskExport();
    ~TaskExport();

public:
    void open();
    bool accept();
    bool reject();
    void clicked(int);

    QDialogButtonBox::StandardButtons getStandardButtons() const
    { return QDialogButtonBox::Ok|QDialogButtonBox::Cancel; }

private:
    ExportWidget* widget;
    Gui::TaskView::TaskBox* taskbox;



};


#endif // MCCADEXPORTER_H
