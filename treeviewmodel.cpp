#include <QStandardItem>
#include "treeviewmodel.h"


TreeViewModel::TreeViewModel(QWidget * parent,bool isT):
        QTreeView(parent),isTemplate(isT)
{
    this->hide();
}
void TreeViewModel::loadModel(QStandardItemModel *model)
{
    if(isTemplate)
    {
        model->setHorizontalHeaderLabels(QStringList() << "Template file" );
    }
    else
    {
         model->setHorizontalHeaderLabels(QStringList() << "Target file" );
    }
    this->setModel(model);
    this->expandAll();
    this->show();
}
