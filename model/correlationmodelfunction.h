#ifndef CORRELATIONMODELFUNCTION_H
#define CORRELATIONMODELFUNCTION_H

#include "correlationmodel.h"

class CorrelationModelFunction: public CorrelationModel
{
    Q_OBJECT

public:
    enum EfuncList
    {
        age_func = 0, //QVariant age(QVariant startDate, QVariant endDate);
        concat_func,//QString concat(char *arg1 ...);
        undef_func
    };

    CorrelationModelFunction(QWidget* parent, QModelDescribing* current, QModelDescribing* target);
    CorrelationModelFunction(QWidget* parent, QModelDescribing* current, QModelDescribing* target, QStandardItemModel* iTableModel);

    virtual void setApplyTreeClick(int id);
    virtual bool applyTreeClick(int id);
    virtual QVariantMap tableModelToMap();
    virtual void fillInTable();
    virtual void fillInTable(QVariantMap mapTable);
    virtual void changeFunctionValue(int col, int row, int funcId);

protected:
     virtual  void setupTableModel(QStandardItemModel* tableModel);

     virtual QVariant switchFunction(int id, const QVariantList& parameters);
     QVariant age(const QVariant& startDate, const QVariant& endDate);
     QVariant concat(const QVariantList& parameters);
     QVariant age(const QVariantList& parameters);

     QString functionName(int id);

private:
         bool isFunc;
};

#endif // CORRELATIONMODELFUNCTION_H
