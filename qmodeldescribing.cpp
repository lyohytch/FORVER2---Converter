#include <QFile>
#include <QTextCodec>

#include "qmodeldescribing.h"

QModelDescribing::QModelDescribing(QObject* parent):
    QStandardItemModel(parent)
{
}

QModelDescribing::QModelDescribing(const QModelDescribing& other):
    QStandardItemModel()
{
    ElementsFromDescriptionFiles = other.getElementsWithoutData();
    isElementWithData = other.checkElementWithDataOrNot();
}


QVariantList QModelDescribing::getElementsWithoutData() const
{
    return ElementsFromDescriptionFiles;
}

bool QModelDescribing::checkElementWithDataOrNot() const
{
    return isElementWithData;
}

QVariantList QModelDescribing::getVisibleElements() const
{
    return VisibleElementsFromDescriptionFiles;
}

QVariantList QModelDescribing::getElementsWithData() const
{
    return VisibleElementWithData;
}

void QModelDescribing::appendFromDataFilesToDataElements(const QString& filename)
{
    qDebug();
    //Open file
    QFile fileSource(filename);
    if (fileSource.exists())
    {
        qDebug() << "File found. Try to take list";
        if (!fileSource.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qCritical() << "File " << filename << " can not be open.";
        }
        else
        {
            QTextCodec *txtCodec = setFileEncodingByContain(&fileSource);

            fileSource.close();
            fileSource.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream fileStream(&fileSource);
            fileStream.setCodec(txtCodec);

            if (checkFileStructure(&fileStream))
            {
                setElementNameByFile(filename);
                addingDataToBlankElements(&fileStream);
            }
            else
            {
                qWarning() << "File structure is incorrect.";
            }
        }
    }
    else
    {
        qCritical() << "File " << filename << " doesn't exist";
    }
}

void QModelDescribing::setElementNameByFile(const QString& filename)
{
    //Move to each class
    // TODO: reqork it
    if (filename.endsWith("Sprav1.txt", Qt::CaseSensitive) ||
        filename.endsWith("sprav_d.txt", Qt::CaseSensitive) ||
        filename.endsWith("F1.TXT", Qt::CaseSensitive))
    {
        elementName = generic;
    }
    else if (filename.endsWith("Sprav2.txt", Qt::CaseSensitive) ||
             filename.endsWith("F2.TXT", Qt::CaseSensitive))
    {
        elementName = figurant;
    }
    else if (filename.endsWith("Sprav3.txt", Qt::CaseSensitive) ||
             filename.endsWith("F5.TXT", Qt::CaseSensitive))
    {
        elementName = locus;
    }
    else if (filename.endsWith("Sprav4.txt", Qt::CaseSensitive) ||
             filename.endsWith("F12.TXT", Qt::CaseSensitive))
    {
        elementName = weapon;
    }
    else
    {
        //Adding by default
        elementName = "";
    }
}

void QModelDescribing::addingDataToBlankElements(QTextStream* fileStream)
{
    qDebug() << " Start";
    QVariantList addingElements = getElementsFromText(fileStream);
    addNextElementsToList(addingElements);
    qDebug() << " End";
}

QVariantList QModelDescribing::getElementsFromText(QTextStream* fileStream)
{
    QVariantList elements;
    //NOTE: some text should be read from UTF8(QString::fromUtf8(char *) )
    QString text = fileStream->readAll();
    QStringList textSplitted = text.split(QRegExp("\\n"));
    QStringList capturedText;
    /**
      ([^\\t]+)\\t([^\\t]+)\\t{5}([^\\t]+)\\t([^\\t]*)\\t[^\\t]*\\t[^\\t]*\\t([^\\t]*)\\t[^\\t]*\\t[^\\t]*\\t([^\\t]*)"
      It' s regular expression for strings like
      "244	5					ed		1		ФИО:	888888888888888888888888	ФИО:	27		221		"
    */
    QRegExp search("([^\\t]+)\\t([^\\t]+)\\t{5}([^\\t]+)\\t([^\\t]*)\\t[^\\t]*\\t[^\\t]*\\t([^\\t]*)\\t[^\\t]*\\t[^\\t]*\\t([^\\t]*)");
    foreach(QString textLine, textSplitted)
    {
         if(search.indexIn(textLine) != -1)
         {
            capturedText = search.capturedTexts();
            elements.append(fillOneElement(capturedText));
         }
         else
         {
             //TODO: to file
            qWarning()<<" Line in Description file is incorrect => "<< textLine;
         }
    }
    qDebug()<< "end. Elements =>"<< elements;
    return elements;
}

