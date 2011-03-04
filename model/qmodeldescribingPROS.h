#ifndef QMODELDESCRIBINGPROS_H
#define QMODELDESCRIBINGPROS_H
#include "qmodeldescribing.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//РљР»Р°СЃСЃ РґР»СЏ С„РѕСЂРјР°С‚Р° РёР· РїСЂРѕРєСѓСЂР°С‚СѓСЂС‹, РїСЂРµРґСЃС‚Р°РІР»СЏРµС‚ СЃРѕР±РѕР№ 4 С„Р°Р№Р»Р°(С„РѕСЂРјС‹), РІ РєРѕС‚РѕСЂС‹С… РґР°РЅРЅС‹Рµ Р·Р°РїРёСЃС‹РІР°СЋС‚СЃСЏ,
// РЅР°С‡РёРЅР°СЏ СЃ РїСЏС‚РѕР№(РЅРµРѕР±СЏР·Р°С‚РµР»СЊРЅРѕ) СЃС‚СЂРѕРєРё. РЎРІСЏР·СѓСЋС‰РµРµ Р·РІРµРЅРѕ - СЌР»РµРјРµРЅС‚ Р (РІРµСЂРѕСЏС‚РЅРѕ, РЅРµРєРѕС‚РѕСЂС‹Р№ id)
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
