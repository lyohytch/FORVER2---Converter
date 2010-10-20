#ifndef QUERYMODEL_H
#define QUERYMODEL_H
#include <QStringList>
#include <QRunnable>
#include <QObject>
#include "qmodeldescribing.h"
#include "correlationmodel.h"
#include "constants.h"
//About class: QModelDescribing => QStringList
//me,dv,av,ed,cb,et,rm

class querymodel: public QObject, public QRunnable
{
        Q_OBJECT

    protected:
        QStringList listOfRequests;//список запросов к базе данных

        void resetList();//очистить список запросов к базе данных
        //bool convert();
        CorrelationModel* corrModel;
        QStringList iQueryRequestDesc;//ƒанные дл€ create table
        QStringList iQueryRequestData;//ƒанные дл€ insert into
        QStringList iCreateTblRequests;
        bool isTarget;//True if it is target
        QString createRequest;
        QString updateRequest;
        QString insertRequest;
        void makeCreateTableRequest();
        void makeUpdateTableRequest();
        void makeInsertIntoListRequests();
        QVariant findByID(const QVariantList& list, const QVariant& searchTmpl);

        void fillingRequestList();
        void initlist();
    public:
        enum CellType
        {
            ed = 0,
            dv,
            av,
            me,
            et,
            cb = 5,
            rm,
            un
        };
        querymodel(CorrelationModel* acorrModel = 0);
        virtual void run();
        void makeRequest();
        QStringList getRequestDesc() const
        {
            return iQueryRequestDesc;
        };
        QStringList getRequestData() const
        {
            return iQueryRequestData;
        };
        QStringList getRequestList() const
        {
            return listOfRequests;
        };
        QStringList getCreateTable() const
        {
            return iCreateTblRequests;
        };

    signals:
        void makeRequestSignal();
    private:
        CellType iCellType;
        //Fill in lists
        void setCellType(const QString& type);
        void setCellType(CellType type)
        {
            iCellType = type;
        };
        CellType getCellType()
        {
            return iCellType;
        };
        QString CellTypeToStr();
        QString CellTypeToStr(const QString& type);
        QString MapToStrDesc(const QVariantMap& map);
        QString MapToStrData(const QVariantMap& map);
};



//////////////////////////////////////////////
//‘ормирование строк дл€ старой системы - 4 файла
/*
class querymodelOld4: public querymodel
{
public:
    querymodelOld4(QModelDescribing *amodel);
    enum CellType
    {
        ed = 0,
        dv,
        av,
        me,
        et,
        cb = 5,
        rm,
        un
    };
    bool checkModel();
protected:
    bool convert();
    void setCellType(const QString & type);
    void setCellType(CellType type) {iCellType = type;};
    CellType getCellType() { return iCellType;};
    QString CellTypeToStr();
    QString CellTypeToStr(const QString & type);
    QString MapToStrDesc(const QVariantMap &map);
    QString MapToStrData(const QVariantMap &map);
private:
    CellType iCellType;

};

// ‘ормирование строк дл€ демо версии
class querymodelDemo: public querymodel
{
public:
    querymodelDemo(QModelDescribing *amodel);
};

//формирование строк дл€ версии от прокуратуры
class querymodelPros: public querymodel
{
public:
    querymodelPros(QModelDescribing *amodel);
};*/
#endif // QUERYMODEL_H
