#ifndef CORRELATIONMODEL_H
#define CORRELATIONMODEL_H

#include <QTableView>

#include "qmodeldescribing.h"

//! CorrelationModel - базовый класс для построения корреляционных таблиц
/**
  * Класс предоставляет модель для построения корреляционной таблицы между файлом источника и файлом приёмника.
  * Также класс выполняет конвертирование данных из формата источника в формат приёмника для последующего сохранения в БД
  */
class CorrelationModel: public QTableView
{
        Q_OBJECT

    public:
        /**
          * Инициализурует переменные iTemplateModel и iTargetModel.Также создаётся пустая tableModel
          * \param parent указатель на родительский виджет
          * \param templ указатель на объект приёмника
          * \param target указатель на объект источника
          */
        CorrelationModel(QWidget* parent, QModelDescribing* templ, QModelDescribing* target);
        /**
          * Инициализурует переменные iTemplateModel и iTargetModel.Также создаётся tableModel по некоторой iTableModel
          * \param parent указатель на родительский виджет
          * \param templ указатель на объект приёмника
          * \param target указатель на объект источника
          * \param iTableModel указатель на некоторую таблицу корреляции
          */
        CorrelationModel(QWidget* parent, QModelDescribing* templ, QModelDescribing* target,
                         QStandardItemModel* iTableModel);
        /**
          * Удаляет корреляционную таблицу tableModel
          */
        ~CorrelationModel();
        /**
          * Заполняет корреляционную таблицу, используя данные взятые из описания моделей источника и приёмника
          */
        virtual void fillInTable() = 0;
        /**
          * Заполняет корреляционную таблицу, используя данные, считанные из файла в виде QVariantMap
          * \param mapTable корреляционная таблица из файла
          */
        virtual void fillInTable(QVariantMap mapTable);
        /**
          * Преобразует корреляционную таблицу tableModel к карте QVariantMap
          * \return Преобразованная таблица корреляции
          */
        virtual QVariantMap tableModelToMap();
        /**
          * Конвертирование данных из источника в данные приёмника
          * \return Данные источника, преобразованные к данным приёмника
          */
        virtual QVariantList targetToTemplate();
        /**
          * Активизирует выбранный столбец в корреляционной таблице
          * \param id какой столбец выбран в корреляционной таблице. 0 - Приёмник, 1 - Источник
          * \sa applyTreeClick (int id)
          */
        virtual void setApplyTreeClick(int id);
        /**
          * Определяет, нужно ли проводит какие-либо действия с элементом из корреляционной таблицы
          * \param id какой столбец выбран в корреляционной таблице. 0 - Приёмник, 1 - Источник
          * \return Можно или нет проводить действия
          * \sa setApplyTreeClick (int id)
          */
        virtual bool applyTreeClick(int id);
        /**
          * Возвращает указатель на объект приёмника QModelDescribing
          * \return указатель на объект приёмника QModelDescribing
          */
        QModelDescribing* getTemplateModel()
        {
            return iTemplateModel;
        }
        /**
          * Возвращает указатель на объект источника QModelDescribing
          * \return указатель на объект источника QModelDescribing
          */
        QModelDescribing* getTargetModel()
        {
            return iTargetModel;
        }
        /**
          * Возвращает указатель на объект корреляционной таблицы QStandardItemModel
          * \return указатель на объект корреляционной таблицы QStandardItemModel
          */
        QStandardItemModel* getTableModel()
        {
            return tableModel;
        }
        /**
          * Очищает корреляционную таблицу
          */
        void clearCorrelationTable();
        /**
          * Изменяет значение в ячейке для столбца приёмника
          * \param col столбец
          * \param row строка
          * \param data данные для замещения
          * \sa changeTargetValue () , changeFunctionValue ()
          */
        void changeTemplateValue(int col, int row, QVariant data);
        /**
          * Изменяет значение в ячейке для столбца источника. Данные были получены от дополнительной корреляционной таблицы
          * \param data данные для замещения
          * \param col столбец
          * \param row строка
          */
        void saveDataForTarget(const QVariant& data, int row, int col);
        /**
          * Изменяет значение в ячейке для столбца источника
          * \param col столбец
          * \param row строка
          * \param data данные для замещения
          * \param first первый или нет элемент добавляется в ячейку таблицы
          * \sa changeTemplateValue () , changeFunctionValue ()
          */
        void changeTargetValue(int col, int row, QVariant data, bool first);
        /**
          * Изменяет значение в ячейке для столбца функций
          * \param col столбец
          * \param row строка
          * \param funcId выбранная функция
          * \sa changeTargetValue () , changeTargetValue ()
          */
        virtual void changeFunctionValue(int col, int row, int funcId)
        {
            Q_UNUSED(col);
            Q_UNUSED(row);
            Q_UNUSED(funcId)
        }
    protected:
        /**
          * Устанавливает для таблицы названия для шапок и устанавливает количество столбцов
          * \param tableModel указатель на корреляционную таблицу QStandardItemModel
          */
        virtual void setupTableModel(QStandardItemModel* tableModel) = 0;
        /**
          * Выбирает функцию преобразования данных источника по некоторому id функции. Выводит результат выполнения функции
          * \param id некоторая функция
          * \param parameters входные параметры для функции
          * \return Результат выполнения функции
          */
        virtual QVariant switchFunction(int id, const QVariantList& parameters) = 0;
        /**
          * Ищет элемент в столбце приёмника, если находит, то возвращает строку и
          * соответствующий элемент источника
          * \param search условия для поиска
          * \param row строка, в которой найден элемент
          * \return найденный элемент в столбце источника
          */
        virtual QVariant findItemInTableTemplate(const QVariant& search, int& row);
        /**
          * Ищет элементы в некотором списке.
          * \param dataListItem элементы, в которых ищем
          * \param searchTemplates те элементы, которые ищем
          * \return найденный список элементов
          * \note Используется при выборе функции преобразования элементов источника
          */

        QVariantList foundByUIDsRetValues(const QVariantList& dataListItem, const QVariantList& searchTemplates);

    private:
        /** Указатель на модель описания приёмника*/
        QModelDescribing* iTemplateModel;
        /** Указатель на модель описания источника*/
        QModelDescribing* iTargetModel;
        /** Указатель на корреляционную таблицу*/
        QStandardItemModel* tableModel;
        /** Можно ли проводить действия со столбцом приёмника в корреляционной таблице*/
        bool isTemp;
        /** Можно ли проводить действия со столбцом источника в корреляционной таблице*/
        bool isTarg;
};

#endif // CORRELATIONMODEL_H
