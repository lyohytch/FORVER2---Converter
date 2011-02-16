#ifndef QMODELDESCRIBING_H
#define QMODELDESCRIBING_H

#include <QVariantList>
#include <QString>
#include <QTextStream>
#include <QStandardItemModel>

#include "constants.h"

//TODO: describe functions
/** Class provide bla-bla-bla
 *
 */

class QModelDescribing : public QStandardItemModel
{
        Q_OBJECT

    protected:

        QString elementName;//"generic_","figurant_","locus_delicti_","weapon_"

        QVariantList ElementsFromDescriptionFiles; //List of maps like (id,name,type,level,value) value == NULL for Description Files. "Element" is one element from this list

        QVariantList VisibleElementsFromDescriptionFiles;

        QVariantList VisibleElementWithData;// QList<  [[[  QList<QMap<QString, QVariant> > ]]]  >

        QVariantList VisibleElementsWithDataForParticularFile;//List of maps (id, value) where value is data from particular file

        //Operations with elements without data
        virtual bool checkFileStructure(QTextStream* /*fileStream*/) = 0;

        virtual QMap<QString, QVariant>* processLineInDescriptionFile(const QString& /*line*/) = 0;

        void addingDataToBlankElements(QTextStream* fileStream);

        void createTreeFromElements(const QVariantList& iList, int i, int levels, QStandardItem* parent);

        virtual bool isValidStringInDescriptionFileToAdd(const QMap<QString, QVariant> &checkMap);

        virtual void addNextElementToList(QMap<QString, QVariant> & oneRec);

        virtual QString readSymbolFromString(const QString& line, int& k);

        virtual bool turn(const QString& line, int& k, int cTurn);

        virtual void createListofVisibleElements();

        //Operations with elements with data
        virtual bool checkFileFileStructureData(QTextStream* /*fileStream*/) = 0;

        virtual QVariantList processLineInDataFile(const QString& /*line*/, const QVariantList &/*DataStructure*/) = 0;

        virtual bool isValidStringInDataFileToAdd(const QVariantList &/*checkMap*/) = 0;

        virtual void addingLoadedDataInVisibleElementsWithData(QTextStream* /*fileStream*/) = 0;

        virtual void setElementNameByDataFile(const QString &/*filename*/) = 0;

        virtual  QVariantList initDataStructure();

        virtual bool findByUIdInVisibleElements(const QVariant& uid, int& pos);

        void setElementNameByFile(const QString& filename);

    public:

        //Operations with elements without data
        virtual bool isValidElementsWithoutData();

        virtual bool isVisibleElement(const QVariant& value);

        QModelDescribing(QObject* parent = 0);

        QModelDescribing(const QModelDescribing& other);

        ~QModelDescribing() {};

        QVariantList getElementsWithoutData() const;

        QVariantList getVisibleElements() const;

        QVariantList getElementsWithData() const;

        void appendToList(const QString& filename);

        QVariant findByIdInVisibleElements(const QVariant& id);

        bool createTreeForViewing();

        bool checkElementWithDataOrNot() const
        {
            return isElementWithData;
        };

        //Operations with elements with data
        virtual bool isValidElementsWithDataForParticularFile() = 0;

        virtual void preparingDataStructureBeforeFilling() = 0;

        virtual bool isValidElementsWithData();

        void loadingDataElementsFromFile(const QString& filename);

        void clearElementsWithData();

        void clearVisibleElements();

        void clearElementsWithoutData();

        void clearAllElements();

    private:

        bool isElementWithData;
};

#endif // QMODELDESCRIBING_H
