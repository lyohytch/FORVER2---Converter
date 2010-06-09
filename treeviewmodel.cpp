#include <QStandardItem>
#include "treeviewmodel.h"
#include <QDebug>

TreeViewModel::TreeViewModel(QWidget * parent,bool isT):
        QTreeView(parent),isTemplate(isT)
{
    /*if(isT)
    {
        this->setGeometry(0,40,parent->width()/2,parent->height() - 50);
    }
    else
    {
        this->setGeometry(parent->width()/2+10,40,parent->width()/2 - 10,parent->height() - 50);
    }*/
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
