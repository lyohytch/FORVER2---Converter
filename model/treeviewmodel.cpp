#include <QStandardItem>
#include <QHeaderView>
#include "treeviewmodel.h"


TreeViewModel::TreeViewModel(QWidget* parent, bool isT):
    QTreeView(parent), isTemplate(isT)
{
    this->hide();
}
void TreeViewModel::loadModel(QStandardItemModel* model)
{

    if (isTemplate)
    {
        //model->setHorizontalHeaderLabels(QStringList() << QString::fromUtf8("Приёмник: куда конвертировать"));
        model->setHorizontalHeaderLabels(QStringList()<<tr("Receiver: where"));
    }
    else
    {
        model->setHorizontalHeaderLabels(QStringList()<<tr("Source: what"));
        //model->setHorizontalHeaderLabels(QStringList() << QString::fromUtf8("Исходник: что конвертировать"));
    }

    this->reset();
    this->setModel(model);
    this->expandAll();
    this->show();
}
