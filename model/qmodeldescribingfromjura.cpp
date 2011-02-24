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
    //               1id          2level          3type       4dop        Mask    Config    5Comment   6Short Name  7Form       8Position    9Name      10Encoding data
    QRegExp search("([^\\t]+)\\t([^\\t]+)\\t{5}([^\\t]+)\\t([^\\t]*)\\t[^\\t]*\\t[^\\t]*\\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)");
    /**
      It's regular expression for strings like
      "							120			Водный	водный				30000	"
    */
    //                                  1dop           2Comment   3Short name 4Form   5Position     6Name     7Encoding data
    QRegExp searchDependFields("\\t{7}([^\\t]*)\\t{3}([^\\t]+)\\t([^\\t]*)\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)\\t([^\\t]*)");
    int count = -1;
    int countDependFields = 0;
    QVariantList dependFieldList = QVariantList();

    foreach(QString textLine, textSplitted)
    {
         if(search.indexIn(textLine) != -1)
         {
            capturedText = search.capturedTexts();

            qDebug()<<capturedText;
            // TODO: move this block to particular function
            if(countDependFields)
            {
                //Выравниваем количество нулей и единиц
                int valueOfOnesAndZeros = FromIntegerToBinaryString(countDependFields).size();
                qDebug()<< dependFieldList;
                alignOneAndZerosInCorrelationCodes(valueOfOnesAndZeros,
                                                   &dependFieldList);

                elements[count].toMap().insert(dependFields, dependFieldList);

                qDebug() << elements[count].toMap().value(dependFields);
            }
            // TODO: move this block to particular function

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

                    qDebug()<<capturedText;

                    dependFieldList.append(setDependFieldInfo(capturedText, ++countDependFields));

                    qDebug()<< dependFieldList;
             }
             else
             {
                // TODO: qRgeExp
                qWarning()<<" Line in Description file is incorrect => "<< textLine;
             }
         }
    }
    qDebug()<< "end. Elements =>"<< elements;
    return elements;
}

QString QModelDescribingFromJura::FromIntegerToBinaryString(int countDependFields)
{
   QString binaryString;
   QString reverseBinaryString;

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
    int diff;
    foreach(QVariant dependElement, *dependList)
    {
        diff = valueOfOnesAndZeros - dependElement.toMap().value(correlationValue).toString().size() ;
        if(diff > 0)
        {
            dependElement.toMap().value(correlationValue).toString().insert(0, &QChar('0'), diff);
        }
    }
}

QVariantMap QModelDescribingFromJura::setDependFieldInfo(const QStringList &capturedText, int countDependFields)
{
    QVariantMap element;
    if(!capturedText.at(1).isEmpty())
    {
        element.insert(dependId, elementName + capturedText.at(1));
    }
    //For av, dv, cb these fields should be empty
    element.insert(targetId, generic + capturedText.at(4) + underline + capturedText.at(5));
    element.insert(targetName, capturedText.at(6));
    element.insert(targetDataForConvert, setTargetDataForConvert(capturedText.at(7)));
    element.insert(correlationValue, FromIntegerToBinaryString(countDependFields) );


    qDebug()<<element;


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
   element.insert(targetId, generic + capturedText.at(7) + underline + capturedText.at(8));
   element.insert(targetName, capturedText.at(9));
   element.insert(targetDataForConvert, setTargetDataForConvert(capturedText.at(10)));

   element.insert(dependFields, QVariantList());

   return element;
}

QString QModelDescribingFromJura::setTargetDataForConvert(const QString & dataForConvert)
{
    QString code = "";// Take value only
    if(dataForConvert.contains("X", Qt::CaseInsensitive) ||
       dataForConvert.contains(QString::fromUtf8("Х"), Qt::CaseInsensitive))
    {
        // Compact code. Value = compactXXX3XXX
        return (compactCode + dataForConvert);
    }
    if(dataForConvert.startsWith("k", Qt::CaseInsensitive) ||
       dataForConvert.startsWith(QString::fromUtf8("к"), Qt::CaseInsensitive))
    {
        // Simple code. Value = code0232
        return (simpleCode + dataForConvert);
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
