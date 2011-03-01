#include "correlationmodelnofunction.h"

CorrelationModelNoFunction::CorrelationModelNoFunction(QWidget* parent, QModelDescribing* templ, QModelDescribing* target):
        CorrelationModel(parent, templ, target)
{
    setupTableModel(getTableModel());
}

CorrelationModelNoFunction::CorrelationModelNoFunction(QWidget *parent, QModelDescribing *templ, QModelDescribing *target, QStandardItemModel *iTableModel):
        CorrelationModel(parent, templ, target, iTableModel)
{
    setupTableModel(getTableModel());
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
    int iTemplateCount = getTemplateModel()->getVisibleElements().count();

    QString nameCurrent, nameTarget;
    QList<QStandardItem*> itemList;
    int i = 0;
    //TODO: move to foreach
    for (; i < iTemplateCount; i++)
    {
        itemList.clear();
        //name1 = iCurrentModel->getVisibleElements()[i].toMap().value(name).toString();
        QString targetValue = getTemplateModel()->getVisibleElements()[i].toMap().value(targetId).toString();
        qDebug()<<" Template by Ids => "<<getTemplateModel()->getVisibleElements()[i].toMap().value(id).toString();
        QStandardItem *itemCurrent;
        QStandardItem *itemTarget;

        nameCurrent = getTemplateModel()->getVisibleElements()[i].toMap().value(name).toString();
        itemCurrent =  new QStandardItem(nameCurrent);
        itemCurrent->setData(getTemplateModel()->getVisibleElements()[i], Qt::UserRole + 1);
        itemCurrent->setEditable(false);

        if( !targetValue.isEmpty() )
        {
           int index = findTargetIndexByTargetValue(targetValue);
           if (index != -1)
           {
                itemTarget = addHeadAndDependingItemsInTarget(getTemplateModel()->getVisibleElements()[i]);
           }
           else
           {
               qWarning()<<"Incorrect correlation. Please check description file";
               itemTarget = NULL;
           }
        }
        else
        {
            qWarning()<<"Empty correlation. Please add information in description file";
            itemTarget = NULL;
        }
        itemList.append(itemCurrent);
        itemList.append(itemTarget);
        getTableModel()->appendRow(itemList);
    }
    this->resizeColumnsToContents();
    this->show();
}

//Изменить поведения меняльщика
//первый щелчок - первый элемент - открывается новая форма - таблица с текущими значениями, где щелчками можно поменять или убрать значение
QStandardItem* CorrelationModelNoFunction::addHeadAndDependingItemsInTarget(const QVariant &elemFromTemplate)
{
   QStandardItem *item;
   QString nameItem;
   QStringList itemNames = QStringList();
   QStringList targetIds = QStringList();
   QVariantList dataToList = QVariantList();

   QVariantList dependList = elemFromTemplate.toMap().value(dependFields).toList();
   QString firstTName = elemFromTemplate.toMap().value(targetId).toString();
   dataToList.append(findTargetDescriptionById(firstTName));
   itemNames.append(findTargetDescriptionById(firstTName).toMap().value(name).toString());
   targetIds.append(firstTName);

   foreach(QVariant dependElement, dependList)
   {
       QString targIdName = dependElement.toMap().value(targetId).toString();
       if (targIdName.isEmpty())
       {
           itemNames.append(QString(" "));
       }
       else
       {
            targetIds.append(targIdName);
            QVariant foundedTarget = findTargetDescriptionById(targIdName);
            if(!foundedTarget.isNull())
            {
                 QString simpleName = foundedTarget.toMap().value(name).toString();
                 dataToList.append(foundedTarget);
                 itemNames.append(simpleName);
            }
            else
            {
                qWarning()<<"Strange error";
            }
       }
   }
   if(itemNames.count() > 0)
   {
       nameItem = itemNames.takeAt(0);
   }
   if(!itemNames.isEmpty())
   {
       nameItem += " { [" + itemNames.takeAt(0) + "]";
   }
   foreach(QString name, itemNames)
   {
       if(!name.isEmpty())
       {
           nameItem += ", [" + name + "]";
       }
   }
   if(!itemNames.isEmpty())
   {
       nameItem += " }";
   }
   //nameItem should be look like "id1 {[id2], [id3], [id1], [ ] ...}
   // Нужно устанавливать номер!!!!
   item = new QStandardItem(nameItem);
   item->setData(dataToList, Qt::UserRole + 1);
   item->setEditable(false);

   return item;
}

