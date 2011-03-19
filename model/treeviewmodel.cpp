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
        model->setHorizontalHeaderLabels(QStringList() << tr("Receiver: where"));
    }
    else
    {
        model->setHorizontalHeaderLabels(QStringList() << tr("Source: what"));
    }

    this->reset();
    this->setModel(model);
    this->expandAll();
    this->show();
}
