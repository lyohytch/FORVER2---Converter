#include <QVariantList>
#include <QMap>
#include <QDateTime>
#include <QString>

#include "correlationmodel.h"
#include "constants.h"

//Describing model fields
/*const QString id("id");
const QString name("name");
const QString type("type");
const QString level("level");
const QString value("value");*/

CorrelationModel::CorrelationModel(QWidget* parent, QModelDescribing* current, QModelDescribing* target):
    QTableView(parent), iCurrentModel(current) , iTargetModel(target)
{
    tableModel = new QStandardItemModel;
    tableModel->setHorizontalHeaderLabels(QStringList() << QString::fromUtf8("Поля приёмника") << QString::fromUtf8("Поля источника") << QString::fromUtf8("Функция"));
    tableModel->setColumnCount(3);
    this->setModel(tableModel);
    this->resizeColumnsToContents();
    isFunc = isTemp = isTarg = false;
}
CorrelationModel::CorrelationModel(QWidget* parent, QModelDescribing* current, QModelDescribing* target, QStandardItemModel* iTableModel):
    QTableView(parent), iCurrentModel(current) , iTargetModel(target), tableModel(iTableModel)
{
    tableModel->setHorizontalHeaderLabels(QStringList() << QString::fromUtf8("Поля приёмника") << QString::fromUtf8("Поля источника") << QString::fromUtf8("Функция"));
    tableModel->setColumnCount(3);
    this->setModel(tableModel);
    this->resizeColumnsToContents();
    isFunc = isTemp = isTarg = false;
}

CorrelationModel::~CorrelationModel()
{
    if (tableModel) delete tableModel;
}
//Rework it to iListData
QVariantList CorrelationModel::targetToCurrent()
{
    //TODO check it
    qDebug() << " Real start convering....";
    QVariantList retData;
    QVariantList dataList = iTargetModel->getListData();
    QVariantList currDescrList = iCurrentModel->getListSignificant();
    QVariantList searchTemplates;
    QVariantList dataListItem;
    QVariantMap oneMap;
    QVariantList appData;
    int cRow = 0;
    int fID;
    for (int i = 0; i < dataList.count(); i++)
    {
        currDescrList = iCurrentModel->getListSignificant();
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
            return tableModel->item(i, iTarget)->data(Qt::UserRole + 1);
        }
    }
    return NULL;
}

void CorrelationModel::showCorrelationMap()
{
    qDebug() << " Not implemented yet";
}

void CorrelationModel::fillInTable()
{
    qDebug();
    clearTable();
    //TODO п р▒п я∙п б╠п б╟п п├п б╩п║п▐п║Б─ п║п┼ п п┘п║я⌠п б╤п п┘п я∙ п║Б─ п я∙п б╩п║п┼п я■п я∙ п б╥п п┘п б╟п║Б─║п я▒п я≤п║Б─╧п б╣ п║п▄п б╩п б╣п я≤п б╣п п┘п║Б─ п║Б─╧=))))
    int iTemplateCount = iCurrentModel->getListSignificant().count();
    int iTargetCount = iTargetModel->getListSignificant().count();
    QString name1, name2;
    QList<QStandardItem*> itemList;
    QVariantMap mapFunc;
    mapFunc.insert(function, CorrelationModel::undef_func);
    int i = 0;
    for (; i < iTemplateCount; i++)
    {
        itemList.clear();
        name1 = iCurrentModel->getListSignificant()[i].toMap().value(name).toString();
        QStandardItem* item1 = new QStandardItem(name1);
        item1->setData(iCurrentModel->getListSignificant()[i], Qt::UserRole + 1);
        QStandardItem* item2;
        if (i < iTargetCount)
        {
            name2 = iTargetModel->getListSignificant()[i].toMap().value(name).toString();
            item2 = new QStandardItem(name2);
            item2->setData(iTargetModel->getListSignificant()[i], Qt::UserRole + 1);
            item2->setEditable(false);
        }
        else
        {
            item2 = NULL;
        }
        QStandardItem* item3 = new QStandardItem("NOT FUNCTION");
        item3->setData(mapFunc, Qt::UserRole + 1);
        itemList.append(item1);
        itemList.append(item2);
        itemList.append(item3);
        tableModel->appendRow(itemList);
        item1->setEditable(false);
        item3->setEditable(false);
    }
    this->resizeColumnsToContents();
}

void CorrelationModel::fillInTable(QVariantMap mapTable)
{
    qDebug() << "not implemented yet";
    clearTable();
    //QMap( List(tempList),List(targList),List(funcList))
    int N = mapTable.value(tempList).toList().count();//йНКХВЕЯРБН ЩКЕЛЕМРНБ Б РЮАКХЖЕ
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

        //Todo НОПЕДЕКХРЭ ХЛЪ
        uname.clear();
        uname = functionName(mapTable.value(funcList).toList().at(i).toMap().value(function).toInt());
        QStandardItem* item3 = new QStandardItem(uname);
        item3->setData(mapTable.value(funcList).toList().at(i), Qt::UserRole + 1);
        item3->setEditable(false);

        itemList.append(item1);
        itemList.append(item2);
        itemList.append(item3);
        tableModel->appendRow(itemList);

    }
    this->resizeColumnsToContents();
}

void CorrelationModel::clearTable()
{
    this->clearSpans();
    tableModel->clear();
    tableModel->setHorizontalHeaderLabels(QStringList() << QString::fromUtf8("Поля приёмника") << QString::fromUtf8("Поля источника") << QString::fromUtf8("Функция"));
    tableModel->setColumnCount(3);
}

