#ifndef MSSQLQUERY_H
#define MSSQLQUERY_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QRunnable>
#include <QMutex>
#include <QWaitCondition>

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
        void completeRequest()
        {
            qDebug() << Q_FUNC_INFO;
        };
        QSqlDatabase db;
        querymodel* queryModel;
        QStringList iListofRequests;//Лист запросов для добавления в базу данных
        QStringList createRequestList;// Лист запросов для создания таблицы базы данных
        virtual void run();
    signals:
        void complete(int aError, QString errStr);
    public slots:
    private:
        QMutex mutex;
};

#endif // MSSQLQUERY_H
