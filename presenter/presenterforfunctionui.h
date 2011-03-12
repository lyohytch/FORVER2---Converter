#ifndef PRESENTERFORFUNCTIONUI_H
#define PRESENTERFORFUNCTIONUI_H
#include <QWidget>
#include <QHBoxLayout>
#include <QRadioButton>

#include "presenter.h"
//Класс для выбора функции
class QWidgetFunction : public QWidget
{
        Q_OBJECT
    public:
        QWidgetFunction();
        ~QWidgetFunction() {}
    protected:
        virtual void closeEvent(QCloseEvent */*clEvent*/);
    signals:
        void FuncWasChecked(int id);
};

class PresenterForFunctionUI : public Presenters
{
        Q_OBJECT

    public:
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
        void setupFunctionWidget();
        virtual void allocateCorrelationModel();
    protected slots:
        virtual void createObjects();
        virtual void freeObjects();
        virtual void ElementTableActivated(const QModelIndex&);
        void FunctionIsChecked(int functionId);
};

#endif // PRESENTERFORFUNCTIONUI_H
