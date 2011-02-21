#ifndef PRESENTER_H
#define PRESENTER_H
#include <QModelIndex>

#include "iview.h"

class Presenters: public QObject
{
    Q_OBJECT

public:
    Presenters(IView *view);

protected:
    IView *_view;
    void loadingModels(QObject* loadedModel, QObject* tree);
    bool selectDescription(const QStringList& filenames, int description_id);
    bool refreshDescribingLists(int descriptionId, QObject * model);
    void allocateMemoryForObjects();
    virtual void allocateCorrelationModel() = 0;
    void loadDescriptionModelFromFiles(const QStringList &filenames, int descriptionId);
    void loadDataFromFilesWithData(const QStringList &filenames, int descriptionId);
    virtual QString setNameFilterForTargetFiles();
    virtual QString setNameFilterForTemplateFiles();
    virtual QString setNameFilterForDataTemplateFiles();
    virtual QString setNameFilterForDataTargetFiles();
    QStringList openFilesByAnyNameFilter(const QString &nameFilter);
    QStringList openDataFilesByAnyNameFilter(const QString &nameFilter);
protected slots:
    void onOpenTargetFiles();
    void onOpenTemplateFiles();

    virtual void createObjects();
    virtual void freeObjects();

    void onLoadTemplateData();
    void onLoadTargetData();
    void ElementTreeTemplateActivated(const QModelIndex&);
    void ElementTreeTargetActivated(const QModelIndex&);
    virtual void ElementTableActivated(const QModelIndex &);
    void FillTable();
    void onExportTemplateDataToDB();
    void onExportTargetDataToDB();
    void makeRequestSlot();

    void onSaveCorrelationTable();
    void onLoadCorrelationTable();

    void completeAddingToDB(int , QString );

signals:
    void loadDescComplete();
    void CreateObjects();
};



#endif // PRESENTER_H
