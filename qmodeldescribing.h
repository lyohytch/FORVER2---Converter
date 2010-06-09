#ifndef QMODELDESCRIBING_H
#define QMODELDESCRIBING_H

#include <QVariantList>
#include <QString>
#include <QTextStream>
#include <QStandardItemModel>
#include <QDebug>


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
    //GENERAL
   /*child*/ virtual void addingDataToList(QTextStream *fileStream) {qDebug()<<Q_FUNC_INFO;Q_UNUSED(fileStream);};
    //TODO move to protect
    /**
     *������ �� ���� ���� (id,name,type,level,value) value == NULL for Description Files
     */
    QVariantList iListDescribing;
    QVariantList iListSignificant;
    QVariantList iListData;// QList<  [[[  QList<QMap<QString, QVariant> > ]]]  >
    /*child*/virtual bool checkFileStructure(QTextStream *fileStream) {qDebug()<<Q_FUNC_INFO;Q_UNUSED(fileStream); return true;};
    virtual void setChildItem(const QVariantList &iList,int i,int levels,QStandardItem *parent);
    /*child*/virtual bool isValidString(const QMap<QString,QVariant> &checkMap);
    /*child*/virtual QMap<QString, QVariant>* process_line(const QString &line){qDebug()<<Q_FUNC_INFO;Q_UNUSED(line);return NULL;};
    //TODO implement process_line.<Methods can be overloaded in children objects>
    virtual QString readElement(const QString &line,int &k);
    virtual bool turn(const QString &line, int &k, int cTurn);
    virtual void fillSignificantList();
    //DATA
    /*child*/virtual bool checkFileFileStructureData(QTextStream *fileStream) {qDebug()<<Q_FUNC_INFO;Q_UNUSED(fileStream); return true;};
    /*child*/virtual QVariantList process_lineData(const QString &line, const QVariantList &/*DataStructure*/){qDebug()<<Q_FUNC_INFO;Q_UNUSED(line);return QVariantList();};
    /*child*/virtual bool isValidStringData(const QVariantList &/*checkMap*/){qDebug()<<Q_FUNC_INFO; return true;};
    /*child*/ virtual void addingLoadedData(QTextStream *fileStream) {qDebug()<<Q_FUNC_INFO;Q_UNUSED(fileStream);};
    void setStatNameByFile(const QString &filename);
    virtual void setStatNameByFileData(const QString &/*filename*/) {};
public:
    /**
     *
     */
    //GENERAL
    QModelDescribing(QObject *parent = 0);
    QModelDescribing(const QModelDescribing &other);
    ~QModelDescribing() {};
    QVariantList getListDescribing() const;
    QVariantList getListSignificant() const;
    QVariantList getListData() const;
    void appendToList(const QString &filename);
    bool modelData() const{return isData;};
    bool createModel();
    virtual bool isValid();
    virtual bool isSignificant(const QVariant &value);
    QVariant findById(const QVariant &id);//����� ������� �� �� � iListSignificant
    //DATA
    virtual bool isValidData();//isDataLoaded?
    void loadingData(const QString &filename);
    void resetDataList();
    void resetSignList();
    void resetDescList();
    void resetAllList();


private:
    bool isData;
};

//����� ��� ������� ������� ������, ��� ���� 4 �����: �������, ��������, ������, �����
class QModelDescribingOld4: public QModelDescribing
{
public:
    //GENERAL
    QModelDescribingOld4(QObject *parent = 0);
    ~QModelDescribingOld4() {};
    //DATA
    void dataPrepare();//iListDataTemp ->iListData
    //virtual bool isValid(){qDebug()<<Q_FUNC_INFO<<"Not implemented yet";return true;};
    bool isValidDataTemp();
protected:
    //GENERAL
    QVariantList iListDataTemp;//������ ����� ���� (��, ��������), ��� �������� - ������ �� ����������� �����
   virtual  void addingDataToList(QTextStream *fileStream);
    //TODO implement checkFileStructure
   virtual  bool checkFileStructure(QTextStream *fileStream);
    //TODO implement isValid()
   virtual QMap<QString, QVariant>* process_line(const QString &line);
   //DATA
   bool foundByUId(const QVariant &uid, int &pos);
   virtual void addingLoadedData(QTextStream *fileStream);
   virtual QVariantList process_lineData(const QString &line, const QVariantList &DataStructure);
   virtual bool isValidStringData(const QVariantList & dataStructure);
   virtual void setStatNameByFileData(const QString &filename);
   int setSeekofLine(const QString & statName);
   QVariant getIdByStatName(const QString &statName,const  QVariantList &oneRecord);

private:
   QVariantList initDataStructure();
};




//����� ��� ����������: ����� �� ���� ���� ��� ����������
class QModelDescribingDemo: public QModelDescribing
{
public:
    QModelDescribingDemo(QObject *parent = 0);
    ~QModelDescribingDemo() {};
    //virtual bool isValid(){qDebug()<<Q_FUNC_INFO<<"Not implemented yet";return true;};

protected:
    //GENERAL
    virtual void addingDataToList(QTextStream *fileStream);
    virtual QMap<QString, QVariant>* process_line(const QString &line);
    virtual  bool checkFileStructure(QTextStream *fileStream);
    virtual bool isValidString(const QMap<QString,QVariant> &checkMap);
    //DATA
    virtual void addingLoadedData(QTextStream *fileStream);
    virtual QVariantList process_lineData(const QString &line, const QVariantList &DataStructure);
    virtual bool isValidStringData(const QVariantList & dataStructure);
private:
    QVariantList initDataStructure();
};





/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����� ��� ������� �� �����������: ���� ������ ����������
class QModelDescribingPros: public QModelDescribing
{
public:
    QModelDescribingPros(QObject *parent = 0);
    ~QModelDescribingPros() {};
protected:
    void addingDataToList(QTextStream *fileStream);
};

#endif // QMODELDESCRIBING_H
