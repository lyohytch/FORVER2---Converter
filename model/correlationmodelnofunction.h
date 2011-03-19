#ifndef CORRELATIONMODELNOFUNCTION_H
#define CORRELATIONMODELNOFUNCTION_H

#include "correlationmodel.h"
//! CorrelationModelNoFunction - класс для построения корреляционных таблиц без функций
/**
  * Класс используется для работы с корреляционной таблицей. В таблице присутствую только
  * столбцы приёмника и источника
  */
class CorrelationModelNoFunction: public CorrelationModel
{
        Q_OBJECT

    public:
        /**
          * Инициализурует переменные iTemplateModel и iTargetModel.Также создаётся пустая tableModel
          * \param parent указатель на родительский виджет
          * \param templ указатель на объект приёмника
          * \param target указатель на объект источника
          * \return none
          */
        CorrelationModelNoFunction(QWidget* parent, QModelDescribing* templ, QModelDescribing* target);
        /**
          * Инициализурует переменные iTemplateModel и iTargetModel.Также создаётся tableModel по некоторой iTableModel
          * \param parent указатель на родительский виджет
          * \param templ указатель на объект приёмника
          * \param target указатель на объект источника
          * \param iTableModel указатель на некоторую таблицу корреляции
          * \return none
          */
        CorrelationModelNoFunction(QWidget* parent, QModelDescribing* templ, QModelDescribing* target, QStandardItemModel* iTableModel);
        virtual void setupTableModel(QStandardItemModel* tableModel);
        virtual QVariantList targetToTemplate();
        virtual void fillInTable();
        virtual void fillInTable(QVariantMap mapTable);
        virtual void setApplyTreeClick(int id)
        {
            Q_UNUSED(id);
        }

    private:
        int findTargetIndexByTargetName(const QString& targetString);
        QVariant findTemplateByTargetName(const QString& tname);
        QString processTargetData(const QVariantList& convertedData, const QVariant& elementFromCorrTable);
        QVariant findTargetDataByName(const QString& targetTemplate, const QVariantList& convertedData);
        QString setNewDataValueByType(const QString& typeElement,  const QString& correlationValue, const QString& dataString);
        bool compareCompactCodes(const QString& codedDataWithoutCode, const QString& datafromTarget);
        bool compareSimpleCodes(const QString& codedDataWithoutCode,  const QString& dataFromTarget);
        bool dependIdSetted(const QString& dependId, const QVariantList& convertedData);
        QStandardItem* addHeadAndDependingItemsInTarget(const QVariant& elemFromTemplate);
        QVariant findTargetDescriptionByName(const QString& targetTemplate);
    protected:
        virtual QVariant switchFunction(int /*id*/, const QVariantList& /*parameters*/)
        {
            return QVariant();
        }

};

#endif // CORRELATIONMODELNOFUNCTION_H
