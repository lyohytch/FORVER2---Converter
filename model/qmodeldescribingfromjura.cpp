#include "qmodeldescribingfromjura.h"
#include "constants.h"
#include "constants_model.h"

#ifdef Q_OS_WIN32
#include <QAxObject>
#include <QAxWidget>
#endif


//TODO: move to config
namespace CapsExcel
{
    enum caps
    {
        Id = 1,
        Lev = 2,
        Type = 7,
        Dop = 8,
        Comm = 11,
        Red = 12,
        Form = 13,
        Name = 15,
        Val = 16
    };
}

QModelDescribingFromJura::QModelDescribingFromJura(QObject* parent):
    QModelDescribing(parent)
{
}

QVariantList QModelDescribingFromJura::getElementsFromExcel(QObject* excSheet)
{
    QVariantList elements = QVariantList();
#ifdef Q_OS_WIN32
    QAxObject* sheet = (QAxObject*)excSheet;
    QAxObject* usedrange = sheet->querySubObject("UsedRange");
    QAxObject* rows = usedrange->querySubObject("Rows");
    int intRowStart = usedrange->property("Row").toInt();
    int intRows = rows->property("Count").toInt();
    qDebug() << "row Count: " << intRows;
    qDebug() << "row Start: " << intRowStart;
    //QVariantMap elementForAdding; //Основные элементы
    //QVariantMap dependElementForAdding; // Элементы из dependList
    QVariantMap tmpElement = QVariantMap();

    int countDependFields = 0;
    int count = -1;
    QVariantList dependFieldList;
    for (int i = intRowStart; i < intRows; ++i)
    {
        tmpElement = getTmpElementFromExcel(sheet, i);
        if (isHeadElement(tmpElement))
        {
            if (countDependFields)
            {
                elements[count] = getNewAlignedCorrelationsList(countDependFields, elements[count].toMap(), &dependFieldList);
                countDependFields = 0;
                dependFieldList.clear();
            }
            elements.append(fillOneElement(tmpElement));
            count++;
        }
        else if (isDependElement(tmpElement) && count != -1)
        {
            dependFieldList.append(setDependFieldInfo(tmpElement, ++countDependFields));
        }
        else
        {
            qWarning() << "Line in description is incorrect";
        }
    }
    //Post processing
    if (count != -1 && countDependFields)
    {
        elements[count] = getNewAlignedCorrelationsList(countDependFields, elements[count].toMap(), &dependFieldList);
        countDependFields = 0;
        dependFieldList.clear();
    }

    delete rows;
    delete usedrange;
    delete sheet;
#else
    Q_UNUSED(excSheet);
#endif
    return elements;
}

#ifdef Q_OS_WIN32
QVariant QModelDescribingFromJura::getCellValueFromExcel(QObject* qaxSheet, int row, int col)
{
    QAxObject* cell = ((QAxObject*)(qaxSheet))->querySubObject("Cells(int,int)", row, col);
    QVariant valueCell = cell->dynamicCall("Value()");
    delete cell;
    return valueCell;
}
#endif

