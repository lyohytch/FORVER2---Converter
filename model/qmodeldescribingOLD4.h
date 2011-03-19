#ifndef QMODELDESCRIBINGOLD4_H
#define QMODELDESCRIBINGOLD4_H
#include "qmodeldescribing.h"

//! QModelDescribingOld4 - класс для классификатор старой версии программы "Справка"
/**
  * Класс обрабатывает четыре файла описания: Sprav1.txt, Sprav2.txt, Sprav3.txt, Sprav4.txt - полученные сохранением
  * из XLS файлов классификаторов.Для каждого типа файлов существует свой файл с данными. Каждому файлу описания соответствует
  * свой ID в базе данных QModelDescribing::elementName. Файлы описания похожи на файл описания для QModelDescribingDemo
  * \mainpage QModelDescribingOld4
  */
class QModelDescribingOld4: public QModelDescribing
{
    public:

        //Operations with elements without data
        /**
          * Конструктор по умолчанию. Инициализирует QStandardItemModel
          * \param parent указатель на родительский объект QObject
          */
        QModelDescribingOld4(QObject* parent = 0);
        ~QModelDescribingOld4() {}

        //Operations with elements with data
        virtual void preparingDataStructureBeforeFilling();//iListDataTemp ->iListData
        virtual bool isValidElementsWithDataForParticularFile();

    protected:

        //Operations with elements without data
        virtual  bool checkFileStructure(QTextStream* fileStream);
        virtual QVariantMap fillOneElement(const QStringList& capturedText);
        //Operations with elements with data
        virtual void addingLoadedDataInVisibleElementsWithData(QTextStream* fileStream);
        virtual QVariantList processLineInDataFile(const QString& line, const QVariantList& DataStructure);
        virtual bool isValidStringInDataFileToAdd(const QVariantList& dataStructure);
        virtual void setElementNameByDataFile(const QString& filename);
        /**
          * Возвращает отступ, с которого будет осуществляться считывание данных для конкретного QModeldescribing::elementName
          * \param statName имя QModeldescribing::elementName
          * \return Отступ от начала строки
          */
        int setSeekofLine(const QString& statName);
        /**
          * Возвращает элемента по QModeldescribing::elementName
          * \param statName имя QModeldescribing::elementName
          * \param oneRecord заполняемый элемент с данными
          * \return элемент с данными, и точным id
          * \note Устанавливается точное значение id по QModeldescribing::elementName, используя сдвиг
          */
        QVariant getIdByElementName(const QString& statName, const  QVariantList& oneRecord);
        virtual bool checkFileFileStructureData(QTextStream* /*fileStream*/);
};

#endif // QMODELDESCRIBINGOLD4_H