//UNUSED function
//TODO rework this function
void CorrelationModel::appendTableRow(const QVariantList& rowList)
{
    //Don't check. Data is correct already
    //Implement without function
    qDebug();
    int i;
    int cRow = rowList.count();
    QVariantMap mapFunc;
    mapFunc.insert(function, CorrelationModel::undef_func);
    QVariantList itemsData;
    QString name1 = rowList[0].toMap().value(name).toString();//template
    QString name2 = rowList[1].toMap().value(name).toString();//start target name

    QList<QStandardItem*> itemList;
    QStandardItem* item1 = new QStandardItem(name1);
    QStandardItem* item2 = new QStandardItem(name2);
    QStandardItem* item3 = new QStandardItem("Not function");
    item1->setEditable(false);
    item2->setEditable(false);
    item3->setEditable(false);

    for (i = 2; i < cRow; i++)
    {
        name2 += comma + space + rowList[i].toMap().value(name).toString();
        itemsData.append(rowList[i]);
    }
    item1->setData(rowList[0], Qt::UserRole + 1);
    item2->setText(name2);
    item2->setData(itemsData, Qt::UserRole + 1);
    item3->setData(mapFunc, Qt::UserRole + 1);

    itemList.append(item1);
    itemList.append(item2);
    itemList.append(item3);
    tableModel->appendRow(itemList);
    this->resizeColumnsToContents();

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
            return isFunc;
    }
}

void CorrelationModel::setApplyTreeClick(int id)
{
    switch (id)
    {
        case iTemplate://Template
            //Change template doesn't supported
            isTemp = false;
            isFunc = false;
            isTarg = false;
            break;
        case iTarget://Target
            isTemp = false;
            isFunc = false;
            isTarg = true;
            break;
        default://Function
            isTemp = false;
            isFunc = true;
            isTarg = false;
            break;
    }
}

void CorrelationModel::changeFunctionValue(int col, int row, int funcId)
{
    qDebug();
    QStandardItem* item = new QStandardItem(functionName(funcId));
    QVariantMap mapFunc;
    mapFunc.insert(function, CorrelationModel::EfuncList(funcId));
    item->setData(mapFunc, Qt::UserRole + 1);
    item->setEditable(false);
    tableModel->setItem(row, col, item);
    this->resizeColumnsToContents();
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

QString CorrelationModel::functionName(int id)
{
    QString retStr;
    switch (id)
    {
        case agef:
            retStr = "AGE";
            break;
        case concatf:
            retStr = "CONCAT";
            break;
        default:
            retStr = "NO FUNCTION";
    }
    return retStr;
}

QVariantMap CorrelationModel::tableModelToMap()
{
    qDebug();
    QVariantMap retMap;
    QVariantList atempList;
    QVariantList atargList;
    QVariantList afuncList;
    int N = tableModel->rowCount();//п п╬п╩п╦я┤п╣я│я┌п╡п╬ п╥п╟п©п╦я│п╣п╧ - я│я┌я─п╬п╨
    for (int i = 0 ; i < N; i++)
    {
        atempList.append(tableModel->item(i, iTemplate)->data(Qt::UserRole + 1));
        atargList.append(tableModel->item(i, iTarget)->data(Qt::UserRole + 1));
        afuncList.append(tableModel->item(i, iFunction)->data(Qt::UserRole + 1));
    }
    retMap.insert(tempList, atempList);
    retMap.insert(targList, atargList);
    retMap.insert(funcList, afuncList);
    return retMap;
}





//----------------------------CorrelationModelDemo-----------------------
/*
CorrelationModelDemo::CorrelationModelDemo(QWidget *parent,QModelDescribing *current, QModelDescribing *target):
   CorrelationModel(parent,current, target)
{

}*/

QVariant CorrelationModel::age(const QVariant& startDate, const QVariant& endDate)
{
    //All checks are done in age function
    return endDate.toDateTime().date().year() - startDate.toDateTime().date().year();
}
QVariant CorrelationModel::age(const QVariantList& parameters)
{
    if (parameters.count() == 2)
    {
        if ((!parameters[0].isNull() && parameters[0].type() == QVariant::DateTime) &&
            (!parameters[1].isNull() && parameters[1].type() == QVariant::DateTime))
        {
            return age(parameters[0], parameters[1]);
        }
    }
    return QVariant();
}

//-----UNUSED--
/*
QString CorrelationModelDemo::concat(char* arg1 ...)
{
    QString retStr = "";
    va_list ap;
    va_start(ap,arg1);
    for(;;)
    {
        char *p = va_arg(ap, char*);
        if (p == NULL)
        {
            break;
        }
        retStr += QString::fromUtf8(p) + space;
    }
    va_end(ap);
    return retStr;
}
*/
//------UNUSED--

QVariant CorrelationModel::concat(const QVariantList& parameters)
{
    //TODO check it
    QString retVar;
    for (int i = 0; i < parameters.count(); i++)
    {
        retVar += parameters[i].toString() + space;
    }
    return (QVariant)retVar;
}

QVariant CorrelationModel::switchFunction(int id, const QVariantList& parameters)
{
    QVariant retVar;
    switch (id)
    {
        case CorrelationModel::age_func:
            {
                retVar = age(parameters);
            }
            break;
        case CorrelationModel::concat_func:
            {
                retVar = concat(parameters);
            }
            break;
        default:
            {
            }
            break;
    }
    return parameters.at(0);
}