int CorrelationModelNoFunction::findTargetIndexByTargetValue(const QString &targetString)
{
    int targetCount = getTargetModel()->getVisibleElements().count();
    for (int i = 0; i< targetCount; i++)
    {
        if(getTargetModel()->getVisibleElements()[i].toMap().value(id).toString() == targetString)
        {
            return i;
        }
    }
    return -1;
}

//!!!!!!!Нужно устанавливать номер!!!!rework
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
            QString dvalString = processTargetData(convertedData, element, templateDescrList);
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

QVariant CorrelationModelNoFunction::findItemInTableTemplate(const QVariant& search, int& row)
{
    int count = getTableModel()->rowCount();
    for (int i = 0; i < count; i++)
    {
        if (getTableModel()->item(i, iTemplate)->data(Qt::UserRole + 1) == search)
        {
            row = i;
            return getTableModel()->item(i, iTemplate)->data(Qt::UserRole + 1);
        }
    }
    return QVariant();
}

QString CorrelationModelNoFunction::processTargetData(const QVariantList &convertedData,
                                                      const QVariant &elementFromCorrTable,
                                                      const QVariantList &templateDescrList)
{
     QString dataString("");

     if(!elementFromCorrTable.isNull())
     {
         QVariantList dependList = elementFromCorrTable.toMap().value(dependFields).toList();
         QString typeElement = elementFromCorrTable.toMap().value(type).toString();//av,dv,cb,et,ed
         //here is a processing
         if( dependList.isEmpty() )
         {
             //Take entire value
             QString targetIdName = elementFromCorrTable.toMap().value(targetId).toString();
             qDebug()<< "targetId = "<<targetIdName;
             QVariant foundedTarget = findTargetDataById(targetIdName, convertedData);
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
         else
         {
             foreach(QVariant dependElement, dependList)
             {
                QString codedData = dependElement.toMap().value(targetDataForConvert).toString();
                QString targetIdName = dependElement.toMap().value(targetId).toString();
                QString correlation = dependElement.toMap().value(correlationValue).toString();
                QString dependIdStr = dependElement.toMap().value(dependId).toString();

                QVariant foundTarget = findTargetDataById(targetIdName, convertedData);

                if (foundTarget.isNull())
                {
                    qWarning()<<"Error in correlation table. See";
                    continue;
                }

                if( codedData.isEmpty())
                {
                    if( !dependIdStr.isEmpty())
                    {
                        //If dependId exist. Get dataString
                        if(dependIdSetted(dependIdStr, convertedData, templateDescrList))
                        {
                            dataString = setNewDataValueByType(typeElement, correlation, dataString);
                        }
                    }
                    continue;
                }

                if(codedData.startsWith(simpleCode))
                {
                    QString codedDataWithoutCode = codedData.section(simpleCode, 0, 0, QString::SectionSkipEmpty);
                   if( codedDataWithoutCode == foundTarget.toMap().value(dvalue).toString())
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

bool CorrelationModelNoFunction::dependIdSetted(const QString &dependId, const QVariantList &convertedData,
                                                const QVariantList &templateDescrList)
{
    //Нашли зависимый элемент в списке
    QVariant foundTarget = findTargetDataById(dependId, templateDescrList);
    QString dataString = processTargetData(convertedData, foundTarget, templateDescrList);

    return !dataString.isEmpty();
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
    if(dataString.isEmpty())
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

QVariant CorrelationModelNoFunction::findTargetDataById(const QString &targetTemplate, const QVariantList &convertedData)
{
    foreach(QVariant elementFromConvList, convertedData)
    {
        if(elementFromConvList.toMap().value(id) == targetTemplate)
        {
            return elementFromConvList;
        }
    }
    return QVariant();
}

QVariant CorrelationModelNoFunction::findTargetDescriptionById(const QString &targetTemplate)
{
    QVariantList searchList = getTargetModel()->getVisibleElements();
    foreach(QVariant el, searchList)
    {
        if(el.toMap().value(id) == targetTemplate)
        {
            return el;
        }
    }
    return QVariant();
}
