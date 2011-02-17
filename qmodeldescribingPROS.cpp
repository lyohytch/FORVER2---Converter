#include "qmodeldescribingPROS.h"

//----------- --------QModelDescribingPros--------------------------------------
QModelDescribingPros::QModelDescribingPros(QObject* parent):
    QModelDescribing(parent)
{
    isProcessLine = 1;
}


bool QModelDescribingPros::checkFileStructure(QTextStream* fileStream)
{
    qDebug() << "Not implemented yet. Accept all files";
    Q_UNUSED(fileStream);
    bool accept = true;
    isProcessLine = 1;
    return accept;
}
/*
QMap<QString, QVariant>* QModelDescribingPros::processLineInDescriptionFile(const QString& line)
{
    qDebug();
    QMap<QString, QVariant> *retMap = new QMap<QString, QVariant>; //QMap["prosDesc", tempList]
    QMap<QString, QVariant> tempRet;
    QVariantList tempList; // QList [QMap<QString, QVariant>]
    QStringList ids = line.split(';', QString::SkipEmptyParts);
    if (!ids.isEmpty() && ids.count() > 1 && isProcessLine)
    {
        //Complete DESCRIBING LIST
        int encounter = 0;
        tempRet.insert(level, QVariant(1));
        tempRet.insert(type, "");
        foreach(QString idval, ids)
        {
            idval.remove(" ");
            tempRet.insert(id, elementName + encounter);
            tempRet.insert(name, idval);
            tempList.append(tempRet);
            encounter++;
        }
        retMap->insert(prosDesc, tempList);
        isProcessLine = 0;
        return retMap;
    }
    return NULL;
}
*/
void QModelDescribingPros::addNextElementsToList(QMap<QString, QVariant> & oneRec)
{
    int i = 0;
    QVariantList tmpList = oneRec.value(prosDesc).toList();
    if (!tmpList.isEmpty())
    {
        foreach(QVariant oneElem, tmpList)
        {
            oneElem.toMap().insert(numb, i++);
            ElementsFromDescriptionFiles.append(oneElem.toMap());
        }
    }
}

void QModelDescribingPros::addingLoadedDataInVisibleElementsWithData(QTextStream* fileStream)
{
    qDebug() << " Start";
    QVariantList oneRecord;
    QVariantMap oneData;
    int i = 0;
    QVariantList dataStructure = initDataStructure();
    while (!fileStream->atEnd())
    {
        QString line = fileStream->readLine();
        oneRecord = processLineInDataFile(line, dataStructure); //Тут только сразу весь мап
        //Первые три строки отбрасываем
        if (!oneRecord.isEmpty() && i >= 5)
        {
            //getIdByStatName
            oneData.insert(id, getIdByStatName(elementName, oneRecord));
            oneData.insert(rapid, oneRecord);
            VisibleElementsWithDataForParticularFile.append(oneData);
        }
        i++;
    }
    //iListData Element
    //QMap = number, value Value и будет записываться в базу данных
    qDebug() << " End. iListData count" << VisibleElementsWithDataForParticularFile.count();
}

bool QModelDescribingPros::isValidStringInDataFileToAdd(const QVariantList& vlist)
{
    return (vlist.count() > 1);
}

QVariantList QModelDescribingPros::processLineInDataFile(const QString& line, const QVariantList& DataStructure)
{
    QStringList listofData = line.split(';');
    QVariantMap oneDataMap;
    QVariantList retVar;
    int j = 0;
    qDebug() << " check data structure: a = " << listofData.count() << " ; b = " << DataStructure.count();
    foreach(QString dataEl, listofData)
    {
        //TODO check it
        if (j >= DataStructure.count())
        {
            qWarning() << "File string corrupted";
            return retVar;
        }
        //TODO remove unneccessary spaces
        QString dataElR = removeSpaces(dataEl);
        oneDataMap = DataStructure[j].toMap();
        oneDataMap.insert(dvalue, dataElR);
        if (isVisibleElement(DataStructure[j]))
        {
            retVar.append(oneDataMap);
        }
        j++;
    }

    if (!isValidStringInDataFileToAdd(retVar))
    {
        return QVariantList();
    }
     return retVar;
}

void QModelDescribingPros::setElementNameByDataFile(const QString& filename)
{
    //TODO implement it = REAL=> FAKE names
    if (filename.endsWith("F1.TXT", Qt::CaseSensitive))
    {
        elementName = generic;
    }
    else if (filename.endsWith("F2.TXT", Qt::CaseSensitive))
    {
        elementName = figurant;
    }
    else if (filename.endsWith("F12.TXT", Qt::CaseSensitive))
    {
        elementName = weapon;
    }
    else if (filename.endsWith("F5.TXT", Qt::CaseSensitive))
    {
        elementName = locus;
    }
}

QVariant QModelDescribingPros::getIdByStatName(const QString& statName, const  QVariantList& oneRecord)
{
    qDebug() << "get id by statName";
    QVariant retVar;
    int offset = 0;
    if (statName == generic)
    {
        offset = 3;
    }
    else if (statName == figurant)
    {
        offset = 4;
    }
    else if (statName == locus)
    {
        offset = 3;
    }
    else if (statName == weapon)
    {
        offset = 3;
    }
    retVar = oneRecord.at(offset).toMap().value(dvalue);
    qDebug() << retVar;
    return retVar;
}

QString QModelDescribingPros::removeSpaces(const QString& ex)
{
    QString ret = ex;
    QStringList exList = ret.split(' ', QString::SkipEmptyParts);
    int i = 0;
    foreach(QString elem, exList)
    {
        if (i == 0)
        {
            ret = elem;
        }
        else
        {
            ret += " " + elem;
        }
        i++;
    }
    return ret;
}


void QModelDescribingPros::preparingDataStructureBeforeFilling()
{

    //TODO rework it!!!
    qDebug() << "  ===START===   ";
    QVariantList oneRec;
    QVariantMap oneMap;
    QVariantMap tmpMap;
    QVariant uid;
    int count = 0;
    int i = 0;
    int pos;
    while (!VisibleElementsWithDataForParticularFile.isEmpty())
    {
        oneRec.clear();
        tmpMap = VisibleElementsWithDataForParticularFile.takeAt(0).toMap();
        oneRec.append(tmpMap.value(rapid).toList());
        //TODO rework uid
        uid = tmpMap.value(id);
        count = 1;
        while (findByUIdInVisibleElements(uid, pos))
        {
            tmpMap = VisibleElementsWithDataForParticularFile.takeAt(pos).toMap();
            oneRec.append(tmpMap.value(rapid).toList());
            count++;
        }
        //Если у нас некорректное число файлов
        if (count != 4)
        {
            qWarning() << "Data files is not valid";
            continue;
        }
        oneMap.insert(numb, i++);
        oneMap.insert(rapid, oneRec);
        VisibleElementWithData.append(oneMap);
    }
    qDebug() << "  ===END===   ";

}

bool QModelDescribingPros::isValidElementsWithDataForParticularFile()
{
    return (VisibleElementsWithDataForParticularFile.count() > 0 );
}

bool QModelDescribingPros::checkFileFileStructureData(QTextStream* /*fileStream*/)
{
    return true;
}

//-------------------QModelDescribingPros--- -----------------------------------
