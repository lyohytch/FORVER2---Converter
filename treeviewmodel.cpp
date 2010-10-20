#include <QStandardItem>
#include <QHeaderView>
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
        model->setHorizontalHeaderLabels(QStringList() << QString::fromUtf8("КУДА КОНВЕРТИРОВАТЬ") );
    }
    else
    {
        model->setHorizontalHeaderLabels(QStringList() << QString::fromUtf8("ЧТО КОНВЕРТИРОВАТЬ") );
    }

    this->reset();
    this->setModel(model);
    this->expandAll();
    this->show();
}