#ifdef Q_OS_WIN32
QVariantMap QModelDescribingFromJura::getTmpElementFromExcel(QObject* qaxSheet, int row)
{
    QVariant valueCell;
    QVariantMap tmpElement;
    //Get ID
    valueCell = getCellValueFromExcel(qaxSheet, row, CapsExcel::Id);
    qDebug() << row << CapsExcel::Id << valueCell;
    tmpElement.insert(id, valueCell);
    //Get Level
    valueCell = getCellValueFromExcel(qaxSheet, row, CapsExcel::Lev);
    qDebug() << row << CapsExcel::Lev << valueCell;
    tmpElement.insert(level, valueCell);
    //Get type
    valueCell = getCellValueFromExcel(qaxSheet, row, CapsExcel::Type);
    qDebug() << row << CapsExcel::Type << valueCell;
    tmpElement.insert(type, valueCell);
    // Get dop
    valueCell = getCellValueFromExcel(qaxSheet, row, CapsExcel::Dop);
    qDebug() << row << CapsExcel::Dop << valueCell;
    tmpElement.insert(dependId, valueCell);
    //Get comment . It's hint
    valueCell = getCellValueFromExcel(qaxSheet, row, CapsExcel::Comm);
    qDebug() << row << CapsExcel::Comm << valueCell;
    tmpElement.insert(hint, valueCell);
    //Set name
    tmpElement.insert(name, valueCell);
    //Get reduce name
    valueCell = getCellValueFromExcel(qaxSheet, row, CapsExcel::Red);
    qDebug() << row << CapsExcel::Red << valueCell;
    if (!valueCell.isNull())
    {
        tmpElement.insert(name, valueCell);
    }
    //Get form
    valueCell = getCellValueFromExcel(qaxSheet, row, CapsExcel::Form);
    qDebug() << row << CapsExcel::Form << valueCell;
    tmpElement.insert(formId, valueCell);
    //Get depend target name
    valueCell = getCellValueFromExcel(qaxSheet, row, CapsExcel::Name);
    qDebug() << row << CapsExcel::Name << valueCell;
    tmpElement.insert(targetName, valueCell);
    //Get targetDataForConvert value
    valueCell = getCellValueFromExcel(qaxSheet, row, CapsExcel::Val);
    qDebug() << row << CapsExcel::Val << valueCell;
    tmpElement.insert(targetDataForConvert, valueCell);
    return tmpElement;
}

#endif

bool QModelDescribingFromJura::isDependElement(const QVariantMap& tmpElement)
{
    bool isDepend = tmpElement.value(id).isNull();
    isDepend = isDepend && tmpElement.value(level).isNull();
    isDepend = isDepend && tmpElement.value(type).isNull();
    isDepend = isDepend && !tmpElement.value(hint).isNull();
    return isDepend;
}

bool QModelDescribingFromJura::isHeadElement(const QVariantMap& tmpElement)
{
    bool isHead = !tmpElement.value(id).isNull() && tmpElement.value(id).type() == QVariant::Double;
    isHead = isHead && !tmpElement.value(level).isNull() && tmpElement.value(level).type() == QVariant::Double;
    isHead = isHead && !tmpElement.value(type).isNull() && isType(tmpElement.value(type).toString());
    return isHead;
}

bool QModelDescribingFromJura::isType(const QString& tstType)
{
    bool isElementType = (ME == tstType) || (AV == tstType) || (DV == tstType) ||
                         (ED == tstType) || (ET == tstType) || (RM == tstType) || (CB == tstType);
    return isElementType;
}

QVariantList QModelDescribingFromJura::getElementsFromText(QTextStream* fileStream)
{
    QVariantList elements = QVariantList();
    QString text = fileStream->readAll();
    QStringList textSplitted = text.split(QRegExp("\\n"));
    QStringList capturedText = QStringList();
    // TODO: rework regexp
    /*
      It's regular expression for strings like
      "101  2                   ed              Год возбуждения     Ф5  3   Г   знач"
    */
    //               1id          2level          3type       4dop        Mask    Config    5Comment   6Short Name  7Form      8Position-InForm    9Name      10Encoding data
    QRegExp search("([^\\t]+)\\t([^\\t]+)\\t{5}([^\\t]+)\\t([^\\t]*)\\t[^\\t]*\\t[^\\t]*\\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)");
    /*
      It's regular expression for strings like
      "                         120         Водный  водный              30000   "
    */
    //                                  1dop           2Comment   3Short name 4Form   5Position-InForm     6Name     7Encoding data
    QRegExp searchDependFields("\\t{7}([^\\t]*)\\t{3}([^\\t]+)\\t([^\\t]*)\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)");
    int count = -1;
    int countDependFields = 0;
    QVariantList dependFieldList = QVariantList();

    foreach(QString textLine, textSplitted)
    {
        if (search.indexIn(textLine) != -1)
        {
            capturedText = search.capturedTexts();

            if (countDependFields)
            {
                elements[count] = getNewAlignedCorrelationsList(countDependFields, elements[count].toMap(), &dependFieldList);
                countDependFields = 0;
                dependFieldList.clear();
            }

            elements.append(fillOneElement(capturedText));
            count++;
        }
        else
        {
            if (searchDependFields.indexIn(textLine) != -1 && count != -1)
            {
                capturedText = searchDependFields.capturedTexts();
                dependFieldList.append(setDependFieldInfo(capturedText, ++countDependFields));
            }
            else
            {
                // TODO: qRgeExp
                qWarning() << " Line in Description file is incorrect => " << textLine;
            }
        }
    }
    if (count != -1 && countDependFields)
    {
        elements[count] = getNewAlignedCorrelationsList(countDependFields, elements[count].toMap(), &dependFieldList);
        countDependFields = 0;
        dependFieldList.clear();
    }

    return elements;
}

