#ifndef PRESENTERFORFUNCTIONUI_H
#define PRESENTERFORFUNCTIONUI_H
#include <QWidget>
#include <QHBoxLayout>
#include <QRadioButton>

#include "presenter.h"
//! QWidgetFunction класс для выбора функции
/**
  * Класс для отображения диалога выбора функции
  */
class QWidgetFunction : public QWidget
{
        Q_OBJECT
    public:
        QWidgetFunction();
        ~QWidgetFunction() {}
    protected:
        /**
          * Обработка закрытия виджета QWidgetFunction
          */
        virtual void closeEvent(QCloseEvent */*clEvent*/);
    signals:
        /**
          * Сигнализирует Presenters о выбранной функции
          * \param id идентификатор выбранной функции
          */
        void FuncWasChecked(int id);
};
//! PresenterForFunctionUI управление действиями пользователя, прослойка между моделью и GUI
/**
  * Класс предоставляющий функционал для построения GUI с возможностью выбора функции. Корреляционная модель
  * содержит столбец Функция
  * \sa Presenters
  */
class PresenterForFunctionUI : public Presenters
{
        Q_OBJECT

    public:
        /**
          * Конструктор по-умолчанию
          * \param view указатель на IView, интерфейс для GUI
          * \param app указатель на QApplication. Используется для установки перевода
          */
        PresenterForFunctionUI(IView* view, QApplication* app) : Presenters(view, app) {}
    private:
        //For function support
        QWidgetFunction* funcWidget;
        //Function
        QVBoxLayout* fLayout;
        QRadioButton* a1;
        QRadioButton* a2;
        QRadioButton* a3;
    protected:
        /**
          * Настройка виджета выбора функции для элемента в корреляционной таблице
          */
        void setupFunctionWidget();
        /**
          * Выделить память для CorrelationModel
          */
        virtual void allocateCorrelationModel();
    protected slots:
        /**
          * Слот для обработки сигнала IView::CreateObjects()
          */
        virtual void createObjects();
        /**
          * Слот для обработки сигнала IView::FreeObjects()
          */
        virtual void freeObjects();
        /**
          * Слот для обработки двойного нажатия на элемент в корреляционной таблице
          * \param index содержимое выбранного элемента
          */
        virtual void ElementTableActivated(const QModelIndex& index);
        /**
          * Реагирует на сигнал QWidgetFunction::FuncWasChecked
          * \param functionId идентификатор выбранной функции
          */
        void FunctionIsChecked(int functionId);
};

#endif // PRESENTERFORFUNCTIONUI_H
