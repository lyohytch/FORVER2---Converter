#include "correlationmodelnofunction.h"

CorrelationModelNoFunction::CorrelationModelNoFunction(QWidget* parent, QModelDescribing* templ, QModelDescribing* target):
        CorrelationModel(parent, templ, target)
{
    setupTableModel(getTableModel());
    this->show();
}

CorrelationModelNoFunction::CorrelationModelNoFunction(QWidget *parent, QModelDescribing *templ, QModelDescribing *target, QStandardItemModel *iTableModel):
        CorrelationModel(parent, templ, target, iTableModel)
{
    setupTableModel(getTableModel());
}

 void CorrelationModelNoFunction::setupTableModel(QStandardItemModel* tableModel)
 {    
     tableModel->setHorizontalHeaderLabels(QStringList()<<tr("Receiver fields") << tr("Source fields"));
     tableModel->setColumnCount(2);
 }

 //TODO: work on this problem
void CorrelationModelNoFunction::fillInTable()
{
    qDebug();
    clearCorrelationTable();
    int iTemplateCount = getTemplateModel()->getVisibleElements().count();

    QString nameCurrent;
    QList<QStandardItem*> itemList;
    int i = 0;
    //TODO: move to foreach
    for (; i < iTemplateCount; i++)
    {
        itemList.clear();
        //name1 = iCurrentModel->getVisibleElements()[i].toMap().value(name).toString();
        //QString formIdName = getTemplateModel()->getVisibleElements()[i].toMap().value(formId).toString();
        QString targetIdName = getTemplateModel()->getVisibleElements()[i].toMap().value(targetName).toString();
        qDebug()<<" Template by Ids => "<<getTemplateModel()->getVisibleElements()[i].toMap().value(id).toString();
        QStandardItem *itemTemplate;
        QStandardItem *itemTarget;

        nameCurrent = getTemplateModel()->getVisibleElements()[i].toMap().value(name).toString();
        itemTemplate =  new QStandardItem(nameCurrent);
        itemTemplate->setData(getTemplateModel()->getVisibleElements()[i], Qt::UserRole + 1);
        itemTemplate->setEditable(false);
        itemTemplate->setData(getTemplateModel()->getVisibleElements()[i].toMap().value(hint), Qt::ToolTipRole);
        itemTarget = addHeadAndDependingItemsInTarget(getTemplateModel()->getVisibleElements()[i]);

        itemList.append(itemTemplate);
        itemList.append(itemTarget);
        getTableModel()->appendRow(itemList);
    }
    this->resizeColumnsToContents();
    this->show();
}

void CorrelationModelNoFunction::fillInTable(QVariantMap mapTable)
{
    clearCorrelationTable();
    qDebug();
    int N = mapTable.value(tempList).toList().count();
    QList<QStandardItem*> itemList;
    QString itemNames = "";
    for(int i = 0; i < N; ++i)
    {
        itemList.clear();
        itemNames = mapTable.value(tempList).toList().at(i).toMap().value(name).toString();
        QStandardItem *item1 = new QStandardItem(itemNames);
        item1->setData(mapTable.value(tempList).toList().at(i), Qt::UserRole + 1);
        item1->setData(mapTable.value(tempList).toList().at(i).toMap().value(hint), Qt::ToolTipRole);
        item1->setEditable(false);
        itemNames = mapTable.value(targList).toList().at(i).toMap().value(itemNamesInForm).toStringList().at(0);
        QStandardItem *item2 = new QStandardItem(itemNames);
        item2->setData(mapTable.value(targList).toList().at(i), Qt::UserRole + 1);
        item2->setData(mapTable.value(targList).toList().at(i).toMap().value(hint), Qt::ToolTipRole);
        item2->setEditable(false);
        itemList.append(item1);
        itemList.append(item2);
        getTableModel()->appendRow(itemList);
    }
    this->resizeColumnsToContents();
}

