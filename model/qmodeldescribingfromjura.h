#ifndef QMODELDESCRIBINGFROMJURA_H
#define QMODELDESCRIBINGFROMJURA_H

#include "qmodeldescribing.h"

//! QModelDescribingFromJura - класс для классификатора, составленного студентами ЮрФака ННГУ
/**
  * Класс для листа описания, утверждённого студентами ЮрФака ННГУ. Содержание файла описания похоже на файлы описаний для QModelDescribingDemo и QModelDescribingOld4
  * класс меняется - БД перегенирируется
  * для класса нет текстовых данных, которые нужно забивать в БД. Класс является так называемым чистым приёмником.
  * в данный формат конвертируются файлы из прокуратуры. Предполагается, что формат в прокуратуре не изменяется(наименование полей)
  * в файле описания содержится информация о соответсвии между формами, по которой сразу будет генерироваться табличка
  * считывается один файл описания
  * \mainpage QModelDescribingFromJura
  */

class QModelDescribingFromJura: public QModelDescribing
{
    public:
        /**
          * Конструктор по умолчанию. Инициализирует QStandardItemModel
          * \param parent указатель на родительский объект QObject
          */
        QModelDescribingFromJura(QObject* parent = 0);
        ~QModelDescribingFromJura() {}
        //Add new field
    protected:
        virtual  bool checkFileStructure(QTextStream* fileStream);
        virtual bool isValidStringInDescriptionFileToAdd(const QMap<QString, QVariant> &/*checkMap*/)
        {
            return true;
        }
        virtual QVariantMap fillOneElement(const QStringList& capturedText);
        virtual QVariantList getElementsFromText(QTextStream* fileStream);
        virtual QVariantList getElementsFromExcel(QObject* excSheet);


        //Operation with data. Not needed. I have to move basic class
        // I want create one basic class and two children. They are _data_ and _describing_
        virtual bool checkFileFileStructureData(QTextStream* /*fileStream*/)
        {
            return true;
        }
        virtual QVariantList processLineInDataFile(const QString& /*line*/, const QVariantList &/*DataStructure*/)
        {
            return QVariantList();
        }
        virtual bool isValidStringInDataFileToAdd(const QVariantList &/*checkMap*/)
        {
            return true;
        }
        virtual void preparingDataStructureBeforeFilling() {}
        virtual void addingLoadedDataInVisibleElementsWithData(QTextStream* /*fileStream*/) {}
        virtual void setElementNameByDataFile(const QString &/*filename*/) {}
        virtual bool isValidElementsWithDataForParticularFile()
        {
            return true;
        }

    private:
        QString setTargetDataForConvert(const QString& dataFor);
        QVariantMap setDependFieldInfo(const QStringList& capturedText, int countDependFields);
        QVariantMap setDependFieldInfo(const QVariantMap& tmpElement, int countDependFields);
        QVariantMap fillOneElement(const QVariantMap& tmpElement);
        QString FromIntegerToBinaryString(int countDependFields);
        void alignOneAndZerosInCorrelationCodes(int valueOfOnesAndZeros, QVariantList* dependList);
        QVariantMap getNewAlignedCorrelationsList(int countDependFields, const QVariantMap& element, QVariantList* dependFieldList);
        QString getElementNameByCodeForm(const QString&);

        bool isDependElement(const QVariantMap& tmpElement);
        bool isHeadElement(const QVariantMap& tmpElement);
        bool isType(const QString& tstType);
#ifdef Q_OS_WIN32
        QVariant getCellValueFromExcel(QObject* qaxSheet, int row, int col);
        QVariantMap getTmpElementFromExcel(QObject* qaxSheet, int row);
#endif
};

#endif // QMODELDESCRIBINGFROMJURA_H
