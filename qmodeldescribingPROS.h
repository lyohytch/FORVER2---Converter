#ifndef QMODELDESCRIBINGPROS_H
#define QMODELDESCRIBINGPROS_H
#include "qmodeldescribing.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Класс для формата из прокуратуры, представляет собой 4 файла(формы), в которых данные записываются,
// начиная с пятой(необязательно) строки. Связующее звено - элемент Р(вероятно, некоторый id)
class QModelDescribingPros: public QModelDescribing
{
    public:
        QModelDescribingPros(QObject* parent = 0);
        ~QModelDescribingPros() {};
        virtual bool isValidElementsWithDataForParticularFile();
    protected:
        //GENERAL
        virtual QMap<QString, QVariant>* processLineInDescriptionFile(const QString& line);
        virtual bool checkFileStructure(QTextStream* fileStream);
        virtual void addNextElementsToList(QMap<QString, QVariant> & oneRec);
        virtual QVariantList getElementsFromText(QTextStream* fileStream);
        //DATA
        virtual void addingLoadedDataInVisibleElementsWithData(QTextStream* fileStream);
        virtual QVariantList processLineInDataFile(const QString& line, const QVariantList& DataStructure);
        virtual bool isValidStringInDataFileToAdd(const QVariantList& dataStructure);
        virtual void setElementNameByDataFile(const QString& filename);
        QVariant getIdByStatName(const QString& statName, const  QVariantList& oneRecord);
        virtual void preparingDataStructureBeforeFilling();//iListDataTemp=>iListData
        virtual bool checkFileFileStructureData(QTextStream* /*fileStream*/);
    private:
        bool isProcessLine;
        QString removeSpaces(const QString& ex);
};
#endif // QMODELDESCRIBINGPROS_H
