#ifndef MSSQLQUERY_H
#define MSSQLQUERY_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QRunnable>

#include "qmodeldescribing.h"
#include "querymodel.h"
/**This class provide API for MS SQL Server
 * For example, connect, disconnect and other
 */
//Convert from StringLIst to request string and request to adding in db
class mssqlquery : public QObject, public QRunnable
{
        Q_OBJECT
    public:
        mssqlquery(QObject* parent = 0, querymodel* model = 0);
        QSqlDatabase db;
        querymodel* queryModel;
        QStringList iListofRequests;//Лист запросов для добавления в базу данных
        QVariantMap createRequestList;// Лист запросов для создания таблицы базы данных
        QVariantMap removeRequestList;//Лист запросов для удаления таблиц
        virtual void run();
    signals:
        void complete(int aError, QString errStr);
       private:
         bool execRequest(const QString & requestString);
         bool checkAllTablesInDB(const QStringList &tables);
         bool createDB();
         bool makeRequests();
};

#endif // MSSQLQUERY_H
