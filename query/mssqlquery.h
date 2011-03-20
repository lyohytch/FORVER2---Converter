#ifndef MSSQLQUERY_H
#define MSSQLQUERY_H

#include <QSqlDatabase>
#include <QRunnable>

#include "querymodel.h"
//! mssqlquery запись сформированных SQL-запросов в базу данных
/**
 * Класс для  записи сформированных SQL-запросов классом querymodel в базу данных. Класс работает асинхронно, по завершении
 * работы выводится сообщение о результате добавления в базу данных. При отсутсвии нужно базы данных, она создаётся. При
 * отсутствии нужных таблиц, они пересоздаются.
 * \note Для установления собственных настроек нужно добавить в файле настроек "converter.ini" следующие строки \n
 * [SQL_Server_Settings] \n
 * Server=localhost\\SQLEXPRESS \n
 * Database=CrimDB \n
 * User=sa \n
 * Password=NNSU \n
 * Также следует учесть, что устанавливаемый здесь пользователь должен иметь соответствующие права для создания базы данных
 * и добавления таблиц, а также добавление в них данных.
 * \sa querymodel
 */
class mssqlquery : public QObject, public QRunnable
{
        Q_OBJECT
    public:
        /**
          * Конструктор по-умолчанию
          * \param parent указатель на родительский объект
          * \param model указатель на querymodel
          */
        mssqlquery(QObject* parent = 0, querymodel* model = 0);
        /**
          * Преобразованные SQL-строки для добавления в базу данных
          */
        querymodel* queryModel;
        /**
          * Список SQL-запросов для добавления в базу данных
          */
        QStringList iListofRequests;
        /**
          * Список SQL-запросов для создания новых таблиц в базе данных
          */
        QVariantMap createRequestList;
        /**
          * Список SQL-запросов для удаления таблиц из базы данных
          */
        QVariantMap removeRequestList;
        /**
          * Асинхронный запуск процедуры добавления данных в базу данных
          */
        virtual void run();
    signals:
        /**
          * Сигнал о завершении опереации добавления данных в базу данных
          * \param aError номер ошибки. 0 - отсутсвие ошибки
          * \param errStr сообщение об ошибке или "SUCCESS"
          */
        void complete(int aError, QString errStr);
    private:
        bool execRequest(QSqlDatabase& db, const QString& requestString);
        bool checkAllTablesInDB(const QStringList& tables);
        bool makeRequests(QSqlDatabase& db);
        //String read from ini file
        QString serverName;
        QString dbName;
        QString uidName;
        QString pwdUid;

};

#endif // MSSQLQUERY_H
