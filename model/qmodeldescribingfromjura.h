#ifndef QMODELDESCRIBINGFROMJURA_H
#define QMODELDESCRIBINGFROMJURA_H

#include "qmodeldescribing.h"

/** Класс для листа описания, утверждённого юристами.
 - класс меняется - БД перегенирируется
 - для класса нет текстовых данных, которые нужно забивать в БД
 - в данный формат конвертируются файлы из прокуратуры. Предполагается, что формат в прокуратуре не изменяется(наименование полей)
 - в файле описания содержится информация о соответсвии между формами, по которой сразу будет генерироваться табличка
 - считывается один файл описания (подобно демо-версии)
*/

class QModelDescribingFromJura: public QModelDescribing
{
public:
    QModelDescribingFromJura(QObject* parent = 0);
    ~QModelDescribingFromJura() {}
    //Add new field
protected:
    virtual  bool checkFileStructure(QTextStream* fileStream);
    virtual bool isValidStringInDescriptionFileToAdd(const QMap<QString, QVariant> &/*checkMap*/) {return true;}
    virtual QVariantMap fillOneElement(const QStringList & capturedText);
    virtual QVariantList getElementsFromText(QTextStream* fileStream);


    //Operation with data. Not needed. I have to move basic class
    // I want create one basic class and two children. They are _data_ and _describing_
    virtual bool checkFileFileStructureData(QTextStream* /*fileStream*/) {return true;}
    virtual QVariantList processLineInDataFile(const QString& /*line*/, const QVariantList &/*DataStructure*/) {return QVariantList();}
    virtual bool isValidStringInDataFileToAdd(const QVariantList &/*checkMap*/) {return true;}
    virtual void preparingDataStructureBeforeFilling() {}
    virtual void addingLoadedDataInVisibleElementsWithData(QTextStream* /*fileStream*/) {}
    virtual void setElementNameByDataFile(const QString &/*filename*/) {}
    virtual bool isValidElementsWithDataForParticularFile() {return true;}

private:
    QString setTargetDataForConvert(const QString & dataFor);
    QVariantMap setDependFieldInfo(const QStringList &capturedText, int countDependFields);
    QString FromIntegerToBinaryString(int countDependFields);
    void alignOneAndZerosInCorrelationCodes(int valueOfOnesAndZeros, QVariantList *dependList);
    QVariantMap getNewAlignedCorrelationsList(int countDependFields, const QVariantMap &element, QVariantList *dependFieldList);
    QString getElementNameByCodeForm(const QString&);
};

#endif // QMODELDESCRIBINGFROMJURA_H
