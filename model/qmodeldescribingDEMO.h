#ifndef QMODELDESCRIBINGDEMO_H
#define QMODELDESCRIBINGDEMO_H
#include "qmodeldescribing.h"

//! QModelDescribingDemo - класс для классификаторов демо-версии программы
/**
  * Класс обрабатывает новые файлы описания. Существует один файл описания, общий, в отличие от QModelDescribingOld4
  */
class QModelDescribingDemo: public QModelDescribing
{
    public:
        /**
          * Конструктор по умолчанию. Инициализирует QStandardItemModel
          * \param parent указатель на родительский объект QObject
          */
        QModelDescribingDemo(QObject* parent = 0);
        ~QModelDescribingDemo() {}
        virtual bool isValidElementsWithDataForParticularFile();

    protected:
        //Operations with elements without data
        virtual  bool checkFileStructure(QTextStream* fileStream);
        virtual bool isValidStringInDescriptionFileToAdd(const QMap<QString, QVariant> &checkMap);
        virtual QVariantMap fillOneElement(const QStringList& capturedText);
        //Operations with elements with data
        virtual void addingLoadedDataInVisibleElementsWithData(QTextStream* fileStream);
        virtual QVariantList processLineInDataFile(const QString& line, const QVariantList& DataStructure);
        virtual bool isValidStringInDataFileToAdd(const QVariantList& dataStructure);
        virtual QVariantList initDataStructure();
        virtual bool checkFileFileStructureData(QTextStream* fileStream);
        virtual void setElementNameByDataFile(const QString& filename)
        {
            Q_UNUSED(filename);
        };
        virtual void preparingDataStructureBeforeFilling() {};
};
#endif // QMODELDESCRIBINGDEMO_H
