#include <QFile>

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

QVariantList QModelDescribing::getVisibleElements() const
{
    return VisibleElementsFromDescriptionFiles;
}

QVariantList QModelDescribing::getElementsWithData() const
{
    return VisibleElementWithData;
}
void QModelDescribing::appendToList(const QString& filename)
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
void QModelDescribing::addingDataToBlankElements(QTextStream* fileStream)
{
    qDebug() << " Start";
    QMap<QString, QVariant> *oneRecord;
    while (!fileStream->atEnd())
    {
        QString line = fileStream->readLine();
        oneRecord = processLineInDescriptionFile(line);
        if (oneRecord != NULL)
        {
            addNextElementToList(*oneRecord);
            delete oneRecord;
        }
    }
    qDebug() << " End";
}

void QModelDescribing::addNextElementToList(QMap<QString, QVariant> & oneRec)
{
    oneRec.insert(numb, ElementsFromDescriptionFiles.count());
    ElementsFromDescriptionFiles.append(oneRec);
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
    qDebug();
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

void QModelDescribing::createTreeFromElements(const QVariantList& iList, int i, int levels, QStandardItem* parent)
{
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

QString QModelDescribing::readSymbolFromString(const QString& line, int& k)
{
    QString Element = "";
    while (line[k] != '\t')
    {
        Element += line[k];
        k++;
        if (k >= line.count())
        {
            qWarning() << "Data out of range!!!";
            return NULL;
        }
    }
    return Element;

}
bool QModelDescribing::turn(const QString& line, int& k, int cTurn)
{
    int count = 0;
    while (count < cTurn)
    {
        if (k >= line.count())
        {
            qWarning() << "Data out of range!!!";
            return false;
        }
        if (line[k] == '\t') ++count;
        k++;
    }
    return true;
}

bool QModelDescribing::isValidElementsWithoutData()
{
    return VisibleElementsFromDescriptionFiles.count() > 0;
}

bool QModelDescribing::isValidElementsWithData()
{
    return VisibleElementWithData.count() > 0;
}

//Заполнить лист значимых элементов
void QModelDescribing::createListofVisibleElements()
{
    int count = ElementsFromDescriptionFiles.count();
    for (int i = 0; i < count; i++)
    {
        if (isVisibleElement(ElementsFromDescriptionFiles[i]))
        {
            VisibleElementsFromDescriptionFiles.append(ElementsFromDescriptionFiles[i]);
        }
    }
}
bool QModelDescribing::isVisibleElement(const QVariant& value)
{
    return (value.toMap().value(type).toString() != RM);
}

QVariant QModelDescribing::findByIdInVisibleElements(const QVariant& uid)
{
    for (int i = 0; i < VisibleElementsFromDescriptionFiles.count() ; i++)
    {
        if (VisibleElementsFromDescriptionFiles[i].toMap().value(id) == uid)
        {
            return VisibleElementsFromDescriptionFiles[i];
        }
    }
    return QVariant();
}



void QModelDescribing::setElementNameByFile(const QString& filename)
{
    //Move to each class
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

void QModelDescribing::clearElementsWithData()
{
    VisibleElementWithData.clear();
}

void QModelDescribing::    clearVisibleElements()
{
    VisibleElementsFromDescriptionFiles.clear();
}

void QModelDescribing::clearElementsWithoutData()
{
    ElementsFromDescriptionFiles.clear();
}

void QModelDescribing::clearAllElements()
{
    this->clear();
    clearElementsWithoutData();
    clearElementsWithData();
    clearVisibleElements();
}

bool QModelDescribing::findByUIdInVisibleElements(const QVariant& uid, int& pos)
{
    int i =  0;
    foreach(QVariant oneRec, VisibleElementsWithDataForParticularFile)
    {
        if (oneRec.toMap( ).value(id) == uid)
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
    for (int i = 0; i < ElementsFromDescriptionFiles.count(); i++)
    {
        if (ElementsFromDescriptionFiles[i].toMap().value(id).toString().contains(elementName))
        {
            retVar.append(ElementsFromDescriptionFiles[i]);
        }
    }
    return retVar;
}
