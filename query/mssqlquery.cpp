#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "mssqlquery.h"

mssqlquery::mssqlquery(QObject* parent, querymodel* model) :
    QObject(parent), queryModel(model)
{
}
void mssqlquery::run()
{
    //qDebug() << "Count requests = " << queryModel->getRequestList().count();
    qDebug() << "Requests = " << queryModel->getRequestList();
    iListofRequests.append(queryModel->getRequestList());
    createRequestList = queryModel->getCreateTable();
    removeRequestList = queryModel->getRemoveTable();

    db = QSqlDatabase::addDatabase("QODBC", "MainDBConnection");
    qDebug() << "ODBC driver valid?" << db.isValid();
    db.setDatabaseName(connectString.arg(driver, server, criminalDB, trusted));
    //Пробуем открыть нашу базу данных
    if (db.open())
    {
        // Заполняем таблицы данными
        if (!makeRequests())
        {
            db.close();
            return;
        }
    }
    else
    {
        // Возможно, нашей БД нет, коннектимся к master
        QSqlDatabase dbNew = QSqlDatabase::addDatabase("QODBC");
        dbNew.setDatabaseName(connectString.arg(driver, server, masterDB, trusted));
        if (dbNew.open())
        {
            //создаём нашу БД
            if (!createDB())
            {
                dbNew.close();
                return;
            }
            dbNew.close();
            //Коннектимся к нашей созданной БД
            db.setDatabaseName(connectString.arg(driver, server, criminalDB, trusted));
            if (db.open())
            {
                if (!makeRequests())
                {
                    db.close();
                    return;
                }
            }
            else
            {
                qDebug() << db.lastError().text();
                emit complete(1, db.lastError().text());
                return;
            }
        }
        else
        {
            //Ошибка не можем приконнектиться к мастеру
            qDebug() << dbNew.lastError().text();
            emit complete(1, dbNew.lastError().text());
            return;
        }
    }
    db.close();
    emit complete(0, tr("SUCCESS"));
}

bool mssqlquery::execRequest(const QString& requestString)
{
    db.transaction();
    if (db.lastError().type() != QSqlError::NoError)
    {
        qDebug() << "Error: " << db.lastError().text();
        db.rollback();
        emit complete(1, db.lastError().text());
        return false;
    }
    db.exec(requestString);
    db.commit();
    return true;
}

bool mssqlquery::checkAllTablesInDB(const QStringList& tables)
{
    bool ret = false;
    foreach(QString name, tblNames)
    {
        if (!tables.contains(name))
        {
            ret = false;
            break;
        }
        ret = true;
    }
    return ret;
}

bool mssqlquery::makeRequests()
{
    //Create table or add data in table
    qDebug() << "Tables in database";
    QStringList tables =  db.tables();
    bool isAllTablesInDB = checkAllTablesInDB(tables);
    if (!isAllTablesInDB)
    {
        foreach(QString name, tblNames)
        {
            if (tables.contains(name))
            {
                if(!removeRequestList.value(name).toString().isEmpty())
                {
                    if (!execRequest(removeRequestList.value(name).toString()))
                    {
                        return false;
                    }
                }
            }
            if (!createRequestList.value(name).toString().isEmpty() &&
                createRequestList.value(name).toString().contains(name + underline))
            {
                if (!execRequest(createRequestList.value(name).toString()))
                {
                    return false;
                }
            }
        }
    }
    //DemoTblName
    qDebug() << "::" << "Adding data to DB";
    foreach(QString request, iListofRequests)
    {
        if (!execRequest(request))
        {
            return false;
        }
    }
    return true;
}

bool mssqlquery::createDB()
{
    return execRequest(createDBReq);
}
