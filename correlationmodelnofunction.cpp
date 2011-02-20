#include "correlationmodelnofunction.h"

CorrelationModelNoFunction::CorrelationModelNoFunction(QWidget* parent, QModelDescribing* current, QModelDescribing* target):
        CorrelationModel(parent, current, target)
{
    createTableModel(getTableModel());
}

CorrelationModelNoFunction::CorrelationModelNoFunction(QWidget *parent, QModelDescribing *current, QModelDescribing *target, QStandardItemModel *iTableModel):
        CorrelationModel(parent, current, target, iTableModel)
{
    setupTableModel(getTableModel());
}

 void CorrelationModelNoFunction::createTableModel(QStandardItemModel* tableModel)
{
    tableModel->setHorizontalHeaderLabels(QStringList() << QString::fromUtf8("Поля приёмника") << QString::fromUtf8("Поля источника"));
    tableModel->setColumnCount(2);
}

 void CorrelationModelNoFunction::setupTableModel(QStandardItemModel* tableModel)
 {
     tableModel->setHorizontalHeaderLabels(QStringList() << QString::fromUtf8("Поля приёмника") << QString::fromUtf8("Поля источника"));
     tableModel->setColumnCount(2);
 }
void CorrelationModelNoFunction::fillInTable()
{
    qDebug();
    clearCorrelationTable();
    int iTemplateCount = iCurrentModel->getVisibleElements().count();

    QString nameCurrent, nameTarget;
    QList<QStandardItem*> itemList;
    int i = 0;

    for (; i < iTemplateCount; i++)
    {
        itemList.clear();
        //name1 = iCurrentModel->getVisibleElements()[i].toMap().value(name).toString();
        //Если есть соответсвие, если нет, то элемент следующий смотрим
        QString targetValue = iCurrentModel->getVisibleElements()[i].toMap().value(target).toString();
        QStandardItem *itemCurrent;
        QStandardItem *itemTarget;
        if( !targetValue.isEmpty() )
        {
           int index = findTargetIndexByTargetValue(targetValue);
           if (index != -1)
           {
                nameCurrent = iCurrentModel->getVisibleElements()[i].toMap().value(name).toString();
                itemCurrent =  new QStandardItem(nameCurrent);
                itemCurrent->setData(iCurrentModel->getVisibleElements()[i], Qt::UserRole + 1);
                itemCurrent->setEditable(false);

                nameTarget = iTargetModel->getVisibleElements()[index].toMap().value(name).toString();
                itemTarget = new QStandardItem(nameTarget);
                itemTarget->setData(iTargetModel->getVisibleElements()[index], Qt::UserRole + 1);
                itemTarget->setEditable(false);
           }
           else
           {
               qWarning("Incorrect correlation. Please check description file");
               itemCurrent = null;
               itemTarget = null;
           }
           itemList.append(itemCurrent);
           itemList.append(itemTarget);
           tableModel->appendRow(itemList);
        }
    }
    resizeColumnsToContents();
}

int CorrelationModelNoFunction::findTargetIndexByTargetValue(const QString &targetString)
{
    int targetCount = iTargetModel->getVisibleElements().count();
    for (int i = 0; i< targetCount; i++)
    {
        if(iTargetModel->getVisibleElements()[i].toMap().value(name).toString() == targetString)
            return i;
    }
    return -1;
}
