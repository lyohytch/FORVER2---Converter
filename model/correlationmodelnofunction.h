#ifndef CORRELATIONMODELNOFUNCTION_H
#define CORRELATIONMODELNOFUNCTION_H

#include "correlationmodel.h"

class CorrelationModelNoFunction: public CorrelationModel
{
public:
    CorrelationModelNoFunction(QWidget* parent, QModelDescribing* current, QModelDescribing* target);
    CorrelationModelNoFunction(QWidget* parent, QModelDescribing* current, QModelDescribing* target, QStandardItemModel* iTableModel);
    virtual void setupTableModel(QStandardItemModel *tableModel);
    virtual void createTableModel(QStandardItemModel *tableModel);
    /**
      Fill correlation table from description file
    */
    virtual void fillInTable();
private:
    int findTargetIndexByTargetValue(const QString &targetString);
protected:
    virtual QVariant switchFunction(int /*id*/, const QVariantList& /*parameters*/)  {return QVariant();}

};

#endif // CORRELATIONMODELNOFUNCTION_H
