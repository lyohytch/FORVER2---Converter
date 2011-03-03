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
        QVariantMap fillCorrelationMap();
    private slots:
        void processDataFromMainPresenter(const QModelIndex & tableData);
        void elementInCorrTableActivated(const QModelIndex &elementData);
        void elementInCorrTableClear(const QModelIndex &elementData);
        void updateElementInCorrTable(int descriptionId, const QString &value);
    private:
        QTableView *viewMainCorrs;
        QVBoxLayout* cLayout;
        QStandardItemModel *tableModel;
        QStandardItemModel* createModelFromData(const QVariant &corrData);
        QList<QStandardItem *> fillRowInAdditionalTable(QVariantList & elfill, int row);
        int activeRow;
        int activeColumn;
        int mainRow;
        int mainColumn;
        bool isWaitChanges;
        QString nameOfTarget;
        QString typeOfElement;
    signals:
        void sendDataToMainTableForm(const QModelIndex &tableData);
        void sendDataToMainTableForm(QStandardItem *item, int mRow, int mCol);
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
    virtual void ElementTreeTargetActivated(const QModelIndex& index);
    virtual void ElementTreeTemplateActivated(const QModelIndex& index);
    void getDataFromAddCorrelationTable(QStandardItem *item, int mainRow, int mainColumn);
private:
    AdditionCorrelationTable *AddTableForm;
signals:
    void sendDataToAddCorrForm(const QModelIndex &);
    void sendChangedDataToAddCorrForm(int descriptionId, const QString &);
};

#endif // PRESENTERFORNONFUNCTIONUI_H
