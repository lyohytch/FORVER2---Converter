#include "qmodeldescribingfromjura.h"

QModelDescribingFromJura::QModelDescribingFromJura(QObject* parent):
        QModelDescribing(parent)
{
}

QVariantList QModelDescribingFromJura::getElementsFromText(QTextStream* fileStream)
{
    QVariantList elements = QVariantList();
    QString text = fileStream->readAll();
    QStringList textSplitted = text.split(QRegExp("\\n"));
    QStringList capturedText = QStringList();
    // TODO: rework regexp
    /**
      It's regular expression for strings like
      "101	2					ed				Год возбуждения		Ф5	3	Г	знач"
    */
    //               1id          2level          3type       4dop        Mask    Config    5Comment   6Short Name  7Form      8Position-InForm    9Name      10Encoding data
    QRegExp search("([^\\t]+)\\t([^\\t]+)\\t{5}([^\\t]+)\\t([^\\t]*)\\t[^\\t]*\\t[^\\t]*\\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)");
    /**
      It's regular expression for strings like
      "							120			Водный	водный				30000	"
    */
    //                                  1dop           2Comment   3Short name 4Form   5Position-InForm     6Name     7Encoding data
    QRegExp searchDependFields("\\t{7}([^\\t]*)\\t{3}([^\\t]+)\\t([^\\t]*)\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)");
    int count = -1;
    int countDependFields = 0;
    QVariantList dependFieldList = QVariantList();

    foreach(QString textLine, textSplitted)
    {
         if(search.indexIn(textLine) != -1)
         {
            capturedText = search.capturedTexts();

            if(countDependFields)
            {
                elements[count] = getNewAlignedCorrelationsList(countDependFields, elements[count].toMap(), &dependFieldList);
                //qDebug() << "elements => "<<elements[count].toMap().value(dependFields);
            }

            elements.append(fillOneElement(capturedText));

            count++;
            countDependFields = 0;
            dependFieldList.clear();
         }
         else
         {
             if( searchDependFields.indexIn(textLine) != -1 && count != -1)
             {
                    capturedText = searchDependFields.capturedTexts();
                    dependFieldList.append(setDependFieldInfo(capturedText, ++countDependFields));
             }
             else
             {
                // TODO: qRgeExp
                qWarning()<<" Line in Description file is incorrect => "<< textLine;
             }
         }
    }
    return elements;
}

QVariantMap QModelDescribingFromJura::getNewAlignedCorrelationsList(int countDependFields,
                                                                    const QVariantMap &element,
                                                                    QVariantList *dependFieldList)
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

   while(countDependFields > 1)
   {
       reverseBinaryString.append(QString::number(countDependFields % 2));
       countDependFields /= 2;
   }
   reverseBinaryString.append(QString::number(countDependFields));
   int reverseStringSize = reverseBinaryString.count();
   for(int i = 0; i < reverseStringSize; ++i)
   {
        binaryString.append(reverseBinaryString.at(reverseStringSize - i - 1));
   }

   return binaryString;
}

void QModelDescribingFromJura::alignOneAndZerosInCorrelationCodes(int valueOfOnesAndZeros, QVariantList *dependList)
{
    int diff = 0;
    QString tempElement = QString();
    QVariantMap tempMap = QVariantMap();
    QString nullString = QString();
    for(int i = 0; i < dependList->size(); ++i)
    {
        diff = valueOfOnesAndZeros - (*dependList)[i].toMap().value(correlationValue).toString().size() ;
        if(diff > 0)
        {
            tempMap =  (*dependList)[i].toMap();
            tempElement = tempMap.value(correlationValue).toString();
            nullString.fill('0', diff);
            tempElement = nullString + tempElement;
            tempMap.insert(correlationValue, tempElement);
            (*dependList)[i] = tempMap;
        }
    }
}

QVariantMap QModelDescribingFromJura::setDependFieldInfo(const QStringList &capturedText, int countDependFields)
{
    QVariantMap element;
    element.insert(dependId, QString(" "));
    if(!capturedText.at(1).isEmpty())
    {
        element.insert(dependId, elementName + capturedText.at(1));
    }
    //For av, dv, cb these fields should be empty
    element.insert(formId, getElementNameByCodeForm(capturedText.at(4)));//F5 , F1 , F2 or F12
    element.insert(targetName, getElementNameByCodeForm(capturedText.at(4)) + underline + capturedText.at(6));
    element.insert(targetDataForConvert, setTargetDataForConvert(capturedText.at(7)));
    element.insert(correlationValue, FromIntegerToBinaryString(countDependFields) );

    return element;
}

QVariantMap QModelDescribingFromJura::fillOneElement(const QStringList & capturedText)
{
   QVariantMap element;
   qDebug() << capturedText;
   element.insert(id, elementName + capturedText.at(1));
   element.insert(level, capturedText.at(2));
   element.insert(type, capturedText.at(3));
   element.insert(name, capturedText.at(5));
   if(!capturedText.at(6).isEmpty())
   {
        element.insert(name, capturedText.at(6));
   }

   //For av, dv, cb these fields should be empty
   //F5_text
   element.insert(formId,getElementNameByCodeForm(capturedText.at(7)));//F5 , F1 , F2 or F12
   element.insert(targetName, getElementNameByCodeForm(capturedText.at(7)) + underline + capturedText.at(9));
   element.insert(targetDataForConvert, setTargetDataForConvert(capturedText.at(10)));

   element.insert(dependFields, QVariantList());

   return element;
}

QString QModelDescribingFromJura::getElementNameByCodeForm(const QString &codeForm)
{
    QString elName = "U";
    if(codeForm == "F5" ||
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

QString QModelDescribingFromJura::setTargetDataForConvert(const QString & dataFor)
{
    QString code = "";// Take value only
    QString dataForConvert = dataFor;
    if(dataForConvert.contains("X", Qt::CaseInsensitive) ||
       dataForConvert.contains(QString::fromUtf8("Х"), Qt::CaseInsensitive))
    {
        // Compact code. Value = compactXXX3XXX
        code =  (compactCode + dataForConvert);
    }
    else if(dataForConvert.startsWith("k", Qt::CaseInsensitive) ||
            dataForConvert.startsWith(QString::fromUtf8("к"), Qt::CaseInsensitive))
        {
             // Simple code. Value = code0232
            dataForConvert.remove(0,1); // Remove k
            code =  (simpleCode + dataForConvert);
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
