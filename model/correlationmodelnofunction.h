#ifndef CORRELATIONMODELNOFUNCTION_H
#define CORRELATIONMODELNOFUNCTION_H

#include "correlationmodel.h"

class CorrelationModelNoFunction: public CorrelationModel
{
public:
    CorrelationModelNoFunction(QWidget* parent, QModelDescribing* templ, QModelDescribing* target);
    CorrelationModelNoFunction(QWidget* parent, QModelDescribing* templ, QModelDescribing* target, QStandardItemModel* iTableModel);
    virtual void setupTableModel(QStandardItemModel *tableModel);
    virtual QVariantList targetToTemplate();
    /**
      Fill correlation table from description file
    */
    virtual void fillInTable();
    virtual QVariant findItemInTableTemplate(const QVariant& search, int& row);
    virtual void setApplyTreeClick(int id) { Q_UNUSED(id); }

private:
    int findTargetIndexByTargetValue(const QString &targetString);
    QString processTargetData(const QVariantList & convertedData, const QVariant& elementFromCorrTable, const QVariantList &templateDescrList);
    QVariant findTargetDataById(const QString &targetTemplate, const QVariantList &convertedData);
    QString setNewDataValueByType(const QString &typeElement,  const QString &correlationValue, const QString &dataString);
    bool compareCompactCodes(const QString &codedDataWithoutCode, const QString & datafromTarget);
    bool dependIdSetted(const QString &dependId, const QVariantList &convertedData, const QVariantList &templateDescrList);
    QStandardItem* addHeadAndDependingItemsInTarget(const QVariant &elemFromTemplate);
    QVariant findTargetDescriptionById(const QString &targetTemplate);
protected:
    virtual QVariant switchFunction(int /*id*/, const QVariantList& /*parameters*/)  {return QVariant();}

};

#endif // CORRELATIONMODELNOFUNCTION_H
