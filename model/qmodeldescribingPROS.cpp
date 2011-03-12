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
    //Strange actions
    bool accept = true;
    isProcessLine = 1;
    return accept;
}

QVariantList QModelDescribingPros::getElementsFromText(QTextStream* fileStream)
{
    qDebug();
    QVariantList elements;
    QString text = fileStream->readAll();
    QStringList textSplitted = text.split(QRegExp("\\n"));
    /**
      F5.TXT
         Р '  В '     Г '       N '   Np '      NPOT '  9 '      10 '  Соц '  Должн. '   14 '   14 '   15 '  16 '   18 '  18.1 '  18.2 '  19 '  19 '   20 '   20 '   20 '   20 '  Фамилия             '   Имя           '  Отчество            ' Дата рожд
      */
    QString lineFromFile = textSplitted.at(2);//you see line above
    QStringList lineSplitted = (lineFromFile.remove(" ")).split('\'', QString::SkipEmptyParts);
    QVariantMap elementForAdding;
    elementForAdding.insert(level, QVariant(1));
    elementForAdding.insert(type, "");
    int count = 0;
    foreach(QString nameKey, lineSplitted)
    {
        elementForAdding.insert(id, elementName + QString::number(count));
        // Установить уникальные имена для файлов из прокуратуры
        elementForAdding.insert(name, additionToNames + setNameWithoutRepeat(nameKey, elements));
        elementForAdding.insert(hint, additionToNames + setNameWithoutRepeat(nameKey, elements));
        elements.append(elementForAdding);
        count++;
    }
    return elements;
}

QString QModelDescribingPros::setNameWithoutRepeat(const QString& namekey, const QVariantList& elements)
{
    QString retStr = namekey;
    int amountElements = elements.count();
    int count = 0;
    foreach(QVariant el, elements)
    {
        if (el.toMap().value(name).toString().section(additionToNames, 1, 1) == retStr)
        {
            //Element exist. Need to add _i
            int value = 0;
            for (int i = count; i < amountElements; ++i)
            {
                if (elements[i].toMap().value(name).toString().section(additionToNames, 1, 1).startsWith(retStr + underline))
                {
                    value++;
                }
            }
            retStr += underline + QString::number(value);
            break;
        }
        count++;
    }
    return retStr;
}

void QModelDescribingPros::setAdditionsToNamesByFile(const QString& filename)
{
    if (filename.endsWith("F1.TXT", Qt::CaseSensitive))
    {
        //Not implemented
        additionToNames = "";
    }
    else if (filename.endsWith("F2.TXT", Qt::CaseSensitive))
    {
        //Not implemented
        additionToNames = "";
    }
    else if (filename.endsWith("F12.TXT", Qt::CaseSensitive))
    {
        //Not implemented
        additionToNames = "";
    }
    else if (filename.endsWith("F5.TXT", Qt::CaseSensitive))
    {
        additionToNames = F5_names;
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
        oneRecord = processLineInDataFile(line, dataStructure);
        if (!oneRecord.isEmpty() && i >= 4)
        {
            //getIdByStatName
            oneData.insert(id, getIdByStatName(elementName, oneRecord));
            oneData.insert(rapid, oneRecord);
            VisibleElementsWithDataForParticularFile.append(oneData);
        }
        i++;
    }
    //iListData Element
    //QMap = number, value Value Рё Р±СѓРґРµС‚ Р·Р°РїРёСЃС‹РІР°С‚СЊСЃСЏ РІ Р±Р°Р·Сѓ РґР°РЅРЅС‹С…
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
        /*
        if (count != 4)
        {
            qWarning() << "Data files is not valid";
            continue;
        }
        */
        oneMap.insert(numb, i++);
        oneMap.insert(rapid, oneRec);
        VisibleElementWithData.append(oneMap);
    }
    qDebug() << "  ===END===   ";

}

bool QModelDescribingPros::isValidElementsWithDataForParticularFile()
{
    return (VisibleElementsWithDataForParticularFile.count() > 0);
}

bool QModelDescribingPros::checkFileFileStructureData(QTextStream* /*fileStream*/)
{
    return true;
}

//-------------------QModelDescribingPros--- -----------------------------------
