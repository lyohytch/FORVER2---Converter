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

 //TODO: work on this problem
void CorrelationModelNoFunction::fillInTable()
{
    qDebug();
    clearCorrelationTable();
    int iTemplateCount = getCurrentModel()->getVisibleElements().count();

    QString nameCurrent, nameTarget;
    QList<QStandardItem*> itemList;
    int i = 0;
    //TODO: move to foreach
    for (; i < iTemplateCount; i++)
    {
        itemList.clear();
        //name1 = iCurrentModel->getVisibleElements()[i].toMap().value(name).toString();
        //Г…Г±Г«ГЁ ГҐГ±ГІГј Г±Г®Г®ГІГўГҐГІГ±ГўГЁГҐ, ГҐГ±Г«ГЁ Г­ГҐГІ, ГІГ® ГЅГ«ГҐГ¬ГҐГ­ГІ Г±Г«ГҐГ¤ГіГѕГ№ГЁГ© Г±Г¬Г®ГІГ°ГЁГ¬
        QString targetValue = getCurrentModel()->getVisibleElements()[i].toMap().value(targetId).toString();
        qDebug()<<" Template by Ids => "<<getCurrentModel()->getVisibleElements()[i].toMap().value(id).toString();
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
               continue;
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
        qDebug()<<"targetModel Ids = > "<<getTargetModel()->getVisibleElements()[i].toMap().value(id);
        if(getTargetModel()->getVisibleElements()[i].toMap().value(id).toString() == targetString)
            return i;
    }
    return -1;
}

//!!!!!!!Если у зависимых полей несколько соответвий из второй части, то указываем это в таблице отношений
QVariantList CorrelationModelNoFunction::targetToCurrent()
{
    qDebug() << " Real start convering....";
    QVariantList retData;
    QVariantList convertedData = QVariantList();
    QVariantList dataForConverting = getTargetModel()->getElementsWithData();
    QVariantList currDescrList = getCurrentModel()->getVisibleElements();

    int cRow = 0;
    QVariantMap mapToAddingInResult = QVariantMap();
    QVariantMap mapForOneElementData = QVariantMap();
    QVariantList searchTemplates = QVariantList();
    QVariantList dataToAppend = QVariantList();

    for (int i = 0; i < dataForConverting.count(); i++)
    {
        convertedData = dataForConverting[i].toMap().value(rapid).toList();//наше обычное описание но с полем dvalue(данные)
        mapToAddingInResult.insert(id, i);
        dataToAppend.clear();
        foreach(QVariant curr, currDescrList)
        {
            mapForOneElementData = curr.toMap();

            searchTemplates.clear();
            searchTemplates.append(findItemInTableTemplate(curr, cRow));// Нашли темплейт в корреляционной таблице

            mapForOneElementData.insert(dvalue, processTargetData(convertedData, searchTemplates) );

            qDebug() << mapForOneElementData;

            dataToAppend.append(mapForOneElementData);
        }
        mapToAddingInResult.insert(rapid, dataToAppend);
        retData.append(mapToAddingInResult);
    }

    return retData;
}

QString CorrelationModelNoFunction::processTargetData(const QVariantList & convertedData, const QVariantList& searchTemplates)
{
     QString dataString = "";
     Q_UNUSED(convertedData)
     if(!searchTemplates.isEmpty())
     {
         //here is a processing

     }
     return dataString;
}
