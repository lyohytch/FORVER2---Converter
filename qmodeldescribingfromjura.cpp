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
      ([^\\t]+)\\t([^\\t]+)\\t{5}([^\\t]+)\\t([^\\t]*)\\t[^\\t]*\\t[^\\t]*\\t([^\\t]*)\\t[^\\t]*\\t[^\\t]*\\t([^\\t]*)"
      It' s regular expression for strings like
      "244	5					ed		1		ФИО:	888888888888888888888888	ФИО:	27		221		"
    */
    QRegExp search("([^\\t]+)\\t([^\\t]+)\\t{5}([^\\t]+)\\t([^\\t]*)\\t[^\\t]*\\t[^\\t]*\\t([^\\t]*)\\t[^\\t]*\\t[^\\t]*\\t([^\\t]*)");
    QRegExp searchDependFields("\\t{7}([^\\t]+)");
    int count = 0;
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
             if( searchDependFields.indexIn(textLine) != -1)
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
   element.insert(repeat, capturedText.at(6));
   return element;
}

bool QModelDescribingFromJura::checkFileStructure(QTextStream* fileStream)
{
    qDebug() << "Not implemented yet. Accept all files";
    Q_UNUSED(fileStream);
    bool accept = true;

    return accept;
}
