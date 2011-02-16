#ifndef QMODELDESCRIBINGDEMO_H
#define QMODELDESCRIBINGDEMO_H
#include "qmodeldescribing.h"

//Класс для демоверсии: вроде бы один файл для считывания
class QModelDescribingDemo: public QModelDescribing
{
    public:
        QModelDescribingDemo(QObject* parent = 0);
        ~QModelDescribingDemo() {};
        virtual bool isValidElementsWithDataForParticularFile();

    protected:
        //GENERAL
        virtual QMap<QString, QVariant>* processLineInDescriptionFile(const QString& line);
        virtual  bool checkFileStructure(QTextStream* fileStream);
        virtual bool isValidStringInDescriptionFileToAdd(const QMap<QString, QVariant> &checkMap);
        virtual QVariantList getElementsFromText(QTextStream* fileStream);
        //DATA
        virtual void addingLoadedDataInVisibleElementsWithData(QTextStream* fileStream);
        virtual QVariantList processLineInDataFile(const QString& line, const QVariantList& DataStructure);
        virtual bool isValidStringInDataFileToAdd(const QVariantList& dataStructure);
        virtual QVariantList initDataStructure();
        virtual bool checkFileFileStructureData(QTextStream* /*fileStream*/);
        virtual void setElementNameByDataFile(const QString &/*filename*/) {};
        virtual void preparingDataStructureBeforeFilling() {};
};
#endif // QMODELDESCRIBINGDEMO_H