QVariantMap QModelDescribingFromJura::getNewAlignedCorrelationsList(int countDependFields,
        const QVariantMap& element,
        QVariantList* dependFieldList)
{
    QVariantMap elementMap = element;
    int valueOfOnesAndZeros = FromIntegerToBinaryString(countDependFields).size();
    alignOneAndZerosInCorrelationCodes(valueOfOnesAndZeros,
                                       dependFieldList);
    elementMap.insert(dependFields, *dependFieldList);

    return elementMap;
}

QString QModelDescribingFromJura::FromIntegerToBinaryString(int countDependFields)
{
    QString binaryString = QString();
    QString reverseBinaryString = QString();

    int bynary = 1 << (countDependFields  - 1);

    while (bynary > 1)
    {
        reverseBinaryString.append(QString::number(bynary % 2));
        bynary /= 2;
    }
    reverseBinaryString.append(QString::number(bynary));
    int reverseStringSize = reverseBinaryString.count();
    for (int i = 0; i < reverseStringSize; ++i)
    {
        binaryString.append(reverseBinaryString.at(reverseStringSize - i - 1));
    }
    return binaryString;
}

void QModelDescribingFromJura::alignOneAndZerosInCorrelationCodes(int valueOfOnesAndZeros, QVariantList* dependList)
{
    int diff = 0;
    QString tempElement = QString();
    QVariantMap tempMap = QVariantMap();
    QString nullString = QString();
    for (int i = 0; i < dependList->size(); ++i)
    {
        diff = valueOfOnesAndZeros - (*dependList)[i].toMap().value(correlationValue).toString().size() ;
        if (diff > 0)
        {
            tempMap = (*dependList)[i].toMap();
            tempElement = tempMap.value(correlationValue).toString();
            nullString.fill('0', diff);
            tempElement = nullString + tempElement;
            tempMap.insert(correlationValue, tempElement);
            (*dependList)[i] = tempMap;
        }
    }
}

QVariantMap QModelDescribingFromJura::setDependFieldInfo(const QStringList& capturedText, int countDependFields)
{
    QVariantMap element;
    element.insert(dependId, QString(" "));
    if (!capturedText.at(1).isEmpty())
    {
        element.insert(dependId, elementName + capturedText.at(1));
    }
    element.insert(name, capturedText.at(2));
    element.insert(formId, getElementNameByCodeForm(capturedText.at(4)));//F5 , F1 , F2 or F12
    element.insert(targetName, getElementNameByCodeForm(capturedText.at(4)) + underline + capturedText.at(6));
    element.insert(targetDataForConvert, setTargetDataForConvert(capturedText.at(7)));
    element.insert(correlationValue, FromIntegerToBinaryString(countDependFields));

    return element;
}

QVariantMap QModelDescribingFromJura::fillOneElement(const QVariantMap& tmpElement)
{
    QVariantMap element;
    element.insert(id, elementName + tmpElement.value(id).toString());
    element.insert(level, tmpElement.value(level));
    element.insert(type, tmpElement.value(type));
    element.insert(name, tmpElement.value(name));
    element.insert(hint, tmpElement.value(hint));
    if (!tmpElement.value(formId).isNull())
    {
        element.insert(formId, getElementNameByCodeForm(tmpElement.value(formId).toString()));
    }

    if (!tmpElement.value(targetName).isNull() && !tmpElement.value(formId).isNull())
    {
        element.insert(targetName, getElementNameByCodeForm(tmpElement.value(formId).toString()) + underline +
                       tmpElement.value(targetName).toString());
    }
    if (!tmpElement.value(targetDataForConvert).isNull())
    {
        element.insert(targetDataForConvert,
                       setTargetDataForConvert(tmpElement.value(targetDataForConvert).toString()));
    }
    return element;
}

