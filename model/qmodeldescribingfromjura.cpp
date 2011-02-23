#include "qmodeldescribingfromjura.h"

QModelDescribingFromJura::QModelDescribingFromJura(QObject* parent):
        QModelDescribing(parent)
{
}

QVariantList QModelDescribingFromJura::getElementsFromText(QTextStream* fileStream)
{
    QVariantList elements;
    QString text = fileStream->readAll();
    QStringList textSplitted = text.split(QRegExp("\\n"));
    QStringList capturedText;
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
    //                                  1dop           2Comment   3Short name     4Encoding data
    QRegExp searchDependFields("\\t{7}([^\\t]*)\\t{3}([^\\t]*)\\t([^\\t]*)\\t{4}([^\\t]*)");
    int count = -1;

    foreach(QString textLine, textSplitted)
    {
         if(search.indexIn(textLine) != -1)
         {
            capturedText = search.capturedTexts();
            elements.append(fillOneElement(capturedText));
            count++;
         }
         else
         {
             if( searchDependFields.indexIn(textLine) != -1 && count != -1)
             {
                 elements[count].toMap().value(dependFields).toList().append(searchDependFields.cap(1));
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

QVariantMap QModelDescribingFromJura::fillOneElement(const QStringList & capturedText)
{
   QVariantMap element;
   element.insert(id, elementName + capturedText.at(1));
   element.insert(level, capturedText.at(2));
   element.insert(type, capturedText.at(3));
   element.insert(name, capturedText.at(5));
   if(!capturedText.at(6).isEmpty())
   {
        element.insert(name, capturedText.at(6));
   }

   return element;
}

bool QModelDescribingFromJura::checkFileStructure(QTextStream* fileStream)
{
    qDebug() << "Not implemented yet. Accept all files";
    Q_UNUSED(fileStream);
    bool accept = true;

    return accept;
}
