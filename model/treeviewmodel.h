#ifndef TREEVIEWMODEL_H
#define TREEVIEWMODEL_H
#include <QTreeView>
#include <QStandardItemModel>
#include <QStringList>
#include "qmodeldescribing.h"
//Возможно наследование qmodeldescribing от QStandardItemModel
class TreeViewModel: public QTreeView
{
        Q_OBJECT

    public:
        TreeViewModel(QWidget* parent = 0, bool isT = false);
        ~TreeViewModel() {};
        void loadModel(QStandardItemModel* model);
    private:
        bool isTemplate;

};

#endif // TREEVIEWMODEL_H
