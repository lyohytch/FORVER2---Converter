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
    tableModel->setHorizontalHeaderLabels(QStringList() << QString::fromUtf8("���� ��������") << QString::fromUtf8("���� ���������"));
    tableModel->setColumnCount(2);
}

 void CorrelationModelNoFunction::setupTableModel(QStandardItemModel* tableModel)
 {
     tableModel->setHorizontalHeaderLabels(QStringList() << QString::fromUtf8("���� ��������") << QString::fromUtf8("���� ���������"));
     tableModel->setColumnCount(2);
 }
void CorrelationModelNoFunction::fillInTable()
{
    qDebug();
    clearCorrelationTable();
    int iTemplateCount = getCurrentModel()->getVisibleElements().count();

    QString nameCurrent, nameTarget;
    QList<QStandardItem*> itemList;
    int i = 0;

    for (; i < iTemplateCount; i++)
    {
        itemList.clear();
        //name1 = iCurrentModel->getVisibleElements()[i].toMap().value(name).toString();
        //���� ���� �����������, ���� ���, �� ������� ��������� �������
        QString targetValue = getCurrentModel()->getVisibleElements()[i].toMap().value(target).toString();
        QStandardItem *itemCurrent;
        QStandardItem *itemTarget;
        if( !targetValue.isEmpty() )
        {
           int index = findTargetIndexByTargetValue(targetValue);
           if (index != -1)
           {
                nameCurrent = getCurrentModel()->getVisibleElements()[i].toMap().value(name).toString();
                itemCurrent =  new QStandardItem(nameCurrent);
                itemCurrent->setData(getCurrentModel()->getVisibleElements()[i], Qt::UserRole + 1);
                itemCurrent->setEditable(false);

                nameTarget = getTargetModel()->getVisibleElements()[index].toMap().value(name).toString();
                itemTarget = new QStandardItem(nameTarget);
                itemTarget->setData(getTargetModel()->getVisibleElements()[index], Qt::UserRole + 1);
                itemTarget->setEditable(false);
           }
           else
           {
               qWarning()<<"Incorrect correlation. Please check description file";
               itemCurrent = NULL;
               itemTarget = NULL;
           }
           itemList.append(itemCurrent);
           itemList.append(itemTarget);
           getTableModel()->appendRow(itemList);
        }
    }
    resizeColumnsToContents();
}

int CorrelationModelNoFunction::findTargetIndexByTargetValue(const QString &targetString)
{
    int targetCount = getTargetModel()->getVisibleElements().count();
    for (int i = 0; i< targetCount; i++)
    {
        if(getTargetModel()->getVisibleElements()[i].toMap().value(name).toString() == targetString)
            return i;
    }
    return -1;
}
