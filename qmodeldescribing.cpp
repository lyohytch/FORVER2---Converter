#include <QFile>

#include "qmodeldescribing.h"



QModelDescribing::QModelDescribing(QObject *parent):
        QStandardItemModel(parent)
{
}

QModelDescribing::QModelDescribing(const QModelDescribing &other):
        QStandardItemModel()
{
    iListDescribing = other.getListDescribing();
    isData = other.modelData();
}

QVariantList QModelDescribing::getListDescribing() const
{
    return iListDescribing;
}

QVariantList QModelDescribing::getListSignificant() const
{
    return iListSignificant;
}
QVariantList QModelDescribing::getListData() const
{
    return iListData;
}
void QModelDescribing::appendToList(const QString &filename)
{
    qDebug();
    //Open file
    QFile fileSource(filename);
    if ( fileSource.exists()) {
        qDebug()<<"File found. Try to take list";
        if (!fileSource.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCritical()<<"File "<<filename<<" can not be open.";
        } else {
            QTextStream fileStream(&fileSource);
            if ( checkFileStructure(&fileStream) ) {
                setStatNameByFile(filename);
                addingDataToList(&fileStream);
            } else {
                qWarning()<<"File structure is incorrect.";
            }
        }
    } else {
        qCritical()<<"File "<<filename<<" doesn't exist";
    }
}
void QModelDescribing::addingDataToList(QTextStream *fileStream)
{
    qDebug()<<" Start";
    QMap<QString, QVariant> *oneRecord;
    while (!fileStream->atEnd()) {
        QString line = fileStream->readLine();
        oneRecord = process_line(line);
        if (oneRecord != NULL) {
            moveOneRecordToList(*oneRecord);
            delete oneRecord;
        }
    }
    qDebug()<<" End";
}

void QModelDescribing::moveOneRecordToList(QMap<QString, QVariant> & oneRec)
{
    oneRec.insert(numb,iListDescribing.count());
    iListDescribing.append(oneRec);
}

void QModelDescribing::loadingData(const QString &filename)
{
    qDebug();
    //Open file
    QFile fileSource(filename);
    if ( fileSource.exists()) {
        qDebug()<<"File found. Try to take list";
        if (!fileSource.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCritical()<<"File "<<filename<<" can not be open.";
        } else {
            QTextStream fileStream(&fileSource);
            if ( checkFileFileStructureData(&fileStream) ) {
                setStatNameByFileData(filename);
                addingLoadedData(&fileStream);
            } else {
                qWarning()<<"File structure is incorrect.";
            }
        }
    } else {
        qCritical()<<"File "<<filename<<" doesn't exist";
    }
}

bool QModelDescribing::createModel()
{
    qDebug();
    //Заполнить значимые элементы
    fillSignificantList();
    //Здесь мы заполняем QStandartItemModel
    QStandardItem *rootItem = new QStandardItem("Model");
    //Fill in list QStandardItem
    setChildItem(iListDescribing,0,0,rootItem);
    //Append children of rootItem
    this->appendRow(rootItem);
    return true;
}

void QModelDescribing::setChildItem(const QVariantList &iList,int i,int levels,QStandardItem *parent)
{
    if (i >= iList.count()) {
        return;
    }
    QStandardItem *child = new QStandardItem(iList[i].toMap().value(name).toString());
    child->setEditable(false);
    child->setData(iList[i],Qt::UserRole + 1);
    if (isSignificant(iList[i])) {
        QFont font = child->font();
        font.setBold(true);
        child->setFont(font);
    }
    int levelcur = iList[i].toMap().value(level).toInt();
    int diff = levelcur  - levels;
    switch (diff) {
    case 0: { // Уровень совпадает. Делаем следующий элемент ребёнком родителя parent
        i++;
        parent->parent()->appendRow(child);
        setChildItem(iList,i,levelcur,child);
    }
    break;
    case 1: { //Уровень следующего на 1 больше. Делаем след элемент ребёнком parent
        i++;
        parent->appendRow(child);
        setChildItem(iList,i,levelcur,child);
    }
    break;
    case -1: { //Уровень след на 1 меньше. Делаем след эл ребёнком дедушки parent
        i++;
        parent->parent()->parent()->appendRow(child);
        setChildItem(iList,i,levelcur,child);
    }
    break;
    default: { //Уровень след меньше , чем на 1
        if (!child) {
            delete child;
        }
        setChildItem(iList,i,levels - 1,parent->parent());
    }
    break;
    }
}
bool QModelDescribing::isValidString(const QMap<QString,QVariant> &checkMap)
{
    bool isNull = ((checkMap.value(id)==NULL)||(checkMap.value(level)==NULL)||
                   (checkMap.value(name)==NULL)||(checkMap.value(type)==NULL));
    if (!isNull)
        return ((checkMap.value(type).toString() == AV)||(checkMap.value(type).toString() == DV)||(checkMap.value(type).toString() == RM)||
                (checkMap.value(type).toString() == CB)||(checkMap.value(type).toString() == ED)||(checkMap.value(type).toString() == ET)||
                (checkMap.value(type).toString() == ME)) ;
    return false;
}