QVariantMap QModelDescribingFromJura::setDependFieldInfo(const QVariantMap& tmpElement, int countDependFields)
{
    QVariantMap element;
    element.insert(dependId, QString(" "));
    if (!tmpElement.value(dependId).isNull())
    {
        element.insert(dependId, elementName + tmpElement.value(dependId).toString());
    }
    element.insert(name, QString(" "));
    if (!tmpElement.value(name).isNull())
    {
        element.insert(name, tmpElement.value(name));
    }
    element.insert(formId, QString(" "));
    if (!tmpElement.value(formId).isNull())
    {
        element.insert(formId, getElementNameByCodeForm(tmpElement.value(formId).toString()));
    }
    element.insert(targetName, QString(" "));
    if (!tmpElement.value(formId).isNull() && !tmpElement.value(targetName).isNull())
    {
        element.insert(targetName, getElementNameByCodeForm(tmpElement.value(formId).toString()) + underline + tmpElement.value(targetName).toString()) ;
    }
    qDebug() << element.value(targetName);
    element.insert(targetDataForConvert, QString(" "));
    if (!tmpElement.value(targetDataForConvert).isNull())
    {
        element.insert(targetDataForConvert, setTargetDataForConvert(tmpElement.value(targetDataForConvert).toString()));
    }
    element.insert(correlationValue, FromIntegerToBinaryString(countDependFields));
    return element;
}

QVariantMap QModelDescribingFromJura::fillOneElement(const QStringList& capturedText)
{
    QVariantMap element;
    qDebug() << capturedText;
    element.insert(id, elementName + capturedText.at(1));
    element.insert(level, capturedText.at(2));
    element.insert(type, capturedText.at(3));
    element.insert(name, capturedText.at(5));
    element.insert(hint, capturedText.at(5));
    if (!capturedText.at(6).isEmpty())
    {
        element.insert(name, capturedText.at(6));
    }

    //For av, dv, cb these fields should be empty
    //F5_text
    element.insert(formId, getElementNameByCodeForm(capturedText.at(7))); //F5 , F1 , F2 or F12
    element.insert(targetName, getElementNameByCodeForm(capturedText.at(7)) + underline + capturedText.at(9));
    element.insert(targetDataForConvert, setTargetDataForConvert(capturedText.at(10)));

    element.insert(dependFields, QVariantList());

    return element;
}

QString QModelDescribingFromJura::getElementNameByCodeForm(const QString& codeForm)
{
    QString elName = "U";
    if (codeForm == "F5" ||
        codeForm == QString::fromUtf8("Ф5"))
    {
        //elName = locus;
        elName = "F5";
    }
    else if (codeForm == "F1" ||
             codeForm == QString::fromUtf8("Ф1"))
    {
        //elName = generic;
        elName = "F1";
    }
    else if (codeForm == "F2" ||
             codeForm == QString::fromUtf8("Ф2"))
    {
        //elName = figurant;
        elName = "F2";
    }
    else if (codeForm == "F12" ||
             codeForm == QString::fromUtf8("Ф12"))
    {
        //elName = weapon;
        elName = "F12";
    }

    return elName;
}

QString QModelDescribingFromJura::setTargetDataForConvert(const QString& dataFor)
{
    QString code = "";// Take value only
    QString dataForConvert = dataFor;
    if (dataForConvert.contains("X", Qt::CaseInsensitive) ||
        dataForConvert.contains(QString::fromUtf8("Х"), Qt::CaseInsensitive))
    {
        // Compact code. Value = compactXXX3XXX
        code = (compactCode + dataForConvert);
    }
    else if (dataForConvert.startsWith("k", Qt::CaseInsensitive) ||
             dataForConvert.startsWith(QString::fromUtf8("к"), Qt::CaseInsensitive))
    {
        // Simple code. Value = code0232
        dataForConvert.remove(0, 1); // Remove k
        code = (simpleCode + dataForConvert);
    }

    return code;
}

bool QModelDescribingFromJura::checkFileStructure(QTextStream* fileStream)
{
    qDebug() << "Not implemented yet. Accept all files";
    Q_UNUSED(fileStream);
    bool accept = true;

    return accept;
}
