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
    private:
    protected:
        QString statName;//"generic_","figurant_","locus_delicti_","weapon_"
        //GENERAL - DESCRIBING MODEL
        void addingDataToList(QTextStream* fileStream);
        //TODO move to protect
        /**
         *Список из карт типа (id,name,type,level,value) value == NULL for Description Files
         */
        QVariantList iListDescribing;// General describing list
        QVariantList iListSignificant;
        QVariantList iListData;// QList<  [[[  QList<QMap<QString, QVariant> > ]]]  >
        /*child*/
        virtual bool checkFileStructure(QTextStream* /*fileStream*/)
        {
            return true;
        };
        //Create tree
        virtual void setChildItem(const QVariantList& iList, int i, int levels, QStandardItem* parent);
        /*child*/
        //Accept or not string from describing file to describing model
        virtual bool isValidString(const QMap<QString, QVariant> &checkMap);

        /*child*/
        //Process line in describing file
        virtual QMap<QString, QVariant>* process_line(const QString& /*line*/)
        {
            return NULL;
        };
        virtual void moveOneRecordToList(QMap<QString, QVariant> & oneRec);
        // Read one element from describing file
        virtual QString readElement(const QString& line, int& k);
        // Turn on k symbols
        virtual bool turn(const QString& line, int& k, int cTurn);
        // Fill significant list in describing model(to be added in DB)
        virtual void fillSignificantList();

        //DATA
        /*child*/
        virtual bool checkFileFileStructureData(QTextStream* /*fileStream*/)
        {
            return true;
        };
        /*child*/
        // Process line from data file
        virtual QVariantList process_lineData(const QString& /*line*/, const QVariantList &/*DataStructure*/)
        {
            return QVariantList();
        };
        /*child*/
        virtual bool isValidStringData(const QVariantList &/*checkMap*/)
        {
            return true;
        };
        /*child*/
        // Adding data in model
        virtual void addingLoadedData(QTextStream* /*fileStream*/)
        {
        };
        virtual bool foundByUId(const QVariant& uid, int& pos);
        void setStatNameByFile(const QString& filename);
        virtual void setStatNameByFileData(const QString &/*filename*/) {};
        QVariantList iListDataTemp;//Массив мапов типа (ид, значение), где значение - данные из конкретного файла
        virtual  QVariantList initDataStructure();
    public:
        /**
         *
         */
        //GENERAL
        QModelDescribing(QObject* parent = 0);
        QModelDescribing(const QModelDescribing& other);
        ~QModelDescribing() {};
        QVariantList getListDescribing() const;
        QVariantList getListSignificant() const;
        QVariantList getListData() const;
        void appendToList(const QString& filename);
        bool modelData() const
        {
            return isData;
        };
        bool createModel();
        virtual bool isValid();
        virtual bool isSignificant(const QVariant& value);
        QVariant findById(const QVariant& id);//Найти элемент по ид в iListSignificant
        //DATA
        /*child*/
        virtual bool isValidDataTemp()
        {
            qDebug();
            return true;
        };
        /*child*/
        virtual void dataPrepare()
        {
        };
        virtual bool isValidData();//isDataLoaded?
        void loadingData(const QString& filename);
        void resetDataList();
        void resetSignList();
        void resetDescList();
        void resetAllList();


    private:
        bool isData;
};

//Класс для старого формата данных, где есть 4 файла: главный, фигурант, оружие, место
class QModelDescribingOld4: public QModelDescribing
{
    public:
        //GENERAL
        QModelDescribingOld4(QObject* parent = 0);
        ~QModelDescribingOld4() {};
        //DATA
        virtual void dataPrepare();//iListDataTemp ->iListData
        //virtual bool isValid(){qDebug()<<Q_FUNC_INFO<<"Not implemented yet";return true;};
        virtual bool isValidDataTemp();
    protected:
        //GENERAL
        //TODO implement checkFileStructure
        virtual  bool checkFileStructure(QTextStream* fileStream);
        //TODO implement isValid()
        virtual QMap<QString, QVariant>* process_line(const QString& line);
        //DATA
        virtual void addingLoadedData(QTextStream* fileStream);
        virtual QVariantList process_lineData(const QString& line, const QVariantList& DataStructure);
        virtual bool isValidStringData(const QVariantList& dataStructure);
        virtual void setStatNameByFileData(const QString& filename);
        int setSeekofLine(const QString& statName);
        QVariant getIdByStatName(const QString& statName, const  QVariantList& oneRecord);
};




//Класс для демоверсии: вроде бы один файл для считывания
class QModelDescribingDemo: public QModelDescribing
{
    public:
        QModelDescribingDemo(QObject* parent = 0);
        ~QModelDescribingDemo() {};
        virtual bool isValidDataTemp();

    protected:
        //GENERAL
        virtual QMap<QString, QVariant>* process_line(const QString& line);
        virtual  bool checkFileStructure(QTextStream* fileStream);
        virtual bool isValidString(const QMap<QString, QVariant> &checkMap);
        //DATA
        virtual void addingLoadedData(QTextStream* fileStream);
        virtual QVariantList process_lineData(const QString& line, const QVariantList& DataStructure);
        virtual bool isValidStringData(const QVariantList& dataStructure);
        virtual QVariantList initDataStructure();
};





/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Класс для формата из прокуратуры, представляет собой 4 файла(формы), в которых данные записываются,
// начиная с пятой(необязательно) строки. Связующее звено - элемент Р(вероятно, некоторый id)
class QModelDescribingPros: public QModelDescribing
{
    public:
        QModelDescribingPros(QObject* parent = 0);
        ~QModelDescribingPros() {};
        virtual bool isValidDataTemp();
    protected:
        //GENERAL
        virtual QMap<QString, QVariant>* process_line(const QString& line);
        virtual bool checkFileStructure(QTextStream* fileStream);
        virtual void moveOneRecordToList(QMap<QString, QVariant> & oneRec);
        //DATA
        virtual void addingLoadedData(QTextStream* fileStream);
        virtual QVariantList process_lineData(const QString& line, const QVariantList& DataStructure);
        virtual bool isValidStringData(const QVariantList& dataStructure);
        virtual void setStatNameByFileData(const QString& filename);
        QVariant getIdByStatName(const QString& statName, const  QVariantList& oneRecord);
        virtual void dataPrepare();//iListDataTemp=>iListData
    private:
        bool isProcessLine;
        QString removeSpaces(const QString& ex);
};

#endif // QMODELDESCRIBING_H