/** Read all lines in file. Need to close and open again file after using function
  */
QTextCodec* QModelDescribing::setFileEncodingByContain(QFile *filesource)
{
    QTextCodec *textCodec = QTextCodec::codecForName("Windows-1251");
    QTextStream outStream(filesource);
    QString text = outStream.readAll();
    if( textCodec->canEncode(text) )
    {
        return textCodec;
    }
    else
    {
        textCodec = QTextCodec::codecForName("UTF-8");
        if ( textCodec->canEncode(text))
        {
            return textCodec;
        }
        else
        {
            qWarning()<<"Unknown encoding";
        }
    }
    return textCodec;
}

void QModelDescribing::addNextElementsToList(const QVariantList & oneRec)
{
    foreach(QVariant ElementFromList, oneRec)
    {
        ElementFromList.toMap().insert(numb, ElementsFromDescriptionFiles.count());
        ElementsFromDescriptionFiles.append(ElementFromList);
    }
}

void QModelDescribing::loadingDataElementsFromFile(const QString& filename)
{
    qDebug();
    //Open file
    QFile fileSource(filename);
    if (fileSource.exists())
    {
        qDebug() << "File found. Try to take list";
        if (!fileSource.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qCritical() << "File " << filename << " can not be open.";
        }
        else
        {
            QTextStream fileStream(&fileSource);
            if (checkFileFileStructureData(&fileStream))
            {
                setElementNameByDataFile(filename);
                addingLoadedDataInVisibleElementsWithData(&fileStream);
            }
            else
            {
                qWarning() << "File structure is incorrect.";
            }
        }
    }
    else
    {
        qCritical() << "File " << filename << " doesn't exist";
    }
}

bool QModelDescribing::createTreeForViewing()
{
    //Заполнить значимые элементы
    createListofVisibleElements();
    //Здесь мы заполняем QStandartItemModel
    QStandardItem* rootItem = new QStandardItem("Model");
    //Fill in list QStandardItem
    createTreeFromElements(ElementsFromDescriptionFiles, 0, 0, rootItem);
    //Append children of rootItem
    this->appendRow(rootItem);
    return true;
}

void QModelDescribing::createListofVisibleElements()
{
    foreach (QVariant ElementFromList, ElementsFromDescriptionFiles)
    {
        if (isVisibleElement(ElementFromList))
        {
            VisibleElementsFromDescriptionFiles.append(ElementFromList);
        }
    }
}

bool QModelDescribing::isVisibleElement(const QVariant& value)
{
    return (value.toMap().value(type).toString() != RM);
}

void QModelDescribing::createTreeFromElements(const QVariantList& iList, int i, int levels, QStandardItem* parent)
{
    //TODO: check for errors
    if (i >= iList.count())
    {
        return;
    }
    QStandardItem* child = new QStandardItem(iList[i].toMap().value(name).toString());
    child->setEditable(false);
    child->setData(iList[i], Qt::UserRole + 1);
    if (isVisibleElement(iList[i]))
    {
        QFont font = child->font();
        font.setBold(true);
        child->setFont(font);
    }
    int levelcur = iList[i].toMap().value(level).toInt();
    int diff = levelcur  - levels;
    switch (diff)
    {
        case 0:   // Уровень совпадает. Делаем следующий элемент ребёнком родителя parent
            {
                i++;
                parent->parent()->appendRow(child);
                createTreeFromElements(iList, i, levelcur, child);
            }
            break;
        case 1:   //Уровень следующего на 1 больше. Делаем след элемент ребёнком pare nt
            {
                i++;
                parent->appendRow(child);
                createTreeFromElements(iList, i, levelcur, child);
            }
            break;
        case -1:   //Уровень след на 1 меньше. Делаем след эл ребёнком дедушки parent
            {
                i++;
                parent->parent()->parent()->appendRow(child);
                createTreeFromElements(iList, i, levelcur, child);
            }
            break;
        default:   //Уровень след меньше , чем на 1
            {
                if (!child)
                {
                    delete child;
                }
                createTreeFromElements(iList, i, levels - 1, parent->parent());
            }
            break;
    }
}

