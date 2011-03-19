#ifndef PRESENTER_H
#define PRESENTER_H
#include <QModelIndex>
#include <QTranslator>
#include <QApplication>
#include <QSettings>

#include "iview.h"

//! Presenters управление действиями пользователя, прослойка между моделью и GUI
/**
  * Класс является прослойкой между моделью и GUI. В классе присутствуют функции, отвечающие за обработку
  * действий пользователя и представление данных на GUI.
  */
class Presenters: public QObject
{
        Q_OBJECT

    public:
        /**
          * Выполняет connect всем сигналам из IView и при необходимости загружает файлы перевода
          * \param view указатель на интерфейс IView
          * \param app указатель на QApplication, для которого загружаем файлы перевода
          */
        Presenters(IView* view, QApplication* app);
    protected:
        /**
          * Указатель на интерфейс IView
          */
        IView* _view;
        /**
          * Указатель на объект QApplication
          */
        QApplication* mainApplication;
        /**
          * Используется для перевода проекта UI
          */
        QTranslator translateUi;
        /**
          * Используется для перевода проекта MODEL
          */
        QTranslator translateModel;
        /**
          * Используется для перевода проекта PRESENTER
          */
        QTranslator translatePresenter;
        /**
          * Загружает данные для источника и приёмника, используя файлы описания по умолчанию
          * \param loadedModel указатель на QModelDescribing
          * \param tree указатель на TreeViewModel
          * \sa getDefaultFilesForModel ()
          */
        void loadingModels(QObject* loadedModel, QObject* tree);
        /**
          * Получить список файлов описания по-умолчанию, подходящих конкретной loadedModel
          * \param loadedModel указатель на QModelDescribing
          * \return Список файлов для данной модели
          * \sa loadingModels ()
          */
        QStringList getDefaultFilesForModel(QObject* loadedModel);
        /**
          * Проверяет можно или нет использовать загруженные файлы описания.
          * \param filenames список файлов
          * \param description_id 0 - ид приёмника, 1 - ид источника
          * \return True - можно, false - иначе
          * \note У каждой модели существует фильтр, по которому мы определяем принадлежность файлов к модели
          */
        bool selectDescription(const QStringList& filenames, int description_id);
        /**
          * Обновилась или нет модель источника или приёмника (0 - приёмник, 1 - источник)
          * \param descriptionId 0 - ид приёмника, 1 - ид источника
          * \param model указатель на объект QModelDescribing
          * \return True - обновилась, false - иначе
          */
        bool refreshDescribingLists(int descriptionId, QObject* model);
        /**
          * Инициализировать объекты: выделить память, расставить на форме виджеты
          *
          */
        void initializeObjects();
        /**
          * Установить слоты для двойных кликов на виджетах деревьев источника и приёмника
          */
        void connectActionsToObjects();
        /**
          * Выделить память для кореляционной таблицы
          */
        virtual void allocateCorrelationModel() = 0;
        /**
          * Выделить память для всех моделей
          */
        virtual void allocateMemory();
        /**
          * Установить какие модели будут приёмниками и источниками
          */
        virtual void setModelsAndTreesObjects();
        /**
          * Загрузить файлы описания для приёмника или источника
          * \param filenames список загруженных файлов
          * \param  descriptionId 0 - ид приёмника, 1 - ид источника
          */
        void loadDescriptionModelFromFiles(const QStringList& filenames, int descriptionId);
        /**
          * Загрузить данные из файлов для приёмника или источника
          * \param filenames список загруженных файлов
          * \param  descriptionId 0 - ид приёмника, 1 - ид источника
          */
        void loadDataFromFilesWithData(const QStringList& filenames, int descriptionId);
        /**
          * Установить список фильтров для файлов источника
          * \return Список фильтров для файлов источника
          */
        virtual QStringList setNameFilterForTargetFiles();
        /**
          * Установить список фильтров для файлов приёмника
          * \return Список фильтров для файлов приёмника
          */
        virtual QStringList setNameFilterForTemplateFiles();
        /**
          * Установить список фильтров для файлов данных приёмника
          * \return Список фильтров для файлов данных приёмника
          */
        virtual QStringList setNameFilterForDataTemplateFiles();
        /**
          * Установить список фильтров для файлов данных источника
          * \return Список фильтров для файлов данных источника
          */
        virtual QStringList setNameFilterForDataTargetFiles();
        /**
          * Открыть файлы описания по списку фильтров
          * \return Список открытых файлов описания
          */
        QStringList openFilesByAnyNameFilter(const QStringList& nameFilter);
        /**
          * Открыть файлы данных по списку фильтров
          * \return Список открытых файлов данных
          */
        QStringList openDataFilesByAnyNameFilter(const QStringList& nameFilter);
        /**
          * Установить направление перевода по-умолчанию: English
          */
        void defaultTranslateSets();
        /**
          * Загрузить файлы с переводом в объекты QTranslator
          */
        void loadTranslationFiles();
    protected slots:
        /**
          * Слот для обработки сигнала IView::OnOpenTargetFiles
          */
        void onOpenTargetFiles();
        /**
          * Слот для обработки сигнала IView::OnOpenTemplateFiles
          */
        void onOpenTemplateFiles();
        /**
          * Слот для обработки сигнала IView::CreateObjects
          */
        virtual void createObjects();
        /**
          * Слот для обработки сигнала IView::FreeObjects
          */
        virtual void freeObjects();
        /**
          * Слот для обработки сигнала IView::OnLoadTemplateData
          */
        void onLoadTemplateData();
        /**
          * Слот для обработки сигнала IView::OnLoadTargetData
          */
        void onLoadTargetData();
        /**
          * Слот для обработки двойного клика на виджете дереве источника
          * \param index активированный элемент в дереве источника
          */
        virtual void ElementTreeTemplateActivated(const QModelIndex& index);
        /**
          * Слот для обработки двойного клика на виджете дереве приёмника
          * \param index активированный элемент в дереве приёмника
          */
        virtual void ElementTreeTargetActivated(const QModelIndex& index);
        /**
          * Слот для обработки двойного клика на виджете корреляционной таблицы
          * \param index активированный элемент в таблице
          */
        virtual void ElementTableActivated(const QModelIndex& index);
        /**
          * Заполнить данными корреляционную таблицу
          */
        void FillTable();
        /**
          * Слот для обработки сигнала IView::OnExportTemplateDataToDB
          */
        void onExportTemplateDataToDB();
        /**
          * Слот для обработки сигнала IView::OnExportTargetDataToDB
          */
        void onExportTargetDataToDB();
        /**
          * Слот запускается после построения querymodel, выделляет память для mssqlquery и запускает запись в базу данных
          */
        void makeRequestSlot();
        /**
          * Слот для обработки сигнала IView::OnSaveCorrelationTable
          */
        void onSaveCorrelationTable();
        /**
          * Слот для обработки сигнала IView::OnLoadCorrelationTable
          */
        void onLoadCorrelationTable();
        /**
          * В слот передаётся информация о результате добавления в базу данных
          * \param err номер ошибки
          * \param msg текст ошибки
          */
        void completeAddingToDB(int err, QString msg);
        /**
          * Слот для обработки сигнала IView::OnEnglishChecked
          * \param checked True - нажат, false - иначе
          */
        void onEnglishChecked(bool checked);
        /**
          * Слот для обработки сигнала IView::OnRussianChecked
          * \param checked True - нажат, false - иначе
          */
        void onRussianChecked(bool checked);

    signals:
        /**
          * Сигнал посылается в том случае, если описание модели загружено
          */
        void loadDescComplete();
    private:
        void setTranslationSettings(QSettings& trSettings, bool english, bool russian);
};



#endif // PRESENTER_H