QStandardItem* CorrelationModelNoFunction::addHeadAndDependingItemsInTarget(const QVariant &elemFromTemplate)
{

   QStandardItem *item;
   QString nameItem;

   QVariantMap corrDataForTable = QVariantMap();// Data to add in additional correlation table
   QStringList itemNames = QStringList(); //"itemNames"
   //QStringList targetIds = QStringList(); //"targetIds"
   QStringList corrValues = QStringList(); //"corrValues"
   QStringList codes = QStringList(); //"codeValues"
   QString typeName = elemFromTemplate.toMap().value(type).toString(); //"typeName"
   QString templateName = elemFromTemplate.toMap().value(name).toString();//"templateName"
   QStringList dependIds = QStringList();//"dependIds" - dopy

   QVariantList dependList = elemFromTemplate.toMap().value(dependFields).toList();

   itemNames.append(elemFromTemplate.toMap().value(targetName).toString());
   corrValues.append(elemFromTemplate.toMap().value(correlationValue).toString());
   codes.append(elemFromTemplate.toMap().value(targetDataForConvert).toString());
   dependIds.append(QString(" "));
   foreach(QVariant dependElement, dependList)
   {
       itemNames.append(dependElement.toMap().value(targetName).toString());
       corrValues.append(dependElement.toMap().value(correlationValue).toString());
       codes.append(dependElement.toMap().value(targetDataForConvert).toString());
       dependIds.append(dependElement.toMap().value(dependId).toString());
   }

   if(itemNames.count() > 0)
   {
       nameItem = itemNames.at(0);
   }

   corrDataForTable.insert(itemNamesInForm, itemNames);
   corrDataForTable.insert(corrValuesInForm, corrValues);
   corrDataForTable.insert(codesInForm, codes);
   corrDataForTable.insert(typeNameInForm, typeName);
   corrDataForTable.insert(dependIdsInForm, dependIds);
   corrDataForTable.insert(templateNameInForm, templateName);

   // Нужно устанавливать номер!!!!
   item = new QStandardItem(nameItem);
   item->setData(corrDataForTable, Qt::UserRole + 1);
   QVariant foundTarget = findTargetDescriptionByName(nameItem);
   if (!foundTarget.isNull())
   {
       item->setData(foundTarget.toMap().value(hint), Qt::ToolTipRole);
   }
   item->setEditable(false);

   return item;
}

int CorrelationModelNoFunction::findTargetIndexByTargetName(const QString &targetString)
{
    int targetCount = getTargetModel()->getVisibleElements().count();

    for (int i = 0; i< targetCount; i++)
    {
        qDebug()<<getTargetModel()->getVisibleElements()[i].toMap().value(name).toString();
        if(getTargetModel()->getVisibleElements()[i].toMap().value(name).toString() == targetString)
        {
            return i;
        }
    }
    return -1;
}

QVariant CorrelationModelNoFunction::findTemplateByTargetName(const QString &tname)
{
    int count = getTableModel()->rowCount();
    for (int i = 0; i < count; i++)
    {
        qDebug()<<getTableModel()->item(i, iTemplate)->data(Qt::UserRole + 1).toMap().value(id).toString();
        if (getTableModel()->item(i, iTemplate)->data(Qt::UserRole + 1).toMap().value(id).toString() == tname)
        {
            if(getTableModel()->item(i, iTarget) != NULL )
            {
                return getTableModel()->item(i, iTarget)->data(Qt::UserRole + 1);
            }
            else
            {
                return QVariant();
            }
        }
    }
    return QVariant();
}

