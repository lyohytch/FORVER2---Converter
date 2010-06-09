#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include "mssqlquery.h"

mssqlquery::mssqlquery(QObject *parent, querymodel *model) :
    QObject(parent), queryModel(model)
{
}
void mssqlquery::run()
{
    qDebug()<<Q_FUNC_INFO<<"Count requests = "<<queryModel->getRequestList().count();
    qDebug()<<Q_FUNC_INFO<<"Requests = "<<queryModel->getRequestList();
    iListofRequests.append(queryModel->getRequestList());
    createRequestList = queryModel->getCreateTable();
    mutex.lock();
    //Создать базу данных, если нет её - добавить данные
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC3","MainDBConnection");
    qDebug() << "ODBC driver valid?" << db.isValid();
    db.setDatabaseName("Driver={SQL Server};Server=localhost\\SQLEXPRESS;Database=CrimDemoDB;Trusted_Connection=yes;");
    if(db.open())
    {
       //Create table or add data in table
       qDebug()<<"Tables in database";
       QStringList tables =  db.tables();
       bool isTableinDB = false;
       foreach(QString table, tables)
       {
           qDebug()<<"\t"<<table;
           if(table == nameTbl)
           {
               isTableinDB = true;
               break;
           }
       }
       if(!isTableinDB)
       {
           //Create table
           //TODO проверить поддерживаются ли транзакции
           db.transaction();
           for(int i = 0; i <createRequestList.count(); i++)
           {
               if ( db.lastError().type() != QSqlError::NoError )
               {
                   qDebug() << "Error: " << db.lastError().text();
                   db.rollback();
                   emit complete(1,db.lastError().text());
                   return;
               }
               try
               {
                   db.exec(createRequestList[i]);
               }
               catch(QSqlError e)
               {
                   qDebug()<<Q_FUNC_INFO<<" err was occured";
                   db.rollback();
                   emit complete(1,e.text());
                   return;
               }
           }
           db.commit();

       }
       //DemoTblName
       qDebug()<<Q_FUNC_INFO<<"::"<<"Adding data to DB";
       foreach(QString request, iListofRequests)
       {
           db.transaction();
           if ( db.lastError().type() != QSqlError::NoError )
           {
               qDebug() << "Error: " << db.lastError().text();
               db.rollback();
               emit complete(1,db.lastError().text());
               return;
           }
           db.exec(request);
           db.commit();
       }

    }
    else
    {
        qDebug()<<db.lastError().text();
        emit complete(1,db.lastError().text());
        return;
    }
    db.close();
    mutex.unlock();
    emit complete(0,"SUCCESS");
}