QString QModelDescribing::readElement(const QString &line, int &k)
{
    QString Element = "";
    while (line[k] != '\t') {
        Element += line[k];
        k++;
        if (k >= line.count()) {
            qWarning()<<"Data out of range!!!";
            return NULL;
        }
    }
    return Element;

}
bool QModelDescribing::turn(const QString &line, int &k, int cTurn)
{
    int count = 0;
    while (count < cTurn) {
        if (k >= line.count()) {
            qWarning()<<"Data out of range!!!";
            return false;
        }
        if (line[k] == '\t') ++count;
        k++;
    }
    return true;
}

bool QModelDescribing::isValid()
{
    return iListSignificant.count() > 0;
}

bool QModelDescribing::isValidData()
{
    return iListData.count() > 0;
}

//Заполнить лист значимых элементов
void QModelDescribing::fillSignificantList()
{
    int count = iListDescribing.count();
    for (int i = 0; i<count; i++) {
        if (isSignificant(iListDescribing[i])) {
            iListSignificant.append(iListDescribing[i]);
        }
    }
}
bool QModelDescribing::isSignificant(const QVariant &value)
{
    return (value.toMap().value(type).toString() != RM);
}

QVariant QModelDescribing::findById(const QVariant &uid)
{
    for (int i = 0; i < iListSignificant.count() ; i++) {
        if (iListSignificant[i].toMap().value(id) == uid) {
            return iListSignificant[i];
        }
    }
    return QVariant();
}



void QModelDescribing::setStatNameByFile(const QString &filename)
{
    //Move to each class
    if (filename.endsWith("Sprav1.txt",Qt::CaseSensitive) ||
            filename.endsWith("sprav_d.txt",Qt::CaseSensitive) ||
            filename.endsWith("F1.TXT", Qt::CaseSensitive)) {
        statName = generic;
    } else if (filename.endsWith("Sprav2.txt",Qt::CaseSensitive) ||
               filename.endsWith("F2.TXT", Qt::CaseSensitive)) {
        statName = figurant;
    } else if (filename.endsWith("Sprav3.txt",Qt::CaseSensitive) ||
               filename.endsWith("F5.TXT", Qt::CaseSensitive)) {
        statName = locus;
    } else if (filename.endsWith("Sprav4.txt",Qt::CaseSensitive) ||
               filename.endsWith("F12.TXT", Qt::CaseSensitive)) {
        statName = weapon;
    } else {
        //Adding by default
        statName = "";
    }
}

void QModelDescribing::resetDataList()
{
    iListData.clear();
}

void QModelDescribing::resetSignList()
{
    iListSignificant.clear();
}

void QModelDescribing::resetDescList()
{
    iListDescribing.clear();
}

void QModelDescribing::resetAllList()
{
    this->clear();
    resetDescList();
    resetDataList();
    resetSignList();
}

bool QModelDescribing::foundByUId(const QVariant &uid, int &pos)
{
    int i = 0;
    foreach(QVariant oneRec,iListDataTemp) {
        if (oneRec.toMap().value(id) == uid) {
            pos = i;
            return true;
        }
        i++;
    }

    return false;
}

//----------------------------------------------------------------------------
//-------------------QModelDescribingOld4--------------------------------------
//start-----------------------------------------------------------------------
QModelDescribingOld4::QModelDescribingOld4(QObject *parent):
        QModelDescribing(parent)
{

}

