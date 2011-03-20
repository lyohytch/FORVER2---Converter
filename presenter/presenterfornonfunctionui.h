#ifndef PRESENTERFORNONFUNCTIONUI_H
#define PRESENTERFORNONFUNCTIONUI_H
#include <QTableView>
#include <QVBoxLayout>
#include <QStandardItemModel>

#include "presenter.h"
//! AdditionCorrelationTable класс для дополнительной настройки элементов источника
/**
  * Класс представляет собой таблицу, в которой можно настраивать дополнительные свойства элемента источника.
  */
class AdditionCorrelationTable : public QWidget
{
        Q_OBJECT
    public:
        /**
          * Конструктор по-умолчанию. Создаётся виджет, настраиваются его элементы, присоединение слотов к сигналам
          * \param presenter указатель на Presenters
          * \param _view указатель на IView
          */
        AdditionCorrelationTable(Presenters* presenter, IView* _view);
        ~AdditionCorrelationTable();
    protected:
        /**
          * Обработка события закрытия виджета AdditionCorrelationTable
          */
        virtual void closeEvent(QCloseEvent */*clEvent*/);
        /**
          * Заполнить элемент с дополнительными свойствами для возвращения его в CorrelationModel
          * \return Возвращает настроенные свойства элемента обратно в CorrelationModel
          */
        QVariantMap fillCorrelationMap();
    private slots:
        void processDataFromMainPresenter(const QModelIndex& tableData);
        void elementInCorrTableActivated(const QModelIndex& elementData);
        void elementInCorrTableClear(const QModelIndex& elementData);
        void updateElementInCorrTable(int descriptionId, const QString& value);
    private:
        QTableView* viewMainCorrs;
        QVBoxLayout* cLayout;
        QStandardItemModel* tableModel;
        QStandardItemModel* createModelFromData(const QVariant& corrData);
        QList<QStandardItem*> fillRowInAdditionalTable(QVariantList& elfill, int row);
        QList<QStandardItem*> fillColumnInAdditionTable(QVariantList &elfill, int col);
        int activeRow;
        int activeColumn;
        int mainRow;
        int mainColumn;
        bool isWaitChanges;
        QString nameOfTarget;
        QString typeOfElement;
    signals:
        /**
          * Послать данные(дополнительные свойства элемента источника) в CorrelationModel
          * \param item Элемент, указатель на QStandardItem
          * \param mRow строка, из которой элемент
          * \param mCol столбец, из которого элемент
          */
        void sendDataToMainTableForm(QStandardItem* item, int mRow, int mCol);
};

//! PresenterForNonFunctionUI управление действиями пользователя, прослойка между моделью и GUI
/**
  * Класс предоставляющий функционал для построения GUI. Корреляционная модель содержит столбцы приёмника и источника.
  * Существует возможность настраивать элементы источника.
  * \sa Presenters , AdditionCorrelationTable
  */
class PresenterForNonFunctionUI: public Presenters
{
        Q_OBJECT
    public:
        /**
          * Конструктор по-умолчанию
          * \param view указатель на IView, интерфейс для GUI
          * \param app указатель на QApplication. Используется для установки перевода
          */
        PresenterForNonFunctionUI(IView* view, QApplication* app): Presenters(view, app) {}
        ~PresenterForNonFunctionUI();
    protected:
        virtual void allocateCorrelationModel();
        /**
          * Выполняет функцию для конкретных параметров
          * \param id идентификатор функции
          * \param parameters список параметров для функции
          * \return Значение, возвращаемое функцией
          */
        virtual QVariant switchFunction(int id, const QVariantList& parameters)
        {
            Q_UNUSED(id);
            Q_UNUSED(parameters);
            return QVariantList();
        }
        virtual void allocateMemory();
        virtual void setModelsAndTreesObjects();
        virtual void createObjects();
        /**
          * Настройка виджета AdditionCorrelationTable
          */
        void setupAddTableForm();
    protected slots:
        virtual void freeObjects();
        virtual void ElementTableActivated(const QModelIndex& index);
        virtual void ElementTreeTargetActivated(const QModelIndex& index);
        virtual void ElementTreeTemplateActivated(const QModelIndex& index);
        /**
          * Слот обрабатывающий сигнал AdditionCorrelationTable::sendDataToMainTableForm
          * \param item указатель на QStandardItem. Элемент из корреляционной таблицы CorrelationModel
          * \param mainRow строка, где находился элемент
          * \param mainColumn столбец, где находился элемент
          */
        void getDataFromAddCorrelationTable(QStandardItem* item, int mainRow, int mainColumn);
    private:
        AdditionCorrelationTable* AddTableForm;
    signals:
        /**
          * Отправить данные из CorrelationModel для AdditionCorrelationTable
          */
        void sendDataToAddCorrForm(const QModelIndex&);
        /**
          * Отправить данные от источника или приёмника от CorrelationModel в AdditionCorrelationTable.
          * Применяется при изменении дополнительных свойств элемента в AdditionCorrelationTable
          */
        void sendChangedDataToAddCorrForm(int descriptionId, const QString&);
};

#endif // PRESENTERFORNONFUNCTIONUI_H
