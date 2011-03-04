#ifndef QMODELDESCRIBINGPROS_H
#define QMODELDESCRIBINGPROS_H
#include "qmodeldescribing.h"

class QModelDescribingPros: public QModelDescribing
{
    public:
        QModelDescribingPros(QObject* parent = 0);
        ~QModelDescribingPros() {}
        virtual bool isValidElementsWithDataForParticularFile();
    protected:
        //GENERAL
        //virtual QMap<QString, QVariant>* processLineInDescriptionFile(const QString& line);
        virtual bool checkFileStructure(QTextStream* fileStream);
        virtual void setAdditionsToNamesByFile(const QString &filename);
        //virtual void addNextElementsToList(QMap<QString, QVariant> & oneRec);
        virtual QVariantList getElementsFromText(QTextStream* /*fileStream*/);
        virtual QVariantMap fillOneElement(const QStringList & /*capturedText*/) {return QVariantMap();}
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
        QString setNameWithoutRepeat(const QString &namekey, const QVariantList &elements);
};
#endif // QMODELDESCRIBINGPROS_H
