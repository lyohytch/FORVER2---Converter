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
          * Список, содержащий карты QModelDescribing::VisibleElementsFromDescriptionFiles
          * QList<  [[[  QList<QMap<QString, QVariant> > ]]]  >
          */
        QVariantList VisibleElementWithData;
        /**
          * Элементы, заполненные данными. (Для одного файла)
          * Список карт типа (id, value)
          * \note Временный список для последующего преобразования в QModelDescribing::VisibleElementWithData
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
          * Заполняет отдельный элемент из QModelDescribing::ElementsFromDescriptionFiles
          * \param capturedText данные, захваченные регулярным выражением
          * \return заполненый элемент
          */
        virtual QVariantMap fillOneElement(const QStringList & capturedText) = 0;
        /**
          * Формирует данные для QModelDescribing::ElementsFromDescriptionFiles
          * \param fileStream текстовый файл
          * \return список элементов QModelDescribing::ElementsFromDescriptionFiles
          */
        virtual QVariantList getElementsFromText(QTextStream* fileStream);
        /**
          * Устанавливает QModelDescribing::additionToNames по имени файла
          * \param filename имя файла
          */
        virtual void setAdditionsToNamesByFile(const QString & filename)
        {
            Q_UNUSED(filename);
        };
        /**
          * Заполняет весь QModelDescribing::ElementsFromDescriptionFiles из файла
          * \param fileStream текстовый файл
          */
        void addingDataToBlankElements(QTextStream* fileStream);
        /**
          * Формирует дерево из QModelDescribing::ElementsFromDescriptionFiles
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
          * Определяет можно ли добавить текущий элемент в QModelDescribing::ElementsFromDescriptionFiles
          * \param checkMap проверяемый элемент
          * \return 1 - подходящий элемент, 0 - иначе
          */
        virtual bool isValidStringInDescriptionFileToAdd(const QMap<QString, QVariant> &checkMap);
        /**
          * Участвует в формировании QModelDescribing::ElementsFromDescriptionFiles. К каждому элементу добавляется порядковый номер
          * \param oneRec
          * \sa addingDataToBlankElements ()
        */
        virtual void addNextElementsToList(const QVariantList & oneRec);
        /**
          * Считывает несколько символов из строки line до появления символа табуляции, начиная с символа k
          * \param line строка, из которой мы считываем данные
          * \param k стартовый индекс, с которого начинается считывание строки
          * \return Строка, начинающаяся с k-ого символа то символа табуляции
          * \deprecated Функция будет заменена использованием регулярного выражения
          */
        virtual QString readSymbolsFromString(const QString& line, int& k);
        /**
          * Сдвигается в строке line, начиная с k-ого символа, на cTurn символов табуляции, причём между ними может быть некоторый текст
          * \param line строка, по которой мы двигаемся
          * \param k стартовый индекс, начиная с которого, мы двигаемся
          * \param cTurn количество символов табуляции
          * \return 1 - удалось сдвинуться, 0 - иначе
          * \deprecated Функция будет заменена использованием регулярного выражения
          */
        virtual bool turn(const QString& line, int& k, int cTurn);
        /**
          * Создаёт список QModelDescribing::VisibleElementsFromDescriptionFiles из списка QModelDescribing::ElementsFromDescriptionFiles
          * \note Выбираются так называемые значимые элементы: поля ввода, альтернативные, дистрибутивные вопросы и списки
          */
        virtual void createListofVisibleElements();

        //Operations with elements with data
        /**
          * Проверяет корректная или нет структура файла с данными
          * \param fileStream поток текстовых данных
          * \return 1 - корректная, 0 - иначе
          */
        virtual bool checkFileFileStructureData(QTextStream* fileStream) = 0;
        /**
          * Обрабатывает одну строку из текстового файла и выводит заполненный данными список DataStructure
          * \param line строка, из которой производится считывание данных
          * \param DataStructure список, шаблон структуры, в которую будут записываться данные
          * \return заполненный данными список DataStructure
          */
        virtual QVariantList processLineInDataFile(const QString& line, const QVariantList &DataStructure) = 0;
        /**
          * Проверяет возможно ли добавление считанной информации из файла данных в список с данными
          * \param checkMap карта с данными, проверяемая на корректность
          * \return 1 - добавление возможно, 0 - иначе
          */
        virtual bool isValidStringInDataFileToAdd(const QVariantList &checkMap) = 0;
        /**
          * Отвечает за считывание данных из текствого файла. Для каждой структуры файла устанавливает свой алгоритм считывания
          * \param fileStream
          */
        virtual void addingLoadedDataInVisibleElementsWithData(QTextStream* fileStream) = 0;
        /**
          * Устанавливает ID элемента, который будет представлен в БД, по имени файла c данными
          * \param filename имя файла
          */
        virtual void setElementNameByDataFile(const QString &filename) = 0;
        /**
          * Инициализирует список-шаблон, в который будут записываться данные
          * \return Инициализированный список-шаблон
          * \note Для каждого файла описания, есть свой тип списка, есть свой тип данных.
          * Причём, если несколько файлов описания, то формируется один список, состоящий из разных частей. Необходимо выбрать из общего списка, только тот
          * что соответствует файлу с данными
          */
        virtual  QVariantList initDataStructure();
        /**
          * Существует ли в списке QModelDescribing::VisibleElementsWithDataForParticularFile элемент с указанным uid и вернут его pos в списке
          * \param uid  ИД элемента, который мы ищем
          * \param pos позиция найденного элемента
          * \return 1 - элемент найден, 0 - иначе
          */
        virtual bool findByUIdInVisibleElements(const QVariant& uid, int& pos);
        /**
          * Устанавливает ID элемента, который будет представлен в БД, по имени файла
          * \param filename имя файла
          */
        void setElementNameByFile(const QString& filename);

    public:

        //Operations with elements without data
        /**
          * Проверяет на пустоту QModelDescribing::VisibleElementsFromDescriptionFiles
          * \return 1 - список не пуст, 0 - иначе
          */
        virtual bool isValidElementsWithoutData();
        /**
          * Проверяет на пустоту QModelDescribing::ElementsFromDescriptionFiles
          * \return 1 - список не пуст, 0 - иначе
          */
        virtual bool isExistElementsWithoutData();
        /**
          * Проверяет на является ли value значимым
          * \param value проверяемый элемент
          * \return 1 - является, 0 - иначе
          * \sa createListofVisibleElements ()
          */
        virtual bool isVisibleElement(const QVariant& value);
        /**
          * Конструктор по умолчанию. Инициализирует QStandardItemModel
          * \param parent указатель на родительский объект QObject
          */
        QModelDescribing(QObject* parent = 0);
        /**
          * Конструктор копирования. Копирует списки ElementsFromDescriptionFiles
          */
        QModelDescribing(const QModelDescribing& other);
        /**
          * Деструктор.Ничего не удаляется, так как все поля в классе статические
          */
        virtual ~QModelDescribing() {}
        /**
          * Возвращает QModelDescribing::ElementsFromDescriptionFiles
          * \return Список QModelDescribing::ElementsFromDescriptionFiles
          */
        QVariantList getElementsWithoutData() const;
        /**
          * Возвращает QModelDescribing::VisibleElementsFromDescriptionFiles
          * \return Список QModelDescribing::VisibleElementsFromDescriptionFiles
          */
        QVariantList getVisibleElements() const;
        /**
          * Возвращает QModelDescribing::VisibleElementWithData
          * \return Список QModelDescribing::VisibleElementWithData
          */
        QVariantList getElementsWithData() const;
        /**
          * Запуск процесса построения QModelDescribing::ElementsFromDescriptionFiles по файлу описания
          * \param filename имя файла описания
          */
        void appendFromDataFilesToDataElements(const QString& filename);
        /**
          * Ищет по ID в списке QModelDescribing::VisibleElementsFromDescriptionFiles
          * \param id идентификатор,по которому осуществляется поиск
          * \return Найденный элемент в списке или NULL
          */
        QVariant findByIdInVisibleElements(const QVariant& id);
        /**
          * Запуск построения дерева по списку QModelDescribing::ElementsFromDescriptionFiles
          * \sa createTreeFromElements ()
          */
        bool createTreeForViewing();
        /**
          * Выводит состояние объекта: содержит ли он данные или нет
          * \return 1 - элемент с данными, 0 иначе
          * \deprecated Функция не используется
          */
        bool checkElementWithDataOrNot() const;

        //Operations with elements with data
        /**
          * Проверяет на пустоту QModelDescribing::VisibleElementsWithDataForParticularFile
          * \return 1 - список не пуст, 0 - иначе
          * \sa isValidElementsWithData ()
          */
        virtual bool isValidElementsWithDataForParticularFile() = 0;
        /**
          * Перегруппировка данных из QModelDescribing::VisibleElementsWithDataForParticularFile перед
          * окончательным добавлением в список VisibleElementWithData
          */
        virtual void preparingDataStructureBeforeFilling() = 0;
        /**
          * Проверяет на пустоту QModelDescribing::VisibleElementWithData
          * \return 1 - список не пуст, 0 - иначе
          * \sa isValidElementsWithDataForParticularFile ()
          */
        virtual bool isValidElementsWithData();
        /**
          * Запускает процесс считывания данных(заполнение QModelDescribing::VisibleElementWithData) из файла с данными
          * \param filename имя файла с данными
          */
        void loadingDataElementsFromFile(const QString& filename);
        /**
          * Очищает список QModelDescribing::VisibleElementWithData
          * \sa clearVisibleElements () , clearElementsWithoutData () , clearAllElements ()
          */
        void clearElementsWithData();
        /**
          * Очищает список QModelDescribing::VisibleElementsFromDescriptionFiles
          * \sa clearElementsWithData () , clearElementsWithoutData () , clearAllElements ()
          */
        void clearVisibleElements();
        /**
          * Очищает список QModelDescribing::ElementsFromDescriptionFiles
          * \sa clearElementsWithData () , clearVisibleElements () , clearAllElements ()
          */
        void clearElementsWithoutData();
        /**
          * Очищает списки QModelDescribing::VisibleElementWithData, QModelDescribing::VisibleElementsFromDescriptionFiles, QModelDescribing::ElementsFromDescriptionFiles
          * \sa clearElementsWithData () , clearVisibleElements () , clearElementsWithoutData ()
          */
        void clearAllElements();

    private:

        bool isElementWithData;
};

#endif // QMODELDESCRIBING_H
