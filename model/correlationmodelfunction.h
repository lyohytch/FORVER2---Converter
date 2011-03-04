#ifndef CORRELATIONMODELFUNCTION_H
#define CORRELATIONMODELFUNCTION_H

#include "correlationmodel.h"
//! CorrelationModelFunction - класс для построения корреляционных таблиц c функциями
/**
  * Класс используется для работы с корреляционной таблицей, в которой добавлен столбец Функция
  *
  */
class CorrelationModelFunction: public CorrelationModel
{
    Q_OBJECT

public:

    /**
      * Список доступных функций
      */
    enum EfuncList
    {
        /** возраст */
        age_func = 0,
        /** объединение */
        concat_func,
        /** непределённая функция */
        undef_func
    };
    /**
      * Инициализурует переменные iTemplateModel и iTargetModel.Также создаётся пустая tableModel
      * \param parent указатель на родительский виджет
      * \param templ указатель на объект приёмника
      * \param target указатель на объект источника
      */
    CorrelationModelFunction(QWidget* parent, QModelDescribing* templ, QModelDescribing* target);
    /**
      * Инициализурует переменные iTemplateModel и iTargetModel.Также создаётся tableModel по некоторой iTableModel
      * \param parent указатель на родительский виджет
      * \param templ указатель на объект приёмника
      * \param target указатель на объект источника
      * \param iTableModel указатель на некоторую таблицу корреляции
      */
    CorrelationModelFunction(QWidget* parent, QModelDescribing* templ, QModelDescribing* target, QStandardItemModel* iTableModel);

    virtual void setApplyTreeClick(int id);
    virtual bool applyTreeClick(int id);
    virtual QVariantMap tableModelToMap();
    virtual void fillInTable();
    virtual void fillInTable(QVariantMap mapTable);
    virtual void changeFunctionValue(int col, int row, int funcId);

protected:
     virtual  void setupTableModel(QStandardItemModel* tableModel);
     virtual QVariant switchFunction(int id, const QVariantList& parameters);
     /**
       * Вычисляет возраст по дате рождения и сегодняшней дате
       * \param startDate дата рождения
       * \param endDate сегодняшняя дата
       * \return Возраст в годах
       */
     QVariant age(const QVariant& startDate, const QVariant& endDate);
     /**
       * Объединяет список строк в одну
       * \param parameters список строк
       * \return Объединённая строка
       */
     QVariant concat(const QVariantList& parameters);
     /**
       * Вычисляет возраст, имея список параметров
       * \param parameters список параметров
       * \return Возраст в годах
       */
     QVariant age(const QVariantList& parameters);
     /**
       * Выдаёт имя функции по её ИД
       * \param id идентификатор функции
       * \return Имя функции
       */
     QString functionName(int id);

private:
         bool isFunc;
};

#endif // CORRELATIONMODELFUNCTION_H
