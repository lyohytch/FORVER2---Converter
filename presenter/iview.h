#ifndef IVIEW_H
#define IVIEW_H
#include <QMainWindow>
//! IView интерфейс для GUI
/**
  * Класс является интерфейсом для построения GUI. Здесь объявлены все сигналы от GUI и виртуальные функции,
  * которые должны быть имплементированы в классе GUI. Подписывание на сигналы происходит в Presenter , GUI только
  * посылает нужные сигналы и не нагружен лишней функциональностью
  * \sa Presenter
  */
class IView: public QMainWindow
{
        Q_OBJECT

    public:
        virtual ~IView() {}
        /**
          * Конструктор по-умолчанию
          * \param parent указатель на родительский виджет. Необходим для инициализации QMainWindow
          */
        IView(QWidget* parent) : QMainWindow(parent) {}
        /**
          * Указатель на объект QModelDescribingOld4
          */
        QObject* model4;
        /**
          * Указатель на объект QModelDescribingPros
          */
        QObject* modelP;
        /**
          * Указатель на объект QModelDescribingDemo
          */
        QObject* modelD;
        /**
          * Указатель на объект QModelDescribingFromJura
          */
        QObject* modelJURA;
        /**
          * Массив объектов QModelDescribing
          */
        QObject** models;
        /**
          * Массив объектов TreeViewModel
          */
        QObject** trees;
        /**
          * Указатель на объект CorrelationModel
          */
        QObject* corrModel;

        /**
          * Указатель на объект querymodel
          */
        QObject* queryModel;
        /**
          * Указатель на объект mssqlquery
          */
        QObject* mssqlQuery;
        /**
          * Удалить какие-нибудь виджеты
          */
        virtual void removeWidgets() = 0;
        /**
          * Добавить какие-нибудь виджеты
          */
        virtual void addWidgets() = 0;
        /**
          * Обновить сообщение на форме
          * \param txtMsg текстовое сообщение
          */
        virtual void updateTextLabel(const QString& txtMsg) = 0;
        /**
          * Установить или нет английский язык
          * \param check true - установить, false - иначе
          */
        virtual void englishLanguageSet(bool check) = 0;
        /**
          * Установить или нет русский язык
          * \param check true - установить, false - иначе
          */
        virtual void russianLanguageSet(bool check) = 0;
        /**
          * Выбранная строка в корреляционной таблице
          */
        int activeRowId;
        /**
          * True - значение для источника в корреляционной таблице выставляется в первый раз, 0 - иначе
          * \note В случае с false предыдущее значение не стирается, к нему добавляется новое
          */
        bool countTV;

    signals:
        /**
          * Сигнал посылается в случае открытия пользователем файлов источника
          */
        void OnOpenTargetFiles();
        /**
          * Сигнал посылается в случае открытия пользователем файлов приёмника
          */
        void OnOpenTemplateFiles();
        /**
          * Сигнал посылается в случае попытки загрузить пользователем файлов данных источника
          */
        void OnLoadTargetData();
        /**
          * Сигнал посылается в случае попытки загрузить пользователем файлов данных приёмника
          */
        void OnLoadTemplateData();
        /**
          * Сигнал посылается Presenters для создания необходимых для работы объектов
          * \sa Presenters
          */
        void CreateObjects();
        /**
          * Сигнал посылается Presenters для удаления объектов
          * \sa Presenters
          */
        void FreeObjects();
        /**
          * Сигнал посылается в случае попытки загрузить пользователем данных приёмника в базу данных
          */
        void OnExportTemplateDataToDB();
        /**
          * Сигнал посылается в случае попытки загрузить пользователем данных источника в базу данных
          */
        void OnExportTargetDataToDB();
        /**
          * Сигнал посылается при двойном щелчке пользователя на дерево источника
          */
        void OnElementTreeTargetActivated();
        /**
          * Сигнал посылается при двойном щелчке пользователя на дерево приёмника
          */
        void OnElementTreeTemplateActivated();
        /**
          * Сигнал посылается в случае попытки сохранить пользователем корреляционную таблицу в файл
          */
        void OnSaveCorrelationTable();
        /**
          * Сигнал посылается в случае попытки загрузить пользователем корреляционную таблицу из файла
          */
        void OnLoadCorrelationTable();

        //Translations
        /**
          * Сигнал посылается при смене языка в меню на английский
          */
        void OnEnglishChecked(bool);
        /**
          * Сигнал посылается при смене языка в меню на русский
          */
        void OnRussianChecked(bool);
};

#endif // IVIEW_H
