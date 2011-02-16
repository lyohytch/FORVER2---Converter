#ifndef QMODELDESCRIBINGOLD4_H
#define QMODELDESCRIBINGOLD4_H
#include "qmodeldescribing.h"

//Класс для старого формата данных, где есть 4 файла: главный, фигурант, оружие, место
class QModelDescribingOld4: public QModelDescribing
{
    public:

        //Operations with elements without data
        QModelDescribingOld4(QObject* parent = 0);
        ~QModelDescribingOld4() {};

        //Operations with elements with data
        virtual void preparingDataStructureBeforeFilling();//iListDataTemp ->iListData
        virtual bool isValidElementsWithDataForParticularFile();

    protected:

        //Operations with elements without data
        virtual  bool checkFileStructure(QTextStream* fileStream);
        virtual QMap<QString, QVariant>* processLineInDescriptionFile(const QString& line);
        virtual QVariantList getElementsFromText(QTextStream* fileStream);
        //Operations with elements with data
        virtual void addingLoadedDataInVisibleElementsWithData(QTextStream* fileStream);
        virtual QVariantList processLineInDataFile(const QString& line, const QVariantList& DataStructure);
        virtual bool isValidStringInDataFileToAdd(const QVariantList& dataStructure);
        virtual void setElementNameByDataFile(const QString& filename);
        int setSeekofLine(const QString& statName);
        QVariant getIdByElementName(const QString& statName, const  QVariantList& oneRecord);
        virtual bool checkFileFileStructureData(QTextStream* /*fileStream*/);
};

#endif // QMODELDESCRIBINGOLD4_H
