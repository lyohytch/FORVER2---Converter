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

        CorrelationModel(QWidget* parent, QModelDescribing* templ, QModelDescribing* target);
        CorrelationModel(QWidget* parent, QModelDescribing* templ, QModelDescribing* target, QStandardItemModel* iTableModel);
        ~CorrelationModel();

        virtual void fillInTable() = 0;
        virtual void fillInTable(QVariantMap mapTable);
        virtual QVariantMap tableModelToMap();
        virtual QVariantList targetToTemplate();
        virtual void setApplyTreeClick(int id);
        virtual bool applyTreeClick(int id);

        QModelDescribing* getTemplateModel()
        {
            return iTemplateModel;
        }
        QModelDescribing* getTargetModel()
        {
            return iTargetModel;
        }
        QStandardItemModel* getTableModel()
        {
            return tableModel;
        }
        void clearCorrelationTable();
        //Change table
        void changeTemplateValue(int col, int row, QVariant data);
        void saveDataForTarget(const QVariant &data, int row, int col);
        void changeTargetValue(int col, int row, QVariant data, bool first);
        virtual void changeFunctionValue(int /*col*/, int /*row*/, int /*funcId*/) {}
    protected:
        virtual void setupTableModel(QStandardItemModel* tableModel) = 0;
        virtual QVariant switchFunction(int /*id*/, const QVariantList& /*parameters*/) = 0;// { return QVariant();}
        virtual QVariant findItemInTableTemplate(const QVariant& search, int& row);
        QVariantList foundByUIDsRetValues(const QVariantList& dataListItem, const QVariantList& searchTemplates);

    private:
        QModelDescribing* iTemplateModel;
        QModelDescribing* iTargetModel;
        QStandardItemModel* tableModel;
        bool isTemp;
        bool isTarg;
};

#endif // CORRELATIONMODEL_H
