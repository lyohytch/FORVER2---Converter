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
    virtual bool isValidStringInDescriptionFileToAdd(const QMap<QString, QVariant> &checkMap);
    virtual QVariantMap fillOneElement(const QStringList & capturedText);
    virtual QVariantList getElementsFromText(QTextStream* fileStream);
};

#endif // QMODELDESCRIBINGFROMJURA_H
