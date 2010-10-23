#ifndef CORRELATIONMODEL_H
#define CORRELATIONMODEL_H

#include <QMap>
#include <QString>
#include <QVariant>
#include <QTableView>


#include "qmodeldescribing.h"

typedef QVariant(*ConverterFunction)(QVariantList);

class CorrelationModel: public QTableView
{
        Q_OBJECT
    public:
        enum EfuncList
        {
            age_func = 0, //QVariant age(QVariant startDate, QVariant endDate);
            concat_func,//QString concat(char *arg1 ...);
            undef_func
        };
        CorrelationModel(QWidget* parent, QModelDescribing* current, QModelDescribing* target);
        CorrelationModel(QWidget* parent, QModelDescribing* current, QModelDescribing* target, QStandardItemModel* iTableModel);
        ~CorrelationModel();
        QModelDescribing* getCurrentModel()
        {
            return iCurrentModel;
        };
        QModelDescribing* getTargetModel()
        {
            return iTargetModel;
        };
        QStandardItemModel* getTableModel()
        {
            return tableModel;
        };
        QVariantMap tableModelToMap();
        void fillInTable();
        void fillInTable(QVariantMap mapTable);
        void clearTable();
        //TODO rework this function
        QVariantList targetToCurrent();
        QVariant findItemInTableTemplate(const QVariant& search, int& row);
        QVariant age(const QVariant& startDate, const QVariant& endDate);
        QVariant concat(const QVariantList& parameters);
        QVariant age(const QVariantList& parameters);
        QVariant switchFunction(int id, const QVariantList& parameters);
        void showCorrelationMap();
        void appendTableRow(const QVariantList& rowList); //Perhaps public slot
        void setApplyTreeClick(int id);
        bool applyTreeClick(int id);
        QString functionName(int id);
        QVariantList foundByUIDsRetValues(const QVariantList& dataListItem, const QVariantList& searchTemplates);

        //Change table
        void changeFunctionValue(int col, int row, int funcId);
        void changeTemplateValue(int col, int row, QVariant data);
        void changeTargetValue(int col, int row, QVariant data, bool first);
    private:
        QModelDescribing* iCurrentModel;
        QModelDescribing* iTargetModel;
        QList<QList<int> > correlationMap;//(Отношение между моделями)
        QStandardItemModel* tableModel;
        bool isTemp;
        bool isTarg;
        bool isFunc;

};

#endif // CORRELATIONMODEL_H
