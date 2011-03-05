#ifndef QMODELDESCRIBINGPROS_H
#define QMODELDESCRIBINGPROS_H
#include "qmodeldescribing.h"
//! QModelDescribingPros - класс для классификаторов, составленных в прокуратуре
/**
  * Класс предоставляет функции для работы с файлами описания, составленными в прокуратуре. Файлы описания существенно
  * отличаются от файлов описания для QModelDescribingOld4 и QModelDescribingDemo. Файлы описания выделяются тем,что классификатор не
  * имеет древовидной структуры (все элементы только первого уровня). В настоящий момент этот класс не используется как приёмником, т.е.
  * создавать структуры, в которые будут конвертироваться данные из других классификаторов, а является только источником данных.
  */

class QModelDescribingPros: public QModelDescribing
{
    public:
        /**
          * Конструктор по умолчанию. Инициализирует QStandardItemModel
          * \param parent указатель на родительский объект QObject
          */
        QModelDescribingPros(QObject* parent = 0);
        ~QModelDescribingPros() {}
        virtual bool isValidElementsWithDataForParticularFile();
    protected:
        //GENERAL
        //virtual QMap<QString, QVariant>* processLineInDescriptionFile(const QString& line);
        virtual bool checkFileStructure(QTextStream* fileStream);
        virtual void setAdditionsToNamesByFile(const QString &filename);
        //virtual void addNextElementsToList(QMap<QString, QVariant> & oneRec);
        virtual QVariantList getElementsFromText(QTextStream* /*fileStream*/);
        virtual QVariantMap fillOneElement(const QStringList & /*capturedText*/) {return QVariantMap();}
        //DATA
        virtual void addingLoadedDataInVisibleElementsWithData(QTextStream* fileStream);
        virtual QVariantList processLineInDataFile(const QString& line, const QVariantList& DataStructure);
        virtual bool isValidStringInDataFileToAdd(const QVariantList& dataStructure);
        virtual void setElementNameByDataFile(const QString& filename);
        /**
          * \see QModelDescribingOld4::getIdByStatName()
          */
        QVariant getIdByStatName(const QString& statName, const  QVariantList& oneRecord);
        virtual void preparingDataStructureBeforeFilling();//iListDataTemp=>iListData
        virtual bool checkFileFileStructureData(QTextStream* /*fileStream*/);
    private:
        bool isProcessLine;
        QString removeSpaces(const QString& ex);
        QString setNameWithoutRepeat(const QString &namekey, const QVariantList &elements);
};
#endif // QMODELDESCRIBINGPROS_H
