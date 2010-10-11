#include <QStandardItem>
#include "treeviewmodel.h"
#include <QDebug>

TreeViewModel::TreeViewModel(QWidget * parent,bool isT):
        QTreeView(parent),isTemplate(isT)
{
    this->hide();
}
void TreeViewModel::loadModel(QStandardItemModel *model)
{
    qDebug()<<Q_FUNC_INFO;
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