bool QModelDescribing::isValidStringInDescriptionFileToAdd(const QMap<QString, QVariant> &checkMap)
{
    bool isNull = ((checkMap.value(id ) == NULL) || (checkMap.value(level) == NULL) ||
                   (checkMap.value(name) == NULL) || (checkMap.value(type) == NULL));
    if (!isNull)
        return ((checkMap.value(type).toString() == AV) || (checkMap.value(type).toString() == DV) || (checkMap.value(type).toString() == RM) ||
                (checkMap.value(type).toString() == CB) || (checkMap.value(type).toString() == ED) || (checkMap.value(type).toString() == ET) ||
                (checkMap.value(type).toString() == ME)) ;
    return false;
}

//TODO: remove unnecessary methods
QString QModelDescribing::readSymbolsFromString(const QString& line, int& k)
{
    QString Element = "";
    int bound = line.count();
    while (line[k] != '\t')
    {
        Element += line[k];
        k++;
        if (k >= bound)
        {
            return NULL;
        }
    }
    return Element;

}
bool QModelDescribing::turn(const QString& line, int& k, int cTurn)
{
    int count = 0;
    int bound = line.count();
    while (count < cTurn)
    {
        if (k >= bound)
        {
            return false;
        }
        if (line[k] == '\t') ++count;
        k++;
    }
    return true;
}
//TODO: remove unnecessary methods

bool QModelDescribing::isValidElementsWithoutData()
{
    return VisibleElementsFromDescriptionFiles.count() > 0;
}

bool QModelDescribing::isExistElementsWithoutData()
{
    return ElementsFromDescriptionFiles.count() > 0;
}

bool QModelDescribing::isValidElementsWithData()
{
    return VisibleElementWithData.count() > 0;
}


QVariant QModelDescribing::findByIdInVisibleElements(const QVariant& uid)
{
    foreach(QVariant ElementFromList, VisibleElementsFromDescriptionFiles)
    {
        if (ElementFromList.toMap().value(id) == uid)
        {
            return ElementFromList;
        }
    }
    return QVariant();
}

void QModelDescribing::clearAllElements()
{
    this->clear();
    clearElementsWithoutData();
    clearElementsWithData();
    clearVisibleElements();
}

void QModelDescribing::clearElementsWithData()
{
    VisibleElementWithData.clear();
}

void QModelDescribing::clearVisibleElements()
{
    VisibleElementsFromDescriptionFiles.clear();
}

void QModelDescribing::clearElementsWithoutData()
{
    ElementsFromDescriptionFiles.clear();
}

bool QModelDescribing::findByUIdInVisibleElements(const QVariant& uid, int& pos)
{
    int i =  0;
    foreach(QVariant ElementFromList, VisibleElementsWithDataForParticularFile)
    {
        if (ElementFromList.toMap( ).value(id) == uid)
        {
            pos = i;
            return true;
         }
        i++;
    }

    return false;
}

QVariantList QModelDescribing::initDataStructure()
{
    QVariantList retVar;
    foreach (QVariant ElementFromList, ElementsFromDescriptionFiles)
    {
        if (ElementFromList.toMap().value(id).toString().contains(elementName))
        {
            retVar.append(ElementFromList);
        }
    }
    return retVar;
}
