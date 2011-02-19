#ifndef IVIEW_H
#define IVIEW_H
#include <QMainWindow>
class IView: public QMainWindow
{
    Q_OBJECT

public:
    virtual ~IView() {}

    IView (QWidget *parent) : QMainWindow(parent) {}

    QObject *model4, *modelP, *modelD;
    QObject **models;

    QObject **trees;

    QObject *corrModel;

    //QueryModel QModelDescring->StringList
    QObject* queryModel;
    QObject* mssqlQuery;


    virtual void removeWidgets() = 0;
    virtual void addWidgets() = 0;

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
};

#endif // IVIEW_H
