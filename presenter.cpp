#include <QFileDialog>
#include <QDesktopWidget>
#include <QThreadPool>


#include "presenter.h"

#include "constants.h"
#include "correlationmodel.h"
#include "qmodeldescribingDEMO.h"
#include "qmodeldescribingOLD4.h"
#include "qmodeldescribingPROS.h"
#include "treeviewmodel.h"
#include "querymodel.h"
#include "mssqlquery.h"

Presenters::Presenters(IView *view)
{
    _view = view;

    connect(_view, SIGNAL(OnOpenTargetFiles()),this,
             SLOT(onOpenTargetFiles()), Qt::QueuedConnection);
    connect(_view, SIGNAL(OnOpenTemplateFiles()), this,
             SLOT(onOpenTemplateFiles()), Qt::QueuedConnection);

    connect(_view, SIGNAL(OnLoadTemplateData()),this,
            SLOT(onLoadTemplateData()), Qt::QueuedConnection);
    connect(_view, SIGNAL(OnLoadTargetData()),this,
            SLOT(onLoadTargetData()), Qt::QueuedConnection);

    connect(_view, SIGNAL(CreateObjects()), this,
            SLOT(createObjects()), Qt::DirectConnection);
    connect(_view, SIGNAL(FreeObjects()), this,
            SLOT(freeObjects()), Qt::DirectConnection);

    connect(_view, SIGNAL(OnExportTemplateDataToDB()), this,
            SLOT(onExportTemplateDataToDB()), Qt::QueuedConnection);
    connect(_view, SIGNAL(OnExportTargetDataToDB()), this,
            SLOT(onExportTargetDataToDB()), Qt::QueuedConnection);

    connect(_view, SIGNAL(OnSaveCorrelationTable()), this,
            SLOT(onSaveCorrelationTable()), Qt::QueuedConnection);
    connect(_view, SIGNAL(OnLoadCorrelationTable()), this,
            SLOT(onLoadCorrelationTable()), Qt::QueuedConnection);
}

void Presenters::onSaveCorrelationTable()
{
    //TEST IT
    QFileDialog dialog((QWidget *)_view);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::List);
    QString fname = dialog.getSaveFileName((QWidget *)_view, tr("Save File"),
                                           QString(),
                                           tr("CorrelationModel files (*.cmf)"));
    if (!fname.endsWith(cmf))
    {
        fname += cmf;
    }
    QFile corrModelFile(fname);
    if (corrModelFile.open(QIODevice::WriteOnly))
    {
        QDataStream out(&corrModelFile);
        QVariantMap tableRecord;
        QVariantMap oneRecord;
        //target
        QVariantList iList = CORR_MODEL->getTargetModel()->getVisibleElements();

        oneRecord.insert(TargetSign, iList);
        iList = CORR_MODEL->getTargetModel()->getElementsWithData();
        oneRecord.insert(TargetData, iList);
        iList = CORR_MODEL->getTargetModel()->getElementsWithoutData();
        oneRecord.insert(TargetDesc, iList);

        //template
        iList = CORR_MODEL->getCurrentModel()->getVisibleElements();
        oneRecord.insert(TemplateSign, iList);
        iList = CORR_MODEL->getCurrentModel()->getElementsWithData();
        oneRecord.insert(TemplateData, iList);
        iList = CORR_MODEL->getCurrentModel()->getElementsWithoutData();
        oneRecord.insert(TemplateDesc, iList);

        //table
        tableRecord = CORR_MODEL->tableModelToMap();
        oneRecord.insert(TableRecord, tableRecord);
        out << oneRecord;
        //this->statusBar()->showMessage("Correlation table was saved", 3000);
    }
    corrModelFile.close();
}

void Presenters::onLoadCorrelationTable()
{
    //Загрузить из файла модель таблицы
    //Удалить предыд таблицу и нарисовать новую
    QFileDialog dialog((QWidget *)_view);
    QStringList filenames;
    QString fname = "";
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("CorrelationModel files (*.cmf)"));
    dialog.setViewMode(QFileDialog::List);
    if (dialog.exec())
    {
        filenames = dialog.selectedFiles();
    }
    foreach(QString f, filenames)
    {
        //Берём первый попавшийся файл
        fname = f;
        break;
    }
    if (fname == "")
    {
        qDebug() << " File isn't loaded";
    }
    else
    {
        QFile cModelFile(fname);
        if (cModelFile.open(QIODevice::ReadOnly))
        {
            QDataStream  in(&cModelFile);
            QVariantMap record;
            in >> record;
            CORR_MODEL->fillInTable(record.value(TableRecord).toMap());
            //this->statusBar()->showMessage("Correlation table was loaded", 3000);
        }
        cModelFile.close();
    }
}

