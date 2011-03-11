#ifndef PRESENTER_H
#define PRESENTER_H
#include <QModelIndex>
#include <QSettings>
#include <QTranslator>
#include <QApplication>

#include "iview.h"


class Presenters: public QObject
{
    Q_OBJECT

public:
    Presenters(IView *view, QApplication *app);

protected:
    IView *_view;
    QApplication *mainApplication;
    QTranslator translateUi;
    QTranslator translateModel;
    QTranslator translatePresenter;
    void loadingModels(QObject* loadedModel, QObject* tree);
    QStringList getDefaultFilesForModel(QObject *loadedModel);
    bool selectDescription(const QStringList& filenames, int description_id);
    bool refreshDescribingLists(int descriptionId, QObject * model);
    void initializeObjects();
    void connectActionsToObjects();
    virtual void allocateCorrelationModel() = 0;
    virtual void allocateMemory();
    virtual void setModelsAndTreesObjects();
    void loadDescriptionModelFromFiles(const QStringList &filenames, int descriptionId);
    void loadDataFromFilesWithData(const QStringList &filenames, int descriptionId);
    virtual QStringList setNameFilterForTargetFiles();
    virtual QStringList setNameFilterForTemplateFiles();
    virtual QStringList setNameFilterForDataTemplateFiles();
    virtual QStringList setNameFilterForDataTargetFiles();
    QStringList openFilesByAnyNameFilter(const QStringList &nameFilter);
    QStringList openDataFilesByAnyNameFilter(const QStringList &nameFilter);
    void defaultTranslateSets();
    void loadTranslationFiles();
protected slots:
    void onOpenTargetFiles();
    void onOpenTemplateFiles();

    virtual void createObjects();
    virtual void freeObjects();

    void onLoadTemplateData();
    void onLoadTargetData();
    virtual void ElementTreeTemplateActivated(const QModelIndex&);
    virtual void ElementTreeTargetActivated(const QModelIndex&);
    virtual void ElementTableActivated(const QModelIndex &);
    void FillTable();
    void onExportTemplateDataToDB();
    void onExportTargetDataToDB();
    void makeRequestSlot();

    void onSaveCorrelationTable();
    void onLoadCorrelationTable();

    void completeAddingToDB(int , QString );

    void onEnglishChecked(bool checked);
    void onRussianChecked(bool checked);

signals:
    void loadDescComplete();
    void CreateObjects();
};



#endif // PRESENTER_H