QVariantList CorrelationModelNoFunction::targetToTemplate()
{
    qDebug() << " Real start convering....";
    QVariantList retData;
    QVariantList convertedData = QVariantList();
    QVariantList dataForConverting = getTargetModel()->getElementsWithData();
    QVariantList templateDescrList = getTemplateModel()->getVisibleElements();

    int cRow = 0;
    QVariantMap mapToAddingInResult = QVariantMap();
    QVariantMap mapForOneElementData = QVariantMap();

    QVariantList dataToAppend = QVariantList();

    for (int i = 0; i < dataForConverting.count(); i++)
    {
        convertedData = dataForConverting[i].toMap().value(rapid).toList();//наше обычное описание но с полем dvalue(данные)
        mapToAddingInResult.insert(id, i);
        dataToAppend.clear();

        foreach(QVariant curr, templateDescrList)
        {
            mapForOneElementData = curr.toMap();

            QVariant element = findItemInTableTemplate(curr, cRow);
            qDebug()<<"NNN "<<element;
            QString dvalString = processTargetData(convertedData, element);
            qDebug()<<"NNN "<<dvalString;
            mapForOneElementData.insert(dvalue,  dvalString);

            qDebug() << mapForOneElementData;

            dataToAppend.append(mapForOneElementData);
        }

        mapToAddingInResult.insert(rapid, dataToAppend);
        retData.append(mapToAddingInResult);
    }

    return retData;
}

QString CorrelationModelNoFunction::processTargetData(const QVariantList &convertedData,
                                                      const QVariant &elementFromCorrTable)
{
    //elementFromCorrTable is QVariantMap with each element is QStringList except templateName and typeName
     QString dataString("Empty");

     if(!elementFromCorrTable.isNull())
     {
         QString typeElement = elementFromCorrTable.toMap().value(typeNameInForm).toString();//av,dv,cb,et,ed
         QStringList itemList = elementFromCorrTable.toMap().value(itemNamesInForm).toStringList();
         QStringList corrVals = elementFromCorrTable.toMap().value(corrValuesInForm).toStringList();
         QStringList codesVals = elementFromCorrTable.toMap().value(codesInForm).toStringList();
         QStringList dependVals = elementFromCorrTable.toMap().value(dependIdsInForm).toStringList();

         int itemsCount = itemList.count();
         int corrsCount = corrVals.count();
         int codesCount = codesVals.count();
         int depdsCount = dependVals.count();

         bool isOneEntity = (itemsCount == 1) && (corrsCount == 1) && (codesCount == 1) && (depdsCount == 1);
         bool isEntityWithOthers = (itemsCount > 1) && (corrsCount > 1) && (codesCount > 1) && (depdsCount > 1);
         bool isCorrectRecord = (itemsCount == corrsCount) && (codesCount == depdsCount) && (itemsCount == codesCount);

         //here is a processing
         if(isCorrectRecord && isOneEntity)
         {
             //Take entire value
             QString targetIdName = itemList.at(0);
             qDebug()<< "targetId = "<<targetIdName;            
             QVariant foundedTarget = findTargetDataByName(targetIdName, convertedData);
             if(!foundedTarget.isNull())
             {
                dataString = foundedTarget.toMap().value(dvalue).toString();
                qDebug()<<"dataString = "<<dataString;
             }
             else
             {
                 qWarning()<<"Error in correlation table. See";
             }

         }
         else if(isCorrectRecord && isEntityWithOthers)
         {
             for(int i = 1; i < itemsCount; ++i)
             {
                QString codedData = codesVals[i];
                QString targetIdName = itemList[i];
                QString correlation = corrVals[i];
                QString dependIdStr = dependVals[i];

                //Check for depending fields
                if( codedData.isEmpty())
                {
                    if( !dependIdStr.isEmpty())
                    {
                        //If dependId exist. Get dataString
                        if(dependIdSetted(dependIdStr, convertedData))
                        {
                            dataString = setNewDataValueByType(typeElement, correlation, dataString);
                        }
                    }
                    continue;
                }

                //Find data element
                QVariant foundTarget = findTargetDataByName(targetIdName, convertedData);

                if (foundTarget.isNull())
                {
                    qWarning()<<"Error in correlation table. See";
                    continue;
                }

                if(codedData.startsWith(simpleCode))
                {
                   QString codedDataWithoutCode = codedData.section(simpleCode, 0, 0, QString::SectionSkipEmpty);
                   if( compareSimpleCodes(codedDataWithoutCode, foundTarget.toMap().value(dvalue).toString())  //Codes are equal
                       || ( codedDataWithoutCode.isEmpty() && dataString == "Empty" )) //Code is empty. Accept it
                   {
                       dataString = setNewDataValueByType(typeElement, correlation, dataString);
                   }
                }
                else if (codedData.startsWith(compactCode))
                {
                    QString codedDataWithoutCode = codedData.section(compactCode, 0, 0, QString::SectionSkipEmpty);
                    if( compareCompactCodes(codedDataWithoutCode, foundTarget.toMap().value(dvalue).toString()) )
                    {
                        dataString = setNewDataValueByType(typeElement, correlation, dataString);
                    }
                }
                else
                {
                    qWarning()<<" Incorrect code of elements";
                    continue;
                }
             }
         }

     }

     return dataString;
}