bool QModelDescribingOld4::checkFileStructure(QTextStream *fileStream)
{
    qDebug()<<"Not implemented yet. Accept all files";
    Q_UNUSED(fileStream);
    bool accept = true;

    return accept;
}
QMap<QString, QVariant>* QModelDescribingOld4::process_line(const QString &line)
{
    int k = 0;
    QMap<QString,QVariant> *retMap = new QMap<QString,QVariant>;
    //Нужно определить ID
    retMap->insert(id,statName+readElement(line,k));
    //Берём уровень
    //Сдвиг
    if (!turn(line,k,1)) {
        qWarning()<<"String isn't valid";
        delete retMap;
        return NULL;
    }
    retMap->insert(level,readElement(line,k));
    //try to take m_asType
    if (!turn(line,k,5)) {
        qWarning()<<"String isn't valid";
        delete retMap;
        return NULL;
    }
    retMap->insert(type,readElement(line,k));
    //Line couldn't be added into map. Return NULL
    //Сдвиг для считывания имени
    if (!turn(line,k,4)) {
        qWarning()<<"String isn't valid";
        delete retMap;
        return NULL;
    }
    retMap->insert(name,readElement(line,k));
    if (!isValidString(*retMap)) {
        qWarning()<<"String isn't valid";
        delete retMap;
        return NULL;
    }
    return retMap;
}

void QModelDescribingOld4::dataPrepare()
{

    //TODO rework it!!!
    qDebug()<<"  ===START===   ";
    QVariantList oneRec;
    QVariantMap oneMap;
    QVariantMap tmpMap;
    QVariant uid;
    int count = 0;
    int i = 0;
    int pos;
    while (!iListDataTemp.isEmpty()) {
        oneRec.clear();
        tmpMap = iListDataTemp.takeAt(0).toMap();
        oneRec.append(tmpMap.value(rapid).toList());
        //TODO rework uid
        uid = tmpMap.value(id);
        count = 1;
        while (foundByUId(uid, pos)) {
            tmpMap = iListDataTemp.takeAt(pos).toMap();
            oneRec.append(tmpMap.value(rapid).toList());
            count++;
        }
        //Если у нас некорректное число файлов
        if (count != 5) {
            qWarning()<<"Data files is not valid";
            continue;
        }
        oneMap.insert(numb,i++);
        oneMap.insert(rapid,oneRec);
        iListData.append(oneMap);
    }
    qDebug()<<"  ===END===   ";

}
void QModelDescribingOld4::addingLoadedData(QTextStream *fileStream)
{
    qDebug()<<" Start";
    QVariantList oneRecord;
    QVariantMap oneData;
    int i = 0;
    //Инициализируем данные в соответствии с таблицей
    QVariantList dataStructure = initDataStructure();
    while (!fileStream->atEnd()) {
        QString line = fileStream->readLine();
        if (i<2) {
            i++;
            continue;
        }
        oneRecord = process_lineData(line,dataStructure);//Тут только сразу весь мап
        //Первые три строки отбрасываем
        if (!oneRecord.isEmpty()) {
            oneData.insert(id,getIdByStatName(statName,oneRecord));
            oneData.insert(rapid,oneRecord);
            iListDataTemp.append(oneData);
        }
    }
    //iListData Element
    //QMap = number, value Value и будет записываться в базу данных
    qDebug()<<" End. iListData count" <<iListDataTemp.count();
}
int QModelDescribingOld4::setSeekofLine(const QString & statName)
{
    int offset = 0;
    if (statName == generic ) {
        offset = 18;
    } else if (statName == figurant) {
        offset = 11;
    } else if (statName == locus) {
        offset = 13;
    } else if (statName == weapon) {
        offset = 6;
    }

    return offset;
}

QVariantList QModelDescribingOld4::process_lineData(const QString &line, const QVariantList &DataStructure)
{
    //TODO regenerate data files. URGENT
    int offset = setSeekofLine(statName);
    QStringList listofData = line.split(';');
    QVariantMap oneDataMap;
    QVariantList retVar;
    int j = 0;
    qDebug()<<" check data structure: a = "<<listofData.count() - offset<<" ; b = "<<DataStructure.count();
    for (int i = offset; listofData.count(); i++) {
        //TODO check it
        if (j >= DataStructure.count()) {
            qWarning()<<"File string corrupted";
            return retVar;
        }
        oneDataMap = DataStructure[j].toMap();
        oneDataMap.insert(dvalue,listofData[i]);
        if (isSignificant(DataStructure[j])) {
            retVar.append(oneDataMap);
        }
        j++;
    }
    if (isValidStringData(retVar)) {
        return retVar;
    }
    return QVariantList();
}
bool QModelDescribingOld4::isValidStringData(const QVariantList & dataStructure)
{
    for (int i = 0; i < dataStructure.count(); i++) {
        if (!dataStructure[i].toMap().contains(dvalue))
            return false;
    }
    return true;
}
QVariantList QModelDescribingOld4::initDataStructure()
{
    QVariantList retVar;
    for ( int i = 0; i < iListDescribing.count(); i++) {
        if (iListDescribing[i].toMap().value(id).toString().contains(statName)) {
            retVar.append(iListDescribing[i]);
        }
    }
    return retVar;
}

