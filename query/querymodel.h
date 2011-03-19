#ifndef QUERYMODEL_H
#define QUERYMODEL_H

#include <QRunnable>

#include "correlationmodel.h"

//About class: QModelDescribing => QStringList
//me,dv,av,ed,cb,et,rm

class querymodel: public QObject, public QRunnable
{
        Q_OBJECT

    protected:
        QStringList listOAllfRequests;//список запросов к базе данных

        void resetList();//очистить список запросов к базе данных
        //bool convert();
        CorrelationModel* corrModel;
        QStringList templateForCreateRequests;//Данные для create table
        QStringList templateForInsertRequests;//Данные для insert into

        QVariantMap createTblRequests;//Реквесты для создания таблиц
        QVariantMap removeTblRequests; //Реквесты для удаления таблиц
        QVariantMap insertRequests;//Реквесты для вставки данных
        QVariantMap updateRequests;//Реквесты для обновления таблицы

        void makeCreateTableRequest();
        void makeRemoveTableRequest();
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
        querymodel(const querymodel& templateQueryModel);
        virtual void run();
        void makeRequest();
        QStringList getRequestDesc() const
        {
            return templateForCreateRequests;
        }
        QStringList getRequestData() const
        {
            return templateForInsertRequests;
        }
        QStringList getRequestList() const
        {
            return listOAllfRequests;
        }
        QVariantMap getCreateTable() const
        {
            return createTblRequests;
        }
        QVariantMap getRemoveTable() const
        {
            return removeTblRequests;
        }

    signals:
        void makeRequestSignal();
    private:
        CellType iCellType;
        //Fill in lists
        void setCellType(const QString& type);
        void setCellType(CellType type)
        {
            iCellType = type;
        }
        CellType getCellType()
        {
            return iCellType;
        }
        QString CellTypeToStr();
        QString CellTypeToStr(const QString& type);
        QString MapToStrDesc(const QVariantMap& map);
        QString MapToStrData(const QVariantMap& map);
        QString requestDataFromList(const QStringList &queryList, const QString  &templ);
        QString fillOneRequest(const QVariantList &templateItem, const QVariantList &visElements);
        QString chooseRequestString(const QString &temp);
};
#endif // QUERYMODEL_H
