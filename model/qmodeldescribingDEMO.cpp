#include "qmodeldescribingDEMO.h"

//------------------------------------------------------------------------------
//-------------------QModelDescribingDemo--------------------------------------
//start----------------------------------------------------------------------------
QModelDescribingDemo::QModelDescribingDemo(QObject* parent):
    QModelDescribing(parent)
{

}

QVariantMap QModelDescribingDemo::fillOneElement(const QStringList& capturedText)
{
    QVariantMap element;
    element.insert(id, elementName + capturedText.at(1));
    element.insert(level, capturedText.at(2));
    element.insert(type, capturedText.at(3));
    element.insert(name, capturedText.at(5));
    element.insert(repeat, capturedText.at(6));
    return element;
}

bool QModelDescribingDemo::checkFileStructure(QTextStream* fileStream)
{
    qDebug() << "Not implemented yet. Accept all files";
    Q_UNUSED(fileStream);
    bool accept = true;

    return accept;
}

bool QModelDescribingDemo::isValidStringInDescriptionFileToAdd(const QMap<QString, QVariant> &checkMap)
{
    bool isNull = ((checkMap.value(id) == NULL) || (checkMap.value(level) == NULL) ||
                   (checkMap.value(name) == NULL) || (checkMap.value(type) == NULL) ||
                   (checkMap.value(repeat) == NULL));
    if (!isNull)
        return ((checkMap.value(type).toString() == AV) || (checkMap.value(type).toString() == DV) || (checkMap.value(type).toString() == RM) ||
                (checkMap.value(type).toString() == CB) || (checkMap.value(type).toString() == ED) || (checkMap.value(type).toString() == ET) ||
                (checkMap.value(type).toString() == ME)) ;
    return false;
}

void QModelDescribingDemo::addingLoadedDataInVisibleElementsWithData(QTextStream* fileStream)
{
    qDebug() << " Start";
    QVariantList oneRecord;
    QVariantMap oneData;
    int i = 0;
    int count = 0;
    QVariantList dataStructure = initDataStructure();
    while (!fileStream->atEnd())
    {
        QString line = fileStream->readLine();
        oneRecord = processLineInDataFile(line, dataStructure); //Тут только сразу весь мап
        //Первые три строки отбрасываем
        if (!oneRecord.isEmpty() && i >= 3)
        {
            oneData.insert(numb, count++);
            oneData.insert(rapid, oneRecord);
            VisibleElementWithData.append(oneData);
        }
        i++;
    }
    //iListData Element
    //QMap = number, value Value и будет записываться в базу данных
    qDebug() << " End. iListData count" << VisibleElementWithData.count();
}
QVariantList QModelDescribingDemo::processLineInDataFile(const QString& line, const QVariantList& DataStructure)
{
    int k = 0;
    QVariantList retVar;
    QVariantMap oneDataMap;
    for (int i = 0; i < DataStructure.count(); i++)
    {
        oneDataMap = DataStructure[i].toMap();
        oneDataMap.insert(dvalue, readSymbolsFromString(line, k));
        if (!turn(line, k, 1))
        {
            qWarning() << " String isn't valid";
            return QVariantList();
        }
        retVar.append(oneDataMap);
    }
    if (isValidStringInDataFileToAdd(retVar))
    {
        return retVar;
    }
    return QVariantList();
}
bool     QModelDescribingDemo::isValidStringInDataFileToAdd(const QVariantList& dataStructure)
{
    for (int i = 0; i < dataStructure.count(); i++)
    {
        if (!dataStructure[i].toMap().contains(dvalue))
            return false;
    }
    return true;
}

QVariantList QModelDescribingDemo::initDataStructure()
{
    QVariantList retVar;
    for (int i = 1; i < 97 ; i++)
    {
        retVar.append(findByIdInVisibleElements(elementName + QVariant(i).toString()));
    }
    return retVar;
}

bool QModelDescribingDemo::isValidElementsWithDataForParticularFile()
{
    return isValidElementsWithData();
}

bool QModelDescribingDemo::checkFileFileStructureData(QTextStream* fileStream)
{
    Q_UNUSED(fileStream);
    return true;
}

//end----------------------------------------------------------------------------
//-------------------QModelDescribingDemo--------------------------------------
