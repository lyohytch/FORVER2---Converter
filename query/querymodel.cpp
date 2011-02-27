#include "querymodel.h"

querymodel::querymodel(CorrelationModel* acorrModel):
    corrModel(acorrModel)
{
    qDebug();
}

querymodel::querymodel(const querymodel &templateQueryModel):
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

//� ��� ���� ���������
//id(for example, generic_133) , name, type(me,dv,av,ed,cb,et,rm)
void querymodel::makeRequest()
{
    qDebug();
    initlist();//����������� ���� ���
    //Making templates create table, update table and insert into requests
    makeCreateTableRequest();//����������� ���� ���
    makeUpdateTableRequest();//����������� ���� ���
    makeInsertIntoListRequests();//����������� ���� ���
    //Fill in list for request
    //1 request = 1 item in iListData
    //����������� ���� ���
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

    //createRequest = "CREATE TABLE TBLNAME()" - ������ ������ ������ �������
    //altRequest = "ALTER TABLE TBLNAME ADD(...)"
}
void  querymodel::makeUpdateTableRequest()
{
    qDebug() << " Not implemented yet";
}
void querymodel::makeInsertIntoListRequests()
{
    //� ������� ������ ���� �������
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
        QVariantList reqList = corrModel->getCurrentModel()->getVisibleElements();
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
    //����� ������� ����������
    //������� ��� ���������
    listOfRequests.clear();
    QVariantList tTemplate = corrModel->getCurrentModel()->getElementsWithData();//�������� ��� ���� �� ����
    QVariantList tTemplateItem;
    QVariantList reqList = corrModel->getCurrentModel()->getVisibleElements();
    QString reqString;
    tTemplate.append(corrModel->targetToCurrent());// �������� �������������������� ���� �� ������ ������
    //qDebug()<<Q_FUNC_INFO<<tTemplate.at(1).toMap().value(rapid).toList().at(0).toMap().value(dvalue);
    for (int i = 0 ; i < tTemplate.count(); i++)
    {
        //TODO ��� �������������� iListData
        reqString = insertRequest;
        tTemplateItem = tTemplate[i].toMap().value(rapid).toList();//������ ���
        for (int j = 0; j < reqList.count(); j++)
        {
            //����� ����� ��� ������� reqList ����� �� ������� � tTemplateItem
            //���� �� ����, �� �������� �������� - ���, �������� ������ �����
            QVariant searchTmpl = reqList[j].toMap().value(id);//�������� ID
            QString foundValue = findByID(tTemplateItem, searchTmpl).toString();
            qDebug() << foundValue;
            if (j == 0)
            {
                reqString += quote + foundValue + quote;//������� �������� ���� dvalue � tTemplateItem
            }
            else
            {
                reqString += comma + quote + foundValue + quote;//������� �������� ���� dvalue � tTemplateItem
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
