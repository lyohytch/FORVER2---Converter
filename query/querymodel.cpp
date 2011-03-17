#include "querymodel.h"

querymodel::querymodel(CorrelationModel* acorrModel):
    corrModel(acorrModel)
{
    qDebug();
}

querymodel::querymodel(const querymodel& templateQueryModel):
    QObject(),
    corrModel(templateQueryModel.corrModel),
    templateForCreateRequests(templateQueryModel.templateForCreateRequests),
    templateForInsertRequests(templateQueryModel.templateForInsertRequests),
    createTblRequests(templateQueryModel.createTblRequests)
{
    qDebug();
}

void querymodel::run()
{
    this->makeRequest();
}

//У нас есть структура
//id(for example, generic_133) , name, type(me,dv,av,ed,cb,et,rm)
void querymodel::makeRequest()
{
    qDebug();
    initlist();//выполниться один раз
    //Making templates create table, update table and insert into requests
    makeCreateTableRequest();//выполниться один раз
    makeUpdateTableRequest();//выполниться один раз
    makeInsertIntoListRequests();//выполниться один раз
    makeRemoveTableRequest();
    //Fill in list for request
    //1 request = 1 item in iListData
    //Заполняется один раз
    fillingRequestList();
    emit this->makeRequestSignal();
}
void querymodel::makeCreateTableRequest()
{
    qDebug() << " Generating template for create table";
    if ( createTblRequests.isEmpty() )
    {
        foreach(QString createTblReq, tblNames)
        {
           createTblRequests.insert(createTblReq, createTbl + space + createTblReq + lBracket + requestDataFromList(templateForCreateRequests, createTblReq).remove(-1,1) + rBracket);
        }
    }
    //createRequest = "CREATE TABLE TBLNAME()" - запрос создаёт пустую табдицу
    //altRequest = "ALTER TABLE TBLNAME ADD(...)"
}

void querymodel::makeRemoveTableRequest()
{
    qDebug()<< "Generate remove tables requests";
    if ( removeTblRequests.isEmpty())
    {
        foreach(QString remTbl, tblNames)
        {
            removeTblRequests.insert(remTbl, dropTbl + space + remTbl);
        }
    }
}

QString querymodel::requestDataFromList(const QStringList &queryList, const QString  &templ)
{
    QString retStr = "";
    foreach(QString el, queryList)
    {
        if (el.contains(templ))
        {
            retStr += el;
        }
    }
    return retStr;
}

void  querymodel::makeUpdateTableRequest()
{
    qDebug() << " Not implemented yet";
}

void querymodel::makeInsertIntoListRequests()
{
    //в запросе только один элемент
    qDebug() << " Generating template for insert into";
    if (insertRequests.isEmpty())
    {
        foreach(QString name, tblNames)
        {
            QString insertRequest = insertTbl + space + name + lBracket +
                                    requestDataFromList(templateForInsertRequests, name).remove(-1, 1) +
                                    rBracket + space + valuesTbl + lBracket;
            insertRequests.insert(name, insertRequest);
        }
    }
    //insertRequest = "INSERT INTO TBLNAME(...) VALUE("
}
void querymodel::initlist()
{
    qDebug();
    QMap<QString, QVariant> oneMap;
    if (templateForCreateRequests.isEmpty() && templateForInsertRequests.isEmpty())
    {
        QVariantList reqList = corrModel->getTemplateModel()->getVisibleElements();
        for (int i = 0; i < reqList.count() ; i++)
        {
            oneMap = reqList[i].toMap();
            templateForCreateRequests.append(MapToStrDesc(oneMap));
            templateForInsertRequests.append(MapToStrData(oneMap));
        }
    }

}

void querymodel::setCellType(const QString& type)
{
    //TODO check it
    (type == ME) ? (iCellType = querymodel::me) :
    (type == DV) ? (iCellType = querymodel::dv) :
    (type == AV) ? (iCellType = querymodel::av) :
    (type == ED) ? (iCellType = querymodel::ed) :
    (type == CB) ? (iCellType = querymodel::cb) :
    (type == ET) ? (iCellType = querymodel::et) :
    (type == RM) ? (iCellType = querymodel::rm) : (iCellType = querymodel::un);
}

QString querymodel::CellTypeToStr()
{
    //TODO implement it
    return VCHARMAX;
}

QString querymodel::CellTypeToStr(const QString& type)
{
    setCellType(type);
    return CellTypeToStr();
}

QString querymodel::MapToStrDesc(const QVariantMap& map)
{
    return (map.value(id).toString() + space + CellTypeToStr(map.value(type).toString()) + comma);//generic_id type for create taable
}
QString querymodel::MapToStrData(const QVariantMap& map)
{
    return (map.value(id).toString() + comma);
}

void querymodel::resetList()
{
    listOAllfRequests.clear();
    templateForCreateRequests.clear();
    templateForInsertRequests.clear();
}

void querymodel::fillingRequestList()
{
    qDebug();
    //Более простая реализация
    //Сначала для темплейта
    listOAllfRequests.clear();

    QVariantList tTemplate = corrModel->getTemplateModel()->getElementsWithData();//Записали все дела из демо
    QVariantList reqList = corrModel->getTemplateModel()->getVisibleElements();

    tTemplate.append(corrModel->targetToTemplate());// добавили переконвертированные дела из старой версии
    //qDebug()<<Q_FUNC_INFO<<tTemplate.at(1).toMap().value(rapid).toList().at(0).toMap().value(dvalue);
    for (int i = 0 ; i < tTemplate.count(); i++)
    {
        listOAllfRequests.append(fillOneRequest(tTemplate[i].toMap().value(rapid).toList(),
                                                reqList) );
    }
}

QString querymodel::fillOneRequest(const QVariantList &templateItem, const QVariantList &visElements)
{
    QString reqString;

    reqString = chooseRequestString(visElements[0].toMap().value(id).toString());
        //Кажись так
    for (int j = 0; j < visElements.count(); j++)
    {
        //Нужно найти для каждого reqList такой же элемент в tTemplateItem
        //Если он есть, то добавить значение - нет, добавить пустое место
        QVariant searchTmpl = visElements[j].toMap().value(id);//Значение ID
        QString foundValue = findByID(templateItem, searchTmpl).toString();
        qDebug() << foundValue;
        if (j == 0)
        {
            reqString += quote + foundValue + quote;//вывести значение поля dvalue в tTemplateItem
        }
        else
        {
            reqString += comma + quote + foundValue + quote;//вывести значение поля dvalue в tTemplateItem
        }
    }
    reqString += rBracket;
    return reqString;
}

QString querymodel::chooseRequestString(const QString &temp)
{
    foreach(QString name, tblNames)
    {
         if (temp.contains(name))
         {
             return insertRequests.value(name).toString();
         }
    }
    return QString();
}

QVariant querymodel::findByID(const QVariantList& list, const QVariant& searchTmpl)
{
    for (int i = 0; i < list.count(); i++)
    {
        if (list[i].toMap().value(id) == searchTmpl)
        {
            return list[i].toMap().value(dvalue);
        }
    }
    return QVariant();
}

