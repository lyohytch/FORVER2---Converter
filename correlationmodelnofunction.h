#ifndef CORRELATIONMODELNOFUNCTION_H
#define CORRELATIONMODELNOFUNCTION_H

#include "correlationmodel.h"

class CorrelationModelNoFunction: public CorrelationModel
{
public:
    CorrelationModelNoFunction(QWidget* parent, QModelDescribing* current, QModelDescribing* target);
    CorrelationModelNoFunction(QWidget* parent, QModelDescribing* current, QModelDescribing* target, QStandardItemModel* iTableModel);

    /**
      Fill correlation table from description file
    */
    virtual void fillInTable();

};

#endif // CORRELATIONMODELNOFUNCTION_H
