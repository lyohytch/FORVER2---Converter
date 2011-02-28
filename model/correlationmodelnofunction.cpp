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
        if( !targetValue.isEmpty() )
        {
           int index = findTargetIndexByTargetValue(targetValue);
           if (index != -1)
           {
                nameCurrent = getTemplateModel()->getVisibleElements()[i].toMap().value(name).toString();
                itemCurrent =  new QStandardItem(nameCurrent);
                itemCurrent->setData(getTemplateModel()->getVisibleElements()[i], Qt::UserRole + 1);
                itemCurrent->setEditable(false);

                //TODO: add other data from dependList
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
    this->resizeColumnsToContents();
    this->show();
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
QVariantList CorrelationModelNoFunction::targetToTemplate()
{
    qDebug() << " Real start convering....";
    QVariantList retData;
    QVariantList convertedData = QVariantList();
    QVariantList dataForConverting = getTargetModel()->getElementsWithData();
    QVariantList currDescrList = getTemplateModel()->getVisibleElements();

    int cRow = 0;
    QVariantMap mapToAddingInResult = QVariantMap();
    QVariantMap mapForOneElementData = QVariantMap();

    QVariantList dataToAppend = QVariantList();

    for (int i = 0; i < dataForConverting.count(); i++)
    {
        convertedData = dataForConverting[i].toMap().value(rapid).toList();//наше обычное описание но с полем dvalue(данные)
        mapToAddingInResult.insert(id, i);
        dataToAppend.clear();

        foreach(QVariant curr, currDescrList)
        {
            mapForOneElementData = curr.toMap();

            mapForOneElementData.insert(dvalue, processTargetData(convertedData,
                                                                  findItemInTableTemplate(curr, cRow)) );

            qDebug() << mapForOneElementData;

            dataToAppend.append(mapForOneElementData);
        }

        mapToAddingInResult.insert(rapid, dataToAppend);
        retData.append(mapToAddingInResult);
    }

    return retData;
}

QString CorrelationModelNoFunction::processTargetData(const QVariantList &convertedData, const QVariant &elementFromCorrTable)
{
     QString dataString = "";

     if(!elementFromCorrTable.isNull())
     {
         QVariantList dependList = elementFromCorrTable.toMap().value(dependFields).toList();
         QString typeElement = elementFromCorrTable.toMap().value(type).toString();//av,dv,cb,et,ed
         //here is a processing
         if( dependList.isEmpty() )
         {
             //Take entire value
             QString targetName = elementFromCorrTable.toMap().value(targetName).toString();
             qDebug()<< "targetId = "<<targetName;
             QVariant foundedTarget = findTargetDataById(targetName, convertedData);
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
                QString targetName = dependElement.toMap().value(targetName).toString();
                QString correlationValue = dependElement.toMap().value(correlationValue).toString();
                QString dependIdStr = dependElement.toMap().value(dependId).toString();

                QVariant foundTarget = findTargetDataById(targetName, convertedData);
                if( codedData.isEmpty())
                {
                    qWarning()<<"Error in correlation table. Incorrect element in depedFields";
                    if( !dependIdStr.isEmpty())
                    {
                        //If dependId exist. Get dataString
                        if(dependIdSetted(dependIdStr, convertedData))
                        {
                            dataString = setNewDataValueByType(typeElement, correlationValue, dataString);
                        }
                    }
                    continue;
                }
                if (foundTarget.isNull())
                {
                    qWarning()<<"Error in correlation table. See";
                    continue;
                }

                if(codedData.startsWith(simpleCode))
                {
                    QString codedDataWithoutCode = QString(simpleCode+"%1").arg(codedData);
                    qDebug()<<codedDataWithoutCode;
                   if( codedDataWithoutCode == foundTarget.toMap().value(dvalue).toString())
                   {
                       qDebug()<<"Codes are equal";
                       dataString = setNewDataValueByType(typeElement, correlationValue, dataString);
                       qDebug()<<"dataString = "<<dataString;
                   }
                }
                else if (codedData.startsWith(compactCode))
                {
                    QString codedDataWithoutCode = QString(compactCode+"%1").arg(codedData);
                    qDebug()<<codedDataWithoutCode;
                    if( compareCompactCodes(codedDataWithoutCode, foundTarget.toMap().value(dvalue).toString()) )
                    {
                        dataString = setNewDataValueByType(typeElement, correlationValue, dataString);
                        qDebug()<<"dataString = "<<dataString;
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
    QVariant foundTarget = findTargetDataById(dependId, convertedData);
    QString dataString = processTargetData(convertedData, foundTarget);

    return !dataString.isEmpty();
}

bool CorrelationModelNoFunction::compareCompactCodes(const QString &codedDataWithoutCode,
                                                     const QString & datafromTarget)
{
    int decimalCount = 0;
    int equalDecCount = 0;
    for(int i = 0; i < codedDataWithoutCode.count(); ++i)
    {
        if(codedDataWithoutCode[i] != X_Rus ||
           codedDataWithoutCode[i] != X_Eng)
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
            for(int i = 0; i < dataString.count() ; ++i)
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
