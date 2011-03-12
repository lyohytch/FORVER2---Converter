#include <QDateTime>

#include "correlationmodelfunction.h"

CorrelationModelFunction::CorrelationModelFunction(QWidget* parent, QModelDescribing* templ, QModelDescribing* target):
    CorrelationModel(parent, templ, target)
{
    setupTableModel(getTableModel());
    isFunc = false;
}

CorrelationModelFunction::CorrelationModelFunction(QWidget* parent, QModelDescribing* templ, QModelDescribing* target, QStandardItemModel* iTableModel):
    CorrelationModel(parent, templ, target, iTableModel)
{
    setupTableModel(getTableModel());
    isFunc = false;
}

void CorrelationModelFunction::setupTableModel(QStandardItemModel* tableModel)
{
    tableModel->setHorizontalHeaderLabels(QStringList() << tr("Receiver fields") << tr("Source fields") << tr("Function"));
    tableModel->setColumnCount(3);
}

void CorrelationModelFunction::fillInTable()
{
    clearCorrelationTable();
    int iTemplateCount = getTemplateModel()->getVisibleElements().count();
    int iTargetCount = getTargetModel()->getVisibleElements().count();
    QString name1, name2;
    QList<QStandardItem*> itemList;
    QVariantMap mapFunc;
    mapFunc.insert(function, CorrelationModelFunction::undef_func);
    int i = 0;
    for (; i < iTemplateCount; i++)
    {
        itemList.clear();
        name1 = getTemplateModel()->getVisibleElements()[i].toMap().value(name).toString();
        QStandardItem* item1 = new QStandardItem(name1);
        item1->setData(getTemplateModel()->getVisibleElements()[i], Qt::UserRole + 1);
        QStandardItem* item2;
        if (i < iTargetCount)
        {
            name2 = getTargetModel()->getVisibleElements()[i].toMap().value(name).toString();
            item2 = new QStandardItem(name2);
            item2->setData(getTargetModel()->getVisibleElements()[i], Qt::UserRole + 1);
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
        getTableModel()->appendRow(itemList);
        item1->setEditable(false);
        item3->setEditable(false);
    }
    this->resizeColumnsToContents();
    this->show();
}

void CorrelationModelFunction::fillInTable(QVariantMap mapTable)
{
    qDebug() << "not implemented yet";
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

        uname.clear();
        uname = functionName(mapTable.value(funcList).toList().at(i).toMap().value(function).toInt());
        QStandardItem* item3 = new QStandardItem(uname);
        item3->setData(mapTable.value(funcList).toList().at(i), Qt::UserRole + 1);
        item3->setEditable(false);

        itemList.append(item1);
        itemList.append(item2);
        itemList.append(item3);
        getTableModel()->appendRow(itemList);

    }
    this->resizeColumnsToContents();
    this->show();
}

bool CorrelationModelFunction::applyTreeClick(int id)
{
    switch (id)
    {
        case iTemplate:
            return CorrelationModel::applyTreeClick(id);
        case iTarget:
            return CorrelationModel::applyTreeClick(id);
        default:
            return isFunc;
    }
}

QVariantMap CorrelationModelFunction::tableModelToMap()
{
    qDebug();
    QVariantMap retMap;
    QVariantList atempList;
    QVariantList atargList;
    QVariantList afuncList;
    int N = getTableModel()->rowCount();
    for (int i = 0 ; i < N; i++)
    {
        atempList.append(getTableModel()->item(i, iTemplate)->data(Qt::UserRole + 1));
        atargList.append(getTableModel()->item(i, iTarget)->data(Qt::UserRole + 1));
        afuncList.append(getTableModel()->item(i, iFunction)->data(Qt::UserRole + 1));
    }
    retMap.insert(tempList, atempList);
    retMap.insert(targList, atargList);
    retMap.insert(funcList, afuncList);
    return retMap;
}

QString CorrelationModelFunction::functionName(int id)
{
    switch (id)
    {
        case agef:
            return "AGE";
        case concatf:
            return "CONCAT";
        default:
            return "NO FUNCTION";
    }
}

void CorrelationModelFunction::setApplyTreeClick(int id)
{
    switch (id)
    {
        case iTemplate://Template
            //Change template doesn't supported
            CorrelationModel::setApplyTreeClick(iTemplate);
            isFunc = false;
            break;
        case iTarget://Target
            CorrelationModel::setApplyTreeClick(iTarget);
            isFunc = false;
            break;
        default://Function
            CorrelationModel::setApplyTreeClick(iTemplate);
            isFunc = true;
            break;
    }
}

void CorrelationModelFunction::changeFunctionValue(int col, int row, int funcId)
{
    qDebug();
    QStandardItem* item = new QStandardItem(functionName(funcId));
    QVariantMap mapFunc;
    mapFunc.insert(function, CorrelationModelFunction::EfuncList(funcId));
    item->setData(mapFunc, Qt::UserRole + 1);
    item->setEditable(false);
    getTableModel()->setItem(row, col, item);
    this->resizeColumnsToContents();
}


QVariant CorrelationModelFunction::age(const QVariant& startDate, const QVariant& endDate)
{
    //All checks are done in age function
    return endDate.toDateTime().date().year() - startDate.toDateTime().date().year();
}
QVariant CorrelationModelFunction::age(const QVariantList& parameters)
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

QVariant CorrelationModelFunction::concat(const QVariantList& parameters)
{
    //TODO check it
    QString retVar;
    for (int i = 0; i < parameters.count(); i++)
    {
        retVar += parameters[i].toString() + space;
    }
    return (QVariant)retVar;
}

QVariant CorrelationModelFunction::switchFunction(int id, const QVariantList& parameters)
{
    QVariant retVar;
    switch (id)
    {
        case CorrelationModelFunction::age_func:
            {
                retVar = age(parameters);
            }
            break;
        case CorrelationModelFunction::concat_func:
            {
                retVar = concat(parameters);
            }
            break;
        default:
            break;
    }
    return parameters.at(0);
}