void QModelDescribingOld4::setStatNameByFileData(const QString &filename)
{
    if (filename.contains("__S.txt")) {
        statName = generic;
    } else if (filename.contains("_C_")) {
        statName = locus;
    } else if (filename.contains("_FP_")) {
        statName = figurant;
    } else if (filename.contains("_O_")) {
        statName = weapon;
    } else if (filename.contains("_FG_")) {
        statName = figurant;
    }
}

bool QModelDescribingOld4::isValidDataTemp()
{
    return iListDataTemp.count() > 0;
}

QVariant QModelDescribingOld4::getIdByStatName(const QString &statName,const  QVariantList &oneRecord)
{
    qDebug()<<"get id by statName";
    QVariant retVar;
    int offset = 0;
    if (statName == generic ) {
        offset = 1;
    } else if (statName == figurant) {
        offset = 0;
    } else if (statName == locus) {
        offset = 4;
    } else if (statName == weapon) {
        offset = 4;
    }
    retVar = oneRecord.at(offset).toMap().value(dvalue);
    qDebug()<<retVar;
    return retVar;
}

//end------------------------------------------------------------------------------
//-------------------QModelDescribingOld4--------------------------------------

//------------------------------------------------------------------------------
//-------------------QModelDescribingDemo--------------------------------------
//start----------------------------------------------------------------------------
QModelDescribingDemo::QModelDescribingDemo(QObject *parent):
        QModelDescribing(parent)
{

}

QMap<QString, QVariant>* QModelDescribingDemo::process_line(const QString &line)
{
    // qDebug()<<Q_FUNC_INFO<<"::"<<line;
    int k = 0;
    QMap<QString,QVariant> *retMap = new QMap<QString,QVariant>;
    //Нужно определить ID
    retMap->insert(id,statName+readElement(line,k));
    //Берём уровень
    //Сдвиг
    if (!turn(line,k,1)) {
        qWarning()<<"String isn't valid";
        delete retMap;
        return NULL;
    }
    retMap->insert(level,readElement(line,k));
    //try to take m_asType
    if (!turn(line,k,5)) {
        qWarning()<<"String isn't valid";
        delete retMap;
        return NULL;
    }
    retMap->insert(type,readElement(line,k));
    //Line couldn't be added into map. Return NULL
    //Сдвиг для считывания имени
    if (!turn(line,k,4)) {
        qWarning()<<"String isn't valid";
        delete retMap;
        return NULL;
    }
    retMap->insert(name,readElement(line,k));
    //Сдвиг для считывания дублирующего элемента
    if (!turn(line,k,3)) {
        qWarning()<<"String isn't valid";
        delete retMap;
        return NULL;
    }
    retMap->insert(repeat,readElement(line,k));
    if (!isValidString(*retMap)) {
        qWarning()<<"String isn't valid";
        delete retMap;
        return NULL;
    }


    return retMap;
}

bool QModelDescribingDemo::checkFileStructure(QTextStream *fileStream)
{
    qDebug()<<"Not implemented yet. Accept all files";
    Q_UNUSED(fileStream);
    bool accept = true;

    return accept;
}

bool QModelDescribingDemo::isValidString(const QMap<QString,QVariant> &checkMap)
{
    bool isNull = ((checkMap.value(id)==NULL)||(checkMap.value(level)==NULL)||
                   (checkMap.value(name)==NULL)||(checkMap.value(type)==NULL)||
                   (checkMap.value(repeat)==NULL));
    if (!isNull)
        return ((checkMap.value(type).toString() == AV)||(checkMap.value(type).toString() == DV)||(checkMap.value(type).toString() == RM)||
                (checkMap.value(type).toString() == CB)||(checkMap.value(type).toString() == ED)||(checkMap.value(type).toString() == ET)||
                (checkMap.value(type).toString() == ME)) ;
    return false;
}

