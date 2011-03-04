#ifndef QMODELDESCRIBING_H
#define QMODELDESCRIBING_H

#include <QVariantList>
#include <QString>
#include <QTextStream>
#include <QStandardItemModel>
#include <QFile>

#include "constants.h"

//! QModelDescribing - базовый класс для файлов описания классификатора
/**
  * Класс предоставляет функции для работы с файлами описания классификаторов.
  * Формирует структуры из файлов описаний для последующего построения корреляционной таблицы
  */

class QModelDescribing : public QStandardItemModel
{
        Q_OBJECT

    protected:
        /**
          * Начало ID элемента в БД. Сейчас актуальны: "generic_","figurant_","locus_delicti_","weapon_"
          */
        QString elementName;
        /**
          * Дополнительные добавки к именам. Используются при отображении на форме
          */
        QString additionToNames;
        /**
          * Все считанные элементы из файла описания
          * Список карт с ключами (id, name, type, level, dvalue).
          */
        QVariantList ElementsFromDescriptionFiles;
        /**
          * Все элементы, которые будут отображаться на форме и несут некоторое значение, из файла описания
          * Список карт с ключами (id, name, type, level, value).
          */
        QVariantList VisibleElementsFromDescriptionFiles;
        /**
          * Элементы, заполненные данными.
          * Список, содержащий карты VisibleElementsFromDescriptionFiles
          * QList<  [[[  QList<QMap<QString, QVariant> > ]]]  >
          */
        QVariantList VisibleElementWithData;
        /**
          * Элементы, заполненные данными. (Для одного файла)
          * Список карт типа (id, value)
          * \note Временный список для последующего преобразования в VisibleElementWithData
          */
        QVariantList VisibleElementsWithDataForParticularFile;

        //Operations with elements without data
        /**
          * Проверяет корректная или нет структура файла
          * \param fileStream поток текстовых данных
          * \return 1 - корректная, 0 - иначе
          */
        virtual bool checkFileStructure(QTextStream *fileStream) = 0;
        /**
          * Заполняет отдельный элемент из ElementsFromDescriptionFiles
          * \param capturedText данные, захваченные регулярным выражением
          * \return заполненый элемент
          */
        virtual QVariantMap fillOneElement(const QStringList & capturedText) = 0;
        /**
          * Формирует данные для ElementsFromDescriptionFiles
          * \param fileStream текстовый файл
          * \return список элементов ElementsFromDescriptionFiles
          */
        virtual QVariantList getElementsFromText(QTextStream* fileStream);
        /**
          * Устанавливает additionToNames по имени файла
          * \param filename имя файлв
          */
        virtual void setAdditionsToNamesByFile(const QString & filename)
        {
            Q_UNUSED(filename);
        };
        /**
          * Заполняет весь ElementsFromDescriptionFiles из файла
          * \param fileStream текстовый файл
          */
        void addingDataToBlankElements(QTextStream* fileStream);
        /**
          * Формирует дерево из ElementsFromDescriptionFiles
          * \param iList один из элементов ElementsFromDescriptionFiles
          * \param i количество пройденных элементов в списке
          * \param levels уровень предыдущего элемента
          * \param parent указатель на родительский элемент QStandardItem
          * \note Функция рекурсивная
          */
        void createTreeFromElements(const QVariantList& iList, int i, int levels, QStandardItem* parent);
        /**
          * Устанавливает кодек по содержимому файла
          * \param filesource указатель на QFile
          * \note Читает весь файл. Нужно переоткрыть файл для дальнейшей работы
          */
        QTextCodec* setFileEncodingByContain(QFile *filesource);
        /**
          * Определяет можно ли добавить текущий элемент в ElementsFromDescriptionFiles
          * \param checkMap проверяемый элемент
          * \return 1 - подходящий элемент, 0 - иначе
          */
        virtual bool isValidStringInDescriptionFileToAdd(const QMap<QString, QVariant> &checkMap);

        virtual void addNextElementsToList(const QVariantList & oneRec);

        virtual QString readSymbolsFromString(const QString& line, int& k);

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
        virtual bool isExistElementsWithoutData();

        virtual bool isVisibleElement(const QVariant& value);

        QModelDescribing(QObject* parent = 0);

        QModelDescribing(const QModelDescribing& other);

        virtual ~QModelDescribing() {}

        QVariantList getElementsWithoutData() const;

        QVariantList getVisibleElements() const;

        QVariantList getElementsWithData() const;

        void appendFromDataFilesToDataElements(const QString& filename);

        QVariant findByIdInVisibleElements(const QVariant& id);

        bool createTreeForViewing();

        bool checkElementWithDataOrNot() const;

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