bool CorrelationModelNoFunction::dependIdSetted(const QString &dependId, const QVariantList &convertedData)
{
    //Нашли зависимый элемент в списке
    QVariant foundTarget = findTemplateByTargetName(dependId);
    qDebug()<<"Element <> "<< foundTarget;
    QString dataString = processTargetData(convertedData, foundTarget);

    return !(dataString == "Empty");
}


bool CorrelationModelNoFunction::compareSimpleCodes(const QString &codedDataWithoutCode,
                                                    const QString &dataFromTarget)
{
    int diff = codedDataWithoutCode.size() - dataFromTarget.size();
    QString targetData = dataFromTarget;
    QString codedData = codedDataWithoutCode;
    QString nullString = QString();
    if( diff > 0)
    {
        nullString.fill('0', diff);
        targetData = nullString + targetData;
    }
    else if( diff < 0)
    {
        nullString.fill('0', -diff);
        codedData = nullString + codedData;
    }

    return  (targetData == codedData);
}

bool CorrelationModelNoFunction::compareCompactCodes(const QString &codedDataWithoutCode,
                                                     const QString & datafromTarget)
{
    int decimalCount = 0;
    int equalDecCount = 0;
    int codeSize = codedDataWithoutCode.size();
    int dataSize = datafromTarget.size();

    if(codeSize != dataSize)
    {
        qWarning()<<" You're comparing incorrect values.";
        return false;
    }

    for(int i = 0; i < codeSize; ++i)
    {
        if(codedDataWithoutCode[i] != X_Eng)
        {
            decimalCount++;
            if(codedDataWithoutCode[i] == datafromTarget[i])
            {
                equalDecCount++;
            }
        }
    }
    if(equalDecCount > 0 && decimalCount == equalDecCount)
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString CorrelationModelNoFunction::setNewDataValueByType(const QString &typeElement,
                                                          const QString &correlationValue, const QString &dataString)
{
    if(dataString == "Empty")
    {
        return correlationValue;
    }
    else
    {
        if(typeElement == DV)
        {
            //Concat two values
            QString dataStringNew = dataString;
            int dataStringSize = dataString.count();
            for(int i = 0; i <  dataStringSize; ++i)
            {
                if(correlationValue[i] == one)
                {
                    dataStringNew[i] = correlationValue[i];
                }
            }
            qDebug()<<" new string : "<<dataStringNew;
            return dataStringNew;
        }
        else
        {
            return correlationValue;
        }
    }
}

QVariant CorrelationModelNoFunction::findTargetDataByName(const QString &targetTemplate, const QVariantList &convertedData)
{
    foreach(QVariant elementFromConvList, convertedData)
    {
        if(elementFromConvList.toMap().value(name) == targetTemplate)
        {
            return elementFromConvList;
        }
    }
    return QVariant();
}

QVariant CorrelationModelNoFunction::findTargetDescriptionByName(const QString &targetTemplate)
{
    QVariantList searchList = getTargetModel()->getVisibleElements();
    foreach(QVariant el, searchList)
    {
        if(el.toMap().value(name) == targetTemplate)
        {
            return el;
        }
    }
    return QVariant();
}
