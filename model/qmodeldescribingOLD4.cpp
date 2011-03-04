#include "qmodeldescribingOLD4.h"
//------------------------------------------------------ ----------------------
//-------------------QModelDescribingOld4--------------------------------------
//start-----------------------------------------------------------------------
QModelDescribingOld4::QModelDescribingOld4(QObject* parent):
    QModelDescribing(parent)
 {

}

bool QModelDescribingOld4::checkFileStructure(QTextStream* fileStream)
{
    qDebug() << "Not implemented yet. Accept all files";
    Q_UNUSED(fileStream);
    bool accept = true;

    return accept;
}

QVariantMap QModelDescribingOld4::fillOneElement(const QStringList & capturedText)
{
   QVariantMap element;
   element.insert(id, elementName + capturedText.at(1));
   element.insert(level, capturedText.at(2));
   element.insert(type, capturedText.at(3));
   element.insert(name, capturedText.at(5));
   return element;
}

void QModelDescribingOld4::preparingDataStructureBeforeFilling()
{

    //TODO rework it!!!
    qDebug() << "  ===START===   ";
    QVariantList oneRec;
    QVariantMap oneMap;
    QVariantMap tmpMap;
    QVariant uid;
    int count = 0;
        int i = 0;
    int  pos;
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
        if (count != 5)
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
void QModelDescribingOld4::addingLoadedDataInVisibleElementsWithData(QTextStream* fileStream)
    {
    qDebug() << " Start";
    QVariantList oneRecord;
    QVariantMap oneData;
    int i = 0;
    //Инициализируем данные в соответствии с таблицей
    QVariantList dataStructure = initDataStructure();
    while (!fileStream->atEnd())
    {
        QString line = fileStream->readLine();
        if (i < 2)
        {
            i++;
            continue;
        }
        oneRecord = processLineInDataFile(line, dataStructure); //Тут только сразу весь мап
        //Первые три строки отбрасываем
        if (!oneRecord.isEmpty())
        {
            oneData.insert(id, getIdByElementName(elementName, oneRecord));
            oneData.insert(rapid, oneRecord);
            VisibleElementsWithDataForParticularFile.append(oneData);
        }
    }
    //iListData Element
    //QMap = number, value Value и будет записываться в базу данных
    qDebug() << " End. iListData count" << VisibleElementsWithDataForParticularFile.count();
}
int QModelDescribingOld4::setSeekofLine(const QString& statName)
{
    int offset = 0;
    if (statName == generic)
    {
        offset = 18;
    }
    else if (statName == figurant)
    {
        offset = 11;
    }
     else if (statName == locus)
    {
         offset = 13;
    }
    else if (statName == weapon)
    {
        offset = 6;
    }

    return offset;
}

QVariantList QModelDescribingOld4::processLineInDataFile(const QString& line, const QVariantList& DataStructure)
{
    //TODO regenerate data files. URGENT
    int offset = setSeekofLine(elementName);
    QStringList listofData = line.split(';');
    QVariantMap oneDataMap;
    QVariantList retVar;
    int j = 0;
    qDebug() << " check data structure: a = " << listofData.count() - offset << " ; b = " << DataStructure.count();
    for (int i = offset; listofData.count(); i++)
    {
        //TODO check it
        if (j >= DataStructure.count())
        {
            qWarning() << "File string corrupted";
            return retVar;
        }
        oneDataMap = DataStructure[j].toMap();
        oneDataMap.insert(dvalue, listofData[i]);
        if (isVisibleElement(DataStructure[j]))
        {
            retVar.append(oneDataMap);
        }
            j++;
    }
    if (isValidStringInDataFileToAdd(retVar))
    {
        return retVar;
    }
    return QVariantList();
}
bool QModelDescribingOld4::isValidStringInDataFileToAdd(const QVariantList& dataStructure)
{
    for (int i = 0; i < dataStructure.count(); i++)
    {
        if (!dataStructure[i].toMap().contains(dvalue))
            return false;
    }
    return true;
}

void QModelDescribingOld4::setElementNameByDataFile(const QString& filename)
{
    if (filename.contains("__S.txt"))
    {
        elementName = generic;
    }
    else if (filename.contains("_C_"))
    {
        elementName = locus;
    }
    else if (filename.contains("_FP_"))
    {
        elementName = figurant;
    }
    else if (filename.contains("_O_"))
    {
        elementName = weapon;
    }
    else if (filename.contains("_FG_"))
    {
        elementName = figurant;
    }
}

bool QModelDescribingOld4::isValidElementsWithDataForParticularFile()
{
    return VisibleElementsWithDataForParticularFile.count() > 0;
}

QVariant QModelDescribingOld4::getIdByElementName(const QString& statName, const  QVariantList& oneRecord)
{
    qDebug() << "get id by statName";
    QVariant retVar;
    int offset = 0;
    if (statName == generic)
    {
        offset = 1;
    }
    else if (statName == figurant)
    {
        offset = 0;
    }
    else if (statName == locus)
    {
        offset     = 4;
    }
    else if (statName == weapon)
    {
        offset = 4;
    }
    retVar = oneRecord.at(offset).toMap().value(dvalue);
    qDebug() << retVar;
    return retVar;
}

bool QModelDescribingOld4::checkFileFileStructureData(QTextStream* /*fileStream*/)
{
    return true;
}

//end------------------------------------------------------------------------------
//-------------------QModelDescribingOld4--------------------------------------