void QModelDescribingDemo::addingLoadedData(QTextStream *fileStream)
{
    qDebug()<<" Start";
    QVariantList oneRecord;
    QVariantMap oneData;
    int i = 0;
    int count = 0;
    QVariantList dataStructure = initDataStructure();
    while (!fileStream->atEnd()) {
        QString line = fileStream->readLine();
        oneRecord = process_lineData(line,dataStructure);//Тут только сразу весь мап
        //Первые три строки отбрасываем
        if (!oneRecord.isEmpty() && i >=3 ) {
            oneData.insert(numb,count++);
            oneData.insert(rapid,oneRecord);
            iListData.append(oneData);
        }
        i++;
    }
    //iListData Element
    //QMap = number, value Value и будет записываться в базу данных
    qDebug()<<" End. iListData count" <<iListData.count();
}
QVariantList QModelDescribingDemo::process_lineData(const QString &line, const QVariantList &DataStructure)
{
    int k = 0;
    QVariantList retVar;
    QVariantMap oneDataMap;
    for (int i = 0; i < DataStructure.count(); i++) {
        oneDataMap = DataStructure[i].toMap();
        oneDataMap.insert(dvalue,readElement(line,k));
        if (!turn(line,k,1)) {
            qWarning()<<" String isn't valid";
            return QVariantList();
        }
        retVar.append(oneDataMap);
    }
    if (isValidStringData(retVar)) {
        return retVar;
    }
    return QVariantList();
}
bool QModelDescribingDemo::isValidStringData(const QVariantList & dataStructure)
{
    for (int i = 0; i < dataStructure.count(); i++) {
        if (!dataStructure[i].toMap().contains(dvalue))
            return false;
    }
    return true;
}

QVariantList QModelDescribingDemo::initDataStructure()
{
    QVariantList retVar;
    for ( int i = 1; i < 97 ; i++) {
        retVar.append(findById(statName + QVariant(i).toString()));
    }
    return retVar;
}

//end----------------------------------------------------------------------------
//-------------------QModelDescribingDemo--------------------------------------







//-------------------QModelDescribingPros--------------------------------------
QModelDescribingPros::QModelDescribingPros(QObject *parent):
        QModelDescribing(parent)
{
    isProcessLine = 1;
}


bool QModelDescribingPros::checkFileStructure(QTextStream *fileStream)
{
    qDebug()<<"Not implemented yet. Accept all files";
    Q_UNUSED(fileStream);
    bool accept = true;
    isProcessLine = 1;
    return accept;
}

