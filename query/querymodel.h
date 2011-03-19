#ifndef QUERYMODEL_H
#define QUERYMODEL_H

#include <QRunnable>

#include "correlationmodel.h"

//! querymodel класс преобразующий QModelDescribing в строки для SQL-запросов
/**
  * Класс для преобразования из QModelDescribing в строки для SQL-запросов. Запуск процедуры преобразования
  * асинхронный. Преобразованные строки передаются в mssqlquery для добавления в базу данных
  * \sa mssqlquery
  */

class querymodel: public QObject, public QRunnable
{
        Q_OBJECT
    public:
        /**
          * Тип элементов из файла описания
          */
        enum CellType
        {
            /** Элемент текстового ввода*/
            ed = 0,
            /** Элемент дистрибутивного ввода*/
            dv,
            /** Элемент альтернативного ввода*/
            av,
            /** Элемент многострочного текстового ввода*/
            me,
            /** Элемент текстового ввода*/
            et,
            /** Раскрывающийся список*/
            cb = 5,
            /** Рамка, не используется для ввода*/
            rm,
            /** Неопределённый тип*/
            un
        };
        /**
          * Конструктор по умолчанию
          * \param acorrModel указатель на корреляционную таблицу CorrelationModel
          */
        querymodel(CorrelationModel* acorrModel = 0);
        /**
          * Конструктор копирования
          * \param templateQueryModel другой экземпляр qquerymodel
          */
        querymodel(const querymodel& templateQueryModel);
        /**
          * Запуск асинхронной процедуры преобразования из QModelDescribing в строки SQL-запросов
          */
        virtual void run();
        /**
          * Возвращает список всех SQL-запросов для добавления в БД
          * \return Список всех SQL-запросов
          */
        QStringList getRequestList() const
        {
            return listOAllfRequests;
        }
        /**
          * Возвращает список всех SQL-запросов для создания новых таблиц в БД
          * \return Список SQL-запросов
          */
        QVariantMap getCreateTable() const
        {
            return createTblRequests;
        }
        /**
          * Возвращает список всех SQL-запросов для удаления таблиц из БД
          * \return Список SQL-запросов
          */
        QVariantMap getRemoveTable() const
        {
            return removeTblRequests;
        }
    signals:
        /**
          * Сигнализирует о завершении операции преобразовании из QModelDescribingModel в строки
          */
        void makeRequestSignal();
    private:
        CorrelationModel* corrModel;
        QStringList listOAllfRequests;//список запросов к базе данных

        QStringList templateForCreateRequests;//Данные для create table
        QStringList templateForInsertRequests;//Данные для insert into

        QVariantMap createTblRequests;//Реквесты для создания таблиц
        QVariantMap removeTblRequests; //Реквесты для удаления таблиц
        QVariantMap insertRequests;//Реквесты для вставки данных
        QVariantMap updateRequests;//Реквесты для обновления таблицы

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
        QString requestDataFromList(const QStringList& queryList, const QString&  templ);
        QString fillOneRequest(const QVariantList& templateItem, const QVariantList& visElements);
        QString chooseRequestString(const QString& temp);
        void makeCreateTableRequest();
        void makeRemoveTableRequest();
        void makeUpdateTableRequest();
        void makeInsertIntoListRequests();
        void fillingRequestList();
        void initlist();
        QVariant findByID(const QVariantList& list, const QVariant& searchTmpl);
        void makeRequest();
        void resetList();
};
#endif // QUERYMODEL_H
