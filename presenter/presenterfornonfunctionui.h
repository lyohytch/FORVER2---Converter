#ifndef PRESENTERFORNONFUNCTIONUI_H
#define PRESENTERFORNONFUNCTIONUI_H
#include <QTableView>
#include <QVBoxLayout>
#include <QStandardItemModel>

#include "presenter.h"


//Класс для выбора функции
class AdditionCorrelationTable : public QWidget
{
        Q_OBJECT
    public:
        AdditionCorrelationTable(Presenters *presenter, IView *_view);
        ~AdditionCorrelationTable() {}
    protected:
        virtual void closeEvent(QCloseEvent */*clEvent*/);
    private slots:
        void processDataFromMainPresenter(const QModelIndex & tableData);
        void elementInCorrTableActivated(const QModelIndex &elementData);
    private:
        QTableView *viewMainCorrs;
        QModelIndex corrAdditionMap;
        QVBoxLayout* cLayout;
        QStandardItemModel *tableModel;
        QStandardItemModel* createModelFromData(const QVariant &corrData);
        QList<QStandardItem *> fillRowInAdditionalTable(QVariantList & elfill);
    signals:
        void sendDataToMainTableForm(const QModelIndex &tableData);

};


class PresenterForNonFunctionUI: public Presenters
{
    Q_OBJECT
public:
    PresenterForNonFunctionUI(IView *view):Presenters(view) {};
protected:
    virtual void allocateCorrelationModel();
    virtual QVariant switchFunction(int /*id*/, const QVariantList& /*parameters*/) { return QVariantList(); }
    virtual void allocateMemory();
    virtual void setModelsAndTreesObjects();
    virtual void createObjects();
    void setupAddTableForm();
protected slots:
    virtual void freeObjects();
    virtual void ElementTableActivated(const QModelIndex & index);
    void getDataFromAddCorrelationTable(const QModelIndex &index);
private:
    AdditionCorrelationTable *AddTableForm;
signals:
    void sendDataToAddCorrForm(const QModelIndex &);
};

#endif // PRESENTERFORNONFUNCTIONUI_H
