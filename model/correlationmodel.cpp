#include <QVariantList>
#include <QMap>
#include <QString>

#include "correlationmodel.h"
#include "constants.h"

//Describing model fields
/*const QString id("id");
const QString name("name");
const QString type("type");
const QString level("level");
const QString value("value");*/

// TODO: make corr model
CorrelationModel::CorrelationModel(QWidget* parent, QModelDescribing* templ, QModelDescribing* target):
    QTableView(parent), iTemplateModel(templ) , iTargetModel(target)
{
    tableModel = new QStandardItemModel;
    this->setModel(tableModel);
    this->resizeColumnsToContents();
    this->hide();
    isTemp = isTarg = false;
}
CorrelationModel::CorrelationModel(QWidget* parent, QModelDescribing* templ, QModelDescribing* target, QStandardItemModel* iTableModel):
    QTableView(parent), iTemplateModel(templ) , iTargetModel(target), tableModel(iTableModel)
{

    this->setModel(tableModel);
    this->resizeColumnsToContents();
    this->hide();
    isTemp = isTarg = false;
}

CorrelationModel::~CorrelationModel()
{
    if (tableModel) delete tableModel;
}

//Rework it to iListData

QVariantList CorrelationModel::targetToTemplate()
{
    //TODO check it
    qDebug() << " Real start convering....";
    QVariantList retData;
    QVariantList dataList = iTargetModel->getElementsWithData();
    QVariantList currDescrList = iTemplateModel->getVisibleElements();
    QVariantList searchTemplates;
    QVariantList dataListItem;
    QVariantMap oneMap;
    QVariantList appData;
    int cRow = 0;
    int fID;
    for (int i = 0; i < dataList.count(); i++)
    {
        currDescrList = iTemplateModel->getVisibleElements();
        dataListItem = dataList[i].toMap().value(rapid).toList();
        oneMap.insert(id, i);
        appData.clear();
        foreach(QVariant curr, currDescrList)
        {
            QVariantMap currAdd = curr.toMap();
            searchTemplates.clear();
            searchTemplates.append(findItemInTableTemplate(curr, cRow));
            fID = tableModel->item(cRow, iFunction)->data(Qt::UserRole + 1).toMap().value(function).toInt();
            currAdd.insert(dvalue,
                           switchFunction(fID, foundByUIDsRetValues(dataListItem, searchTemplates)));
            qDebug() << currAdd;
            appData.append(currAdd);
        }
        oneMap.insert(rapid, appData);
        retData.append(oneMap);
    }
    return retData;
}

QVariantList CorrelationModel::foundByUIDsRetValues(const QVariantList& dataListItem, const QVariantList& searchTemplates)
{
    QVariantList retList;
    foreach(QVariant sTemplate, searchTemplates)
    {
        foreach(QVariant dItem, dataListItem)
        {
            if (sTemplate.toMap().value(id) == dItem.toMap().value(id))
            {
                retList.append(dItem.toMap().value(dvalue));
            }
        }
    }
    return retList;
}

QVariant CorrelationModel::findItemInTableTemplate(const QVariant& search, int& row)
{
    int count = tableModel->rowCount();
    for (int i = 0; i < count; i++)
    {
        if (tableModel->item(i, iTemplate)->data(Qt::UserRole + 1) == search)
        {
            row = i;
            if(tableModel->item(i, iTarget) != NULL )
            {
                return tableModel->item(i, iTarget)->data(Qt::UserRole + 1);
            }
            else
            {
                return QVariant();
            }
        }
    }
    return QVariant();
}

void CorrelationModel::fillInTable(QVariantMap mapTable)
{
    clearCorrelationTable();
    //QMap( List(tempList),List(targList),List(funcList))
    int N = mapTable.value(tempList).toList().count();
    QString uname;
    QList<QStandardItem*> itemList;
    int cTempElem = 0;
    for (int i = 0; i < N ; i++)
    {
        itemList.clear();
        uname.clear();
        uname = mapTable.value(tempList).toList().at(i).toMap().value(name).toString();
        QStandardItem* item1 = new QStandardItem(uname);
        item1->setData(mapTable.value(tempList).toList().at(i), Qt::UserRole + 1);
        item1->setEditable(false);
        cTempElem = mapTable.value(targList).toList().at(i).toList().count();
        uname.clear();
        if (cTempElem == 0)
        {
            uname = mapTable.value(targList).toList().at(i).toMap().value(name).toString();
        }
        else
        {
            for (int j = 0; j < cTempElem; j++)
            {
                QVariantMap aTargElem = mapTable.value(targList).toList().at(i).toList().at(j).toMap();
                if (j == 0)
                {
                    uname += aTargElem.value(name).toString();
                }
                else
                {
                    uname += comma + space + aTargElem.value(name).toString();
                }
            }
        }
        QStandardItem* item2 = new QStandardItem(uname);
        item2->setData(mapTable.value(targList).toList().at(i), Qt::UserRole + 1);
        item2->setEditable(false);

        itemList.append(item1);
        itemList.append(item2);
        tableModel->appendRow(itemList);

    }
    this->resizeColumnsToContents();
    this->show();
}

void CorrelationModel::clearCorrelationTable()
{
    this->clearSpans();
    tableModel->clear();
    setupTableModel(tableModel);
}

bool CorrelationModel::applyTreeClick(int id)
{
    switch (id)
    {
        case iTemplate:
            return isTemp;
        case iTarget:
            return isTarg;
        default:
            return isTemp;
    }
}

void CorrelationModel::setApplyTreeClick(int id)
{
    switch (id)
    {
        case iTemplate://Template
            //Change template doesn't supported
            isTemp = false;
            isTarg = false;
            break;
        case iTarget://Target
            isTemp = false;
            isTarg = true;
            break;
        default:
            break;
    }
}

void CorrelationModel::changeTemplateValue(int col, int row, QVariant data)
{
    qDebug();
    QStandardItem* item = new QStandardItem(data.toMap().value(name).toString());
    item->setData(data, Qt::UserRole + 1);
    item->setEditable(false);
    tableModel->setItem(row, col, item);
    this->resizeColumnsToContents();
}

void CorrelationModel::changeTargetValue(int col, int row, QVariant data, bool first)
{
    //TODO check for multiple data
    qDebug() << "::" << data << " is First = " << first;
    QStandardItem* item;
    if (first)
    {
        item = new QStandardItem(data.toMap().value(name).toString());
        item->setData(data, Qt::UserRole + 1);
    }
    else
    {
        item = tableModel->item(row, col);
        QVariantList appData;
        QVariant itemData = item->data(Qt::UserRole + 1);
        QString nameCell = item->text() + ", " + data.toMap().value(name).toString();
        appData.append(itemData);
        appData.append(data);
        item->setData(appData, Qt::UserRole + 1);
        item->setText(nameCell);
    }
    item->setEditable(false);
    tableModel->setItem(row, col, item);
    this->resizeColumnsToContents();
}

QVariantMap CorrelationModel::tableModelToMap()
{
    QVariantMap retMap;
    QVariantList atempList;
    QVariantList atargList;
    int N = tableModel->rowCount();
    for (int i = 0 ; i < N; i++)
    {
        atempList.append(tableModel->item(i, iTemplate)->data(Qt::UserRole + 1));
        atargList.append(tableModel->item(i, iTarget)->data(Qt::UserRole + 1));
    }
    retMap.insert(tempList, atempList);
    retMap.insert(targList, atargList);
    return retMap;
}