QMap<QString, QVariant>* QModelDescribingPros::process_line(const QString &line)
{
    qDebug();
    QMap<QString,QVariant> *retMap = new QMap<QString,QVariant>; //QMap["prosDesc", tempList]
    QMap<QString,QVariant> tempRet;
    QVariantList tempList; // QList [QMap<QString, QVariant>]
    QStringList ids = line.split(';', QString::SkipEmptyParts);
    if (!ids.isEmpty() && ids.count() > 1 && isProcessLine) {
        //Complete DESCRIBING LIST
        int encounter = 0;
        tempRet.insert(level, QVariant(1));
        tempRet.insert(type, "");
        foreach(QString idval, ids) {
            idval.remove(" ");
            tempRet.insert(id,statName+encounter);
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

void QModelDescribingPros::moveOneRecordToList(QMap<QString, QVariant> & oneRec)
{
    int i = 0;
    QVariantList tmpList = oneRec.value(prosDesc).toList();
    if (!tmpList.isEmpty()) {
        foreach(QVariant oneElem, tmpList) {
            oneElem.toMap().insert(numb, i++);
            iListDescribing.append(oneElem.toMap());
        }
    }
}




void QModelDescribingPros::addingLoadedData(QTextStream *fileStream)
{
    qDebug()<<" Start";
    QVariantList oneRecord;
    QVariantMap oneData;
    int i = 0;
    QVariantList dataStructure = initDataStructure();
    while (!fileStream->atEnd()) {
        QString line = fileStream->readLine();
        oneRecord = process_lineData(line,dataStructure);//Тут только сразу весь мап
        //Первые три строки отбрасываем
        if (!oneRecord.isEmpty() && i >=5 ) {
            //getIdByStatName
            oneData.insert(id,getIdByStatName(statName, oneRecord));
            oneData.insert(rapid,oneRecord);
            iListDataTemp.append(oneData);
        }
        i++;
    }
    //iListData Element
    //QMap = number, value Value и будет записываться в базу данных
    qDebug()<<" End. iListData count" <<iListDataTemp.count();
}

bool QModelDescribingPros::isValidStringData(const QVariantList & vlist)
{
    return (vlist.count() > 1);
}

QVariantList QModelDescribingPros::process_lineData(const QString &line, const QVariantList &DataStructure)
{
    QStringList listofData = line.split(';');
    QVariantMap oneDataMap;
    QVariantList retVar;
    int j = 0;
    qDebug()<<" check data structure: a = "<<listofData.count()<<" ; b = "<<DataStructure.count();
    foreach(QString dataEl, listofData) {
        //TODO check it
        if (j >= DataStructure.count()) {
            qWarning()<<"File string corrupted";
            return retVar;
        }
        //TODO remove unneccessary spaces
        QString dataElR = removeSpaces(dataEl);
        oneDataMap = DataStructure[j].toMap();
        oneDataMap.insert(dvalue,dataElR);
        if (isSignificant(DataStructure[j])) {
            retVar.append(oneDataMap);
        }
        j++;
    }

    if ( !isValidStringData(retVar) ) {
        return QVariantList();
    }
    return retVar;
}

void QModelDescribingPros::setStatNameByFileData(const QString &filename)
{
    //TODO implement it = REAL=> FAKE names
    if (filename.endsWith("F1.TXT", Qt::CaseSensitive)) {
        statName = generic;
    } else if (filename.endsWith("F2.TXT", Qt::CaseSensitive)) {
        statName = figurant;
    } else if ( filename.endsWith("F12.TXT", Qt::CaseSensitive)) {
        statName = weapon;
    } else if ( filename.endsWith("F5.TXT", Qt::CaseSensitive)) {
        statName = locus;
    }
}

QVariantList QModelDescribingPros::initDataStructure()
{
    QVariantList retVar;
    for ( int i = 0; i < iListDescribing.count(); i++) {
        if (iListDescribing[i].toMap().value(id).toString().contains(statName)) {
            retVar.append(iListDescribing[i]);
        }
    }
    return retVar;
}

QVariant QModelDescribingPros::getIdByStatName(const QString &statName,const  QVariantList &oneRecord)
{
    qDebug()<<"get id by statName";
    QVariant retVar;
    int offset = 0;
    if (statName == generic ) {
        offset = 3;
    } else if (statName == figurant) {
        offset = 4;
    } else if (statName == locus) {
        offset = 3;
    } else if (statName == weapon) {
        offset = 3;
    }
    retVar = oneRecord.at(offset).toMap().value(dvalue);
    qDebug()<<retVar;
    return retVar;
}

QString QModelDescribingPros::removeSpaces(const QString &ex)
{
    QString ret = ex;
    QStringList exList = ret.split(' ', QString::SkipEmptyParts);
    int i = 0;
    foreach(QString elem, exList) {
        if (i == 0) {
            ret = elem;
        } else {
            ret += " " + elem;
        }
        i++;
    }
    return ret;
}

void QModelDescribingPros::dataPrepare()
{

    //TODO rework it!!!
    qDebug()<<"  ===START===   ";
    QVariantList oneRec;
    QVariantMap oneMap;
    QVariantMap tmpMap;
    QVariant uid;
    int count = 0;
    int i = 0;
    int pos;
    while (!iListDataTemp.isEmpty()) {
        oneRec.clear();
        tmpMap = iListDataTemp.takeAt(0).toMap();
        oneRec.append(tmpMap.value(rapid).toList());
        //TODO rework uid
        uid = tmpMap.value(id);
        count = 1;
        while (foundByUId(uid, pos)) {
            tmpMap = iListDataTemp.takeAt(pos).toMap();
            oneRec.append(tmpMap.value(rapid).toList());
            count++;
        }
        //Если у нас некорректное число файлов
        if (count != 4) {
            qWarning()<<"Data files is not valid";
            continue;
        }
        oneMap.insert(numb,i++);
        oneMap.insert(rapid,oneRec);
        iListData.append(oneMap);
    }
    qDebug()<<"  ===END===   ";

}

//-------------------QModelDescribingPros--------------------------------------
