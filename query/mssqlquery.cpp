#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>

#include "mssqlquery.h"
#include "constants.h"
#include "constants_query.h"

mssqlquery::mssqlquery(QObject* parent, querymodel* model) :
    QObject(parent), queryModel(model)
{
}
void mssqlquery::run()
{
    //qDebug() << "Count requests = " << queryModel->getRequestList().count();
    //Reading settings from ini file
    QSettings sqlSettings(applicationIni, QSettings::IniFormat);
    serverName = sqlSettings.value(sqlServerSettings + slash + srvName, server).toString();
    dbName = sqlSettings.value(sqlServerSettings + slash + dtbsName, criminalDB).toString();
    uidName = sqlSettings.value(sqlServerSettings + slash + userName, saUID).toString();
    pwdUid = sqlSettings.value(sqlServerSettings + slash + pwdUser, saPWD).toString();

    qDebug() << "Requests = " << queryModel->getRequestList();
    iListofRequests.append(queryModel->getRequestList());
    createRequestList = queryModel->getCreateTable();
    removeRequestList = queryModel->getRemoveTable();

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "MainDBConnection");
    qDebug() << "ODBC driver valid?" << db.isValid();
    db.setDatabaseName(connectString.arg(driver, serverName, dbName, uidName, pwdUid));
    //Пробуем открыть нашу базу данных
    if (db.open())
    {
        // Заполняем таблицы данными
        if (!makeRequests(db))
        {
            db.close();
            return;
        }
    }
    else
    {
        // Возможно, нашей БД нет, коннектимся к master
        db.setDatabaseName(connectString.arg(driver, serverName, masterDB, uidName, pwdUid));
        if (db.open())
        {
            //создаём нашу БД
            db.exec(createDBReq.arg(dbName));
            db.close();
            //Коннектимся к нашей созданной БД
            db.setDatabaseName(connectString.arg(driver, serverName, dbName, uidName, pwdUid));
            if (db.open())
            {
                if (!makeRequests(db))
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
            qDebug() << db.lastError().text();
            emit complete(1, db.lastError().text());
            return;
        }
    }
    db.close();
    emit complete(0, tr("SUCCESS"));
}

bool mssqlquery::execRequest(QSqlDatabase &db, const QString& requestString)
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

bool mssqlquery::makeRequests(QSqlDatabase &db)
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
                    if (!execRequest(db, removeRequestList.value(name).toString()))
                    {
                        return false;
                    }
                }
            }
            if (!createRequestList.value(name).toString().isEmpty() &&
                createRequestList.value(name).toString().contains(name + underline))
            {
                if (!execRequest(db, createRequestList.value(name).toString()))
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
        if (!execRequest(db, request))
        {
            return false;
        }
    }
    return true;
}