void Presenters::onExportTemplateDataToDB()
{
    qDebug();
    if (MODELS(TEMPLATEDESC)->isValidElementsWithData())
    {
        //Make string request
        qDebug() << "=======Start Loading Template======";
        _view->queryModel = new querymodel(CORR_MODEL);
        QUERY_MODEL->setAutoDelete(false);
        connect(QUERY_MODEL, SIGNAL(makeRequestSignal()), this,
                SLOT(makeRequestSlot()), Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(QUERY_MODEL);

        //TODO move to slot

        qDebug() << "=======End Template======";
    }
    else
    {
        qDebug() << " Template data wasn't loaded";
       // this->statusBar()->showMessage("Template data wasn't loaded", 3000);
    }
}

void Presenters::makeRequestSlot()
{
    qDebug();
    //Adding data to DB
    _view->mssqlQuery = new mssqlquery(0, QUERY_MODEL);
    connect(MSSQL_QUERY, SIGNAL(complete(int, QString)), this,
            SLOT(completeAddingToDB(int, QString)), Qt::QueuedConnection);
    QThreadPool::globalInstance()->start(MSSQL_QUERY);
}

void Presenters::completeAddingToDB(int aError, QString errStr)
{
    qDebug() << " id = " << aError << " Msg: " << errStr;
    //pLabel->setText("Adding data was complete. ErrorMessage: " + errStr);
    //Clear Data.
    MODELS(TARGETDESC)->clearElementsWithData();
    MODELS(TEMPLATEDESC)->clearElementsWithData();
    // Remove unneeded querymodel
    if (_view->queryModel)
    {
        delete _view->queryModel;
        _view->queryModel = NULL;
    }
}

void Presenters::onExportTargetDataToDB()
{
    qDebug();
    //У нас должны быть загружены данные и модель демо версии
    if (MODELS(TARGETDESC)->isValidElementsWithData() &&
        MODELS(TEMPLATEDESC)->isValidElementsWithoutData())
    {
        //Make string request
        qDebug() << "=======Start Converting======";
        _view->queryModel = new querymodel(CORR_MODEL);
        QUERY_MODEL->setAutoDelete(false);
        connect(QUERY_MODEL, SIGNAL(makeRequestSignal()), this,
                SLOT(makeRequestSlot()), Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(QUERY_MODEL);
        qDebug() << "=======End Converting======";
    }
    else
    {
        qDebug() << " Target data wasn't loaded";
        //this->statusBar()->showMessage("Target data wasn't loaded", 3000);
    }
}

void Presenters::createObjects()
{
    allocateMemoryForObjects();
    loadingModels(MODELS(TEMPLATEDESC), TREES(TEMPLATEDESC));
    loadingModels(MODELS(TARGETDESC), TREES(TARGETDESC));
}

void Presenters::freeObjects()
{
    //Delete target
    if (_view->model4) delete _view->model4;
    //Delete template
    if (_view->modelD) delete _view->modelD;

    //Delete template pros
    if (_view->modelP) delete _view->modelP;


    //Delete corr model
    if (_view->corrModel) delete _view->corrModel;
    if (_view->models) delete _view->models;
    if (_view->trees[TARGETDESC]) delete _view->trees[TARGETDESC];
    if (_view->trees[TEMPLATEDESC]) delete _view->trees[TEMPLATEDESC];
    if (_view->trees) delete _view->trees;
}

void Presenters::FillTable()
{
    qDebug() << " fill in table if it need";
    if (MODELS(TARGETDESC)->isValidElementsWithoutData() &&
        MODELS(TEMPLATEDESC)->isValidElementsWithoutData())
    {
        CORR_MODEL->fillInTable();
    }
    else
    {
        qWarning() << " Desc loaded incomplete";
    }
}

void Presenters::loadingModels(QObject* loadedModel, QObject* tree)
{
    QStringList filenames;
    if (loadedModel == _view->modelD)
    {
        filenames.append(demopath);
    }
    else if (loadedModel == _view->model4)
    {
        filenames.append(target1path);
        filenames.append(target2path);
        filenames.append(target3path);
        filenames.append(target4path);
    }
    else if (loadedModel == _view->modelP)
    {
        filenames.append(prosPathF1);
        filenames.append(prosPathF2);
        filenames.append(prosPathF5);
        filenames.append(prosPathF12);
    }


    foreach(QString fname, filenames)
    {
        ((QModelDescribing *)loadedModel)->appendFromDataFilesToDataElements(fname);
    }
    if (((QModelDescribing *)loadedModel)->getElementsWithoutData().count() > 0)
    {
        //TODO попробовать просто отобразить модель
        ((QModelDescribing *)loadedModel)->createTreeForViewing();
        ((TreeViewModel *)tree)->loadModel((QModelDescribing *)loadedModel);
        //pLabel->setText("Model was loaded");
        emit loadDescComplete();
    }
}

void Presenters::allocateMemoryForObjects()
{
    // TODO: create factory of objects
    //Multiple case. Old Spravka's files
    _view->model4 = new QModelDescribingOld4();

    //One to one case. Demo version
    _view->modelD = new QModelDescribingDemo();

    //Cases from prosecutor's office
    _view->modelP = new QModelDescribingPros();

    _view->models = new QObject*[2];

    _view->models[TARGETDESC] = _view->modelP;
    _view->models[TEMPLATEDESC] = _view->modelD;

    _view->trees = new QObject*[2];
    _view->trees[TARGETDESC] = new TreeViewModel((QWidget *)_view, TARGETDESC);
    _view->trees[TEMPLATEDESC] = new TreeViewModel((QWidget *)_view, TEMPLATEDESC);

    connect(TREES(TARGETDESC), SIGNAL(doubleClicked(const QModelIndex&)), this,
            SLOT(ElementTreeTargetActivated(const QModelIndex&)), Qt::QueuedConnection);
    connect(TREES(TEMPLATEDESC), SIGNAL(doubleClicked(const QModelIndex&)), this,
            SLOT(ElementTreeTemplateActivated(const QModelIndex&)), Qt::QueuedConnection);


    connect(this, SIGNAL(loadDescComplete()), this, SLOT(FillTable()), Qt::QueuedConnection);
    //Correlation model
    allocateCorrelationModel();   
}

void Presenters::ElementTableActivated(const QModelIndex & index)
{
    qDebug() << "index = " << index.data(Qt::UserRole + 1);
    //Определить row
    qDebug() << " index.column():=" << index.column();
    qDebug() << " index.row():=" << index.row();
    _view->activeRowId = index.row();

    //TODO implement three methods
    switch (index.column())
    {
        case iTemplate:
            {
                CORR_MODEL->setApplyTreeClick(iTemplate);
                //pLabel->setText("Change template variable. Row: " + QVariant(index.row() + 1).toString() + " Column: " + QVariant(index.column() + 1).toString());
            }
            break;
        case iTarget:
            {
                CORR_MODEL->setApplyTreeClick(iTarget);
                //pLabel->setText("Change target variables. Row: " + QVariant(index.row() + 1).toString() + " Column: " + QVariant(index.column() + 1).toString());
                _view->countTV = true;
            }
            break;
        default:
            qWarning() << " Strange column??";
    }
}

void Presenters::ElementTreeTemplateActivated(const QModelIndex& index)
{
    qDebug() << "index = " << index.data(Qt::UserRole + 1);
    if (CORR_MODEL->applyTreeClick(iTemplate) &&
        MODELS(TEMPLATEDESC)->isVisibleElement(index.data(Qt::UserRole + 1)))
    {
        //Нужен только один элемент Лучше использовать QVariant ?
        CORR_MODEL->changeTemplateValue(iTemplate, _view->activeRowId, index.data(Qt::UserRole + 1));
    }
    else
    {
        qDebug() << " template table cell isn't checked or not applicable";
        //this->statusBar()->showMessage("Template table cell isn't checked or not applicable", 3000);
    }
}

void Presenters::ElementTreeTargetActivated(const QModelIndex& index)
{
    qDebug() << "index = " << index.data(Qt::UserRole + 1);
    qDebug() << "index.row() = " << index.row();
    if (CORR_MODEL->applyTreeClick(iTarget) &&
        MODELS(TARGETDESC)->isVisibleElement(index.data(Qt::UserRole + 1)))
    {
        //TODO как-то нужно это добавить в таблицу
        CORR_MODEL->changeTargetValue(iTarget, _view->activeRowId, index.data(Qt::UserRole + 1), _view->countTV);
        _view->countTV = false;
    }
    else
    {
        qDebug() << " target table cell isn't checked or not applicable";
        //this->statusBar()->showMessage("Target table cell isn't checked or not applicable", 3000);
    }
}


void Presenters::onOpenTargetFiles()
{
    QStringList filenames = openFilesByAnyNameFilter(setNameFilterForTargetFiles());
    loadDescriptionModelFromFiles(filenames, TARGETDESC);
}

QString Presenters::setNameFilterForTargetFiles()
{
    return QString("Text target files (Sprav?.txt ; sprav_d.txt ; F*.TXT)");
}

void Presenters::loadDescriptionModelFromFiles(const QStringList &filenames, int descriptionId)
{
    if (selectDescription(filenames, descriptionId))
    {
        MODELS(descriptionId)->clearAllElements();
        CORR_MODEL->clearCorrelationTable();
        foreach(QString fname, filenames)
        {
            MODELS(descriptionId)->appendFromDataFilesToDataElements(fname);
        }
        if ( MODELS(descriptionId)->isExistElementsWithoutData())
        {
            //TODO попробовать просто отобразить модель
            MODELS(descriptionId)->createTreeForViewing();
            TREES(descriptionId)->loadModel(MODELS(descriptionId));
            //pLabel->setText("Target model was created");
            emit loadDescComplete();
        }
    }
}

void Presenters::onOpenTemplateFiles()
{
    QStringList filenames = openFilesByAnyNameFilter(setNameFilterForTemplateFiles());
    loadDescriptionModelFromFiles(filenames, TEMPLATEDESC);
}

QString Presenters::setNameFilterForTemplateFiles()
{
    return QString("Text template files (Sprav?.txt ; sprav_d.txt ; F*.TXT)");
}

QStringList Presenters::openFilesByAnyNameFilter(const QString &nameFilter)
{
    QFileDialog dialog((QWidget *)_view);
    QStringList filenames;
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr(nameFilter));
    dialog.setViewMode(QFileDialog::List);
    if (dialog.exec())
    {
        filenames = dialog.selectedFiles();
    }
    return filenames;
}


bool Presenters::selectDescription(const QStringList& filenames, int description_id)
{
    // Информация об именах файлов
    int old4 = 0, demo = 0, pros = 0;
    if (filenames.count() > 0)
    {
        foreach(QString fname, filenames)
        {
            if (fname.contains("Sprav", Qt::CaseSensitive))
            {
                old4++;
            }
            else if (fname.contains("sprav_d", Qt::CaseSensitive))
            {
                demo++;
            }
            else if (fname.contains("F", Qt::CaseSensitive))
            {
                pros++;
            }
        }

        if (old4 == filenames.count())
        {
            return refreshDescribingLists(description_id, _view->model4);
        }
        else if (demo == filenames.count())
        {
            return refreshDescribingLists(description_id, _view->modelD);
        }
        else if (pros == filenames.count())
        {
            return refreshDescribingLists(description_id, _view->modelP);
        }
    }
    return false;
}

bool Presenters::refreshDescribingLists(int descriptionId, QObject * model)
{
    switch (descriptionId)
    {
        case TARGETDESC:
            {
                if (_view->models[TEMPLATEDESC] == model) return false;
            }
            break;
        case TEMPLATEDESC:
            {
                if (_view->models[TARGETDESC] == model) return false;
            }
            break;
        default:
            return false;
    }
    _view->models[descriptionId] = model;
    _view->removeWidgets();
    allocateCorrelationModel();
    _view->addWidgets();
    return true;
}

void Presenters::onLoadTemplateData()
{
    qDebug();
    if (!MODELS(TEMPLATEDESC)->isValidElementsWithoutData())
    {
        qWarning() << " Description wasn't loaded";
      //  this->statusBar()->showMessage("Template Description wasn't loaded", 5000);
        return;
    }
    QStringList filenames = openDataFilesByAnyNameFilter(setNameFilterForDataTemplateFiles());
    loadDataFromFilesWithData(filenames, TEMPLATEDESC);
}

void Presenters::loadDataFromFilesWithData(const QStringList &filenames, int descriptionId)
{
    //Append data
    foreach(QString fname, filenames)
    {
        MODELS(descriptionId)->loadingDataElementsFromFile(fname);
    }
    // TODO make message when error happened
    if (MODELS(descriptionId)->isValidElementsWithDataForParticularFile())
    {
        MODELS(descriptionId)->preparingDataStructureBeforeFilling();
       // pLabel->setText("Template data successfully loaded");
    }
    else
    {
        //pLabel->setText("Data didn't load");
    }
}

QString Presenters::setNameFilterForDataTemplateFiles()
{
    return QString("Text template data files (*_C_*.txt ; *_FG_*.txt ; *_O_*.txt ; *__S.txt ; *_FP_*.txt ; F*.TXT ; data_sprav_d.txt)");
}

QStringList Presenters::openDataFilesByAnyNameFilter(const QString &nameFilter)
{
    QFileDialog dialog((QWidget *)_view);
    QStringList filenames;
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr(nameFilter));
    dialog.setViewMode(QFileDialog::List);
    if (dialog.exec())
    {
        filenames = dialog.selectedFiles();
    }
    return filenames;
}

void Presenters::onLoadTargetData()
{
    qDebug();
    //Create file dialog
    if (!MODELS(TARGETDESC)->isValidElementsWithoutData())
    {
        qWarning() << " Description wasn't loaded";
       // this->statusBar()->showMessage("Target Description wasn't loaded", 5000);
        return;
    }
    QStringList filenames = openDataFilesByAnyNameFilter(setNameFilterForDataTargetFiles());
    loadDataFromFilesWithData(filenames, TARGETDESC);
}

QString Presenters::setNameFilterForDataTargetFiles()
{
    return QString("Text target data files (*_C_*.txt ; *_FG_*.txt ; *_O_*.txt ; *__S.txt ; *_FP_*.txt; F*.TXT)");
}
