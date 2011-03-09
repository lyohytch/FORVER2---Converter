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
    IView (QWidget *parent) : QMainWindow(parent) {}
    /**
      * Указатели на объекты QModelDescribing
      */
    QObject *model4, *modelP, *modelD, *modelJURA;
    /**
      * Массив объектов QModelDescribing
      */
    QObject **models;
    /**
      * Массив объектов TreeViewModel
      */
    QObject **trees;
    /**
      * Указатель на объект CorrelationModel
      */
    QObject *corrModel;

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
    virtual void russianLanguageSet(bool check) = 0;

    int activeRowId;
    bool countTV;

signals:
     void OnOpenTargetFiles();
     void OnOpenTemplateFiles();

     void OnLoadTargetData();
     void OnLoadTemplateData();

     void CreateObjects();
     void FreeObjects();

     void OnExportTemplateDataToDB();
     void OnExportTargetDataToDB();

     void refreshDescribingLists(int descriptionId, QObject * model);

     void OnElementTreeTargetActivated();
     void OnElementTreeTemplateActivated();

     void OnSaveCorrelationTable();
     void OnLoadCorrelationTable();

     //Translations
     void OnEnglishChecked(bool);
     void OnRussianChecked(bool);
};

#endif // IVIEW_H
