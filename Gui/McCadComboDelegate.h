#ifndef MCCADCOMBODELEGATE_H
#define MCCADCOMBODELEGATE_H
#include <qitemdelegate.h>

class ComboDelegate : public QItemDelegate
{
    //Q_OBJECT

public:
    ComboDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // MCCADCOMBODELEGATE_H
