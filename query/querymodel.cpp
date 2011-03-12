#include "querymodel.h"

querymodel::querymodel(CorrelationModel* acorrModel):
    corrModel(acorrModel)
{
    qDebug();
}

querymodel::querymodel(const querymodel& templateQueryModel):
    QObject(),
    corrModel(templateQueryModel.corrModel),
    iQueryRequestDesc(templateQueryModel.iQueryRequestDesc),
    iQueryRequestData(templateQueryModel.iQueryRequestData),
    iCreateTblRequests(templateQueryModel.iCreateTblRequests),
    isTarget(templateQueryModel.isTarget),
    createRequest(templateQueryModel.createRequest),
    updateRequest(templateQueryModel.updateRequest),
    insertRequest(templateQueryModel.insertRequest)
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
    //Fill in list for request
    //1 request = 1 item in iListData
    //Заполняется один раз
    fillingRequestList();
    emit this->makeRequestSignal();
}
void querymodel::makeCreateTableRequest()
{
    qDebug() << " Generating template for create table";
    if (createRequest.isEmpty())
    {
        createRequest += createTbl + space + nameTbl + lBracket + iQueryRequestDesc[0] + rBracket;
        iCreateTblRequests.clear();
        iCreateTblRequests.append(createRequest);
        QString altRequest;
        for (int i = 1; i < iQueryRequestDesc.count(); i++)
        {
            altRequest = alterTbl + space + nameTbl + space + addTbl + space +
                         iQueryRequestDesc[i];
            iCreateTblRequests.append(altRequest);
        }
    }

    //createRequest = "CREATE TABLE TBLNAME()" - запрос создаёт пустую табдицу
    //altRequest = "ALTER TABLE TBLNAME ADD(...)"
}
void  querymodel::makeUpdateTableRequest()
{
    qDebug() << " Not implemented yet";
}
void querymodel::makeInsertIntoListRequests()
{
    //в запросе только один элемент
    qDebug() << " Generating template for insert into";
    if (insertRequest.isEmpty())
    {
        insertRequest += insertTbl + space + nameTbl + lBracket;
        for (int i = 0; i < iQueryRequestData.count(); i++)
        {
            if (i == 0)
            {
                insertRequest += iQueryRequestData[i];
            }
            else
            {
                insertRequest += comma + iQueryRequestData[i];
            }
        }
        insertRequest += rBracket + space + valuesTbl + lBracket;
    }
    //insertRequest = "INSERT INTO TBLNAME(...) VALUE("
}
void querymodel::initlist()
{
    qDebug();
    QMap<QString, QVariant> oneMap;
    if (iQueryRequestDesc.isEmpty() && iQueryRequestData.isEmpty())
    {
        QVariantList reqList = corrModel->getTemplateModel()->getVisibleElements();
        for (int i = 0; i < reqList.count() ; i++)
        {
            //I know that cpDescribeList[i] is QMap object
            oneMap = reqList[i].toMap();
            iQueryRequestDesc.append(MapToStrDesc(oneMap));
            iQueryRequestData.append(MapToStrData(oneMap));
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
    return (map.value(id).toString() + space + CellTypeToStr(map.value(type).toString())/* + comma*/);//generic_id type for create taable
}
QString querymodel::MapToStrData(const QVariantMap& map)
{
    return (map.value(id).toString()/* + comma*/);
}

void querymodel::resetList()
{
    listOfRequests.clear();
    iQueryRequestDesc.clear();
    iQueryRequestData.clear();
    createRequest.clear();
    insertRequest.clear();
}

void querymodel::fillingRequestList()
{
    qDebug();
    //Более простая реализация
    //Сначала для темплейта
    listOfRequests.clear();
    QVariantList tTemplate = corrModel->getTemplateModel()->getElementsWithData();//Записали все дела из демо
    QVariantList tTemplateItem;
    QVariantList reqList = corrModel->getTemplateModel()->getVisibleElements();
    QString reqString;
    tTemplate.append(corrModel->targetToTemplate());// добавили переконвертированные дела из старой версии
    //qDebug()<<Q_FUNC_INFO<<tTemplate.at(1).toMap().value(rapid).toList().at(0).toMap().value(dvalue);
    for (int i = 0 ; i < tTemplate.count(); i++)
    {
        //TODO как представляется iListData
        reqString = insertRequest;
        tTemplateItem = tTemplate[i].toMap().value(rapid).toList();//Кажись так
        for (int j = 0; j < reqList.count(); j++)
        {
            //Нужно найти для каждого reqList такой же элемент в tTemplateItem
            //Если он есть, то добавить значение - нет, добавить пустое место
            QVariant searchTmpl = reqList[j].toMap().value(id);//Значение ID
            QString foundValue = findByID(tTemplateItem, searchTmpl).toString();
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
        listOfRequests.append(reqString);
    }
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

