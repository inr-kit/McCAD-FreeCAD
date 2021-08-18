#include "PreCompiled.h"
#include "McCadComboDelegate.h"
#include <qcombobox.h>

ComboDelegate::ComboDelegate(QObject *parent): QItemDelegate(parent)
{
}

QWidget *ComboDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &index ) const
{
    QWidget *widget = nullptr;
    if(index.isValid() && index.column() == 1)
    {
        QComboBox *editor = new QComboBox(parent);
        QStringList list ;
        list << tr("Yes") << tr("No");
        editor->addItems(list);
        return editor;
    }
    else
    {
      return widget;
    }
}

void ComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::DisplayRole).toString();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->addItem(value);
}

void ComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();
    model->setData(index, value);
}

void ComboDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
