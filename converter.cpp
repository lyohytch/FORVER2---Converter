#include <QFileDialog>
#include <QTreeView>
#include <QDesktopWidget>
#include <QThreadPool>

#include "converter.h"
#include "ui_converter.h"
#include "constants.h"

converter::converter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::converter)
{
    ui->setupUi(this);
    isTargetRow = false;
    isTemplateRow = false;
    QDesktopWidget *d = QApplication::desktop();
    this->setGeometry(100,100,d->width()/3,d->height() - 400);
    //TODO rework it for more productivity
    //Target files
    model4 = new QModelDescribingOld4();
    tree4 = new TreeViewModel(this,false);
    connect(tree4,SIGNAL(doubleClicked(const QModelIndex &)),this,
            SLOT(ElementTreeTargetActivated(const QModelIndex &)),Qt::QueuedConnection);
    //Template files
    modelD = new QModelDescribingDemo();
    treed = new TreeViewModel(this,true);
    connect(treed,SIGNAL(doubleClicked(const QModelIndex &)),this,
            SLOT(ElementTreeTemplateActivated(const QModelIndex &)),Qt::QueuedConnection);
    //Target files Pros
    modelP = new QModelDescribingPros();
    treep = new TreeViewModel(this, false);
    connect(treep, SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(ElementTreeTargetActivated(const QModelIndex &)), Qt::QueuedConnection);


    models = new QModelDescribing*[2];
    models[TARGETDESC] = modelP;
    models[TEMPLATEDESC] = modelD;

    trees = new TreeViewModel*[2];
    trees[TARGETDESC] = treep;
    trees[TEMPLATEDESC] = treed;

    //Correlation model
    corrModel = new CorrelationModel(this,models[TEMPLATEDESC],models[TARGETDESC]);
    connect(corrModel,SIGNAL(doubleClicked(const QModelIndex &)),this,
            SLOT(ElementTableActivated(const QModelIndex &)),Qt::QueuedConnection);

    connect(this,SIGNAL(loadDescComplete()),this,
            SLOT(FillTable()),Qt::QueuedConnection);


    //Put tree model on widget
    layout  = new QHBoxLayout;
    //template
    layout->addWidget(trees[TEMPLATEDESC]);
    //table - corrModel
    layout->addWidget(corrModel);
    //target
    layout->addWidget(trees[TARGETDESC]);
    this->centralWidget()->setLayout(layout);
    pLabel = new QLabel;
    pLabel->setText("Application started. Please load target and template files to start converting");
    this->statusBar()->addWidget(pLabel);

    //Create window to specify function
    funcWidget = new QWidgetFunction;
    fLayout = new QVBoxLayout;
    funcWidget->setGeometry(d->width()/2,d->height()/3,300,100);
    a1 = new QRadioButton("AGE",funcWidget);
    a2 = new QRadioButton("CONCAT",funcWidget);
    a3 = new QRadioButton("NO FUNCTION",funcWidget);
    a1->setObjectName("AGE");
    a2->setObjectName("CONCAT");
    a3->setObjectName("NONE");
    a3->setChecked(true);
    fLayout->addWidget(a1);
    fLayout->addWidget(a2);
    fLayout->addWidget(a3);
    funcWidget->setLayout(fLayout);
    connect(funcWidget,SIGNAL(FuncWasChecked(int)),this,SLOT(FunctionIsChecked(int)),Qt::QueuedConnection);
    //tests
    //funcWidget->show();
    //Automatic load Загрузить файлы из каталога ресурсы
    init();


}

converter::~converter()
{
    //Delete target
    if(model4) delete model4;
    if(tree4) delete tree4;
    //Delete template
    if(modelD) delete modelD;
    if(treed) delete treed;
    //Delete template pros
    if(modelP) delete modelP;
    if(treep) delete treep;

    //Delete corr model
    if(corrModel) delete corrModel;
    if(models) delete models;
    if(trees) delete trees;

    //Delete layout
    if(layout) delete layout;

    if(pLabel) delete pLabel;

    //Delete func form
    if(a1) delete a1;
    if(a2) delete a2;
    if(a3) delete a3;
    if(fLayout) delete fLayout;
    if(funcWidget) delete funcWidget;

    if(ui) delete ui;
}

void converter::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//Open target files
void converter::on_actionOpen_triggered()
{
    //Target file
    qDebug();
    //Create file dialog
    QFileDialog dialog(this);
    QStringList filenames;
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Text target files (Sprav?.txt)"));
    dialog.setViewMode(QFileDialog::List);
    if(dialog.exec())
    {
        filenames = dialog.selectedFiles();
    }
    //---------------------
    if(filenames.count() > 0)
    {
        models[TARGETDESC]->resetAllList();
        corrModel->clearTable();
    }
    for(int i = 0; i < filenames.count() ;i++)
    {
        models[TARGETDESC]->appendToList(filenames[i]);
    }
    if(models[TARGETDESC]->getListDescribing().count() > 0)
    {
        //TODO попробовать просто отобразить модель
        models[TARGETDESC]->createModel();
        trees[TARGETDESC]->loadModel(models[TARGETDESC]);
        pLabel->setText("Target model was created");
        emit loadDescComplete();
    }

    //Получить данные из модели
}

void converter::on_actionOpen_template_triggered()
{
    //Template file
    qDebug();
    //Create file dialog
    QFileDialog dialog(this);
    QStringList filenames;
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Text template files (sprav_d.txt)"));
    dialog.setViewMode(QFileDialog::List);
    if(dialog.exec())
    {
        filenames = dialog.selectedFiles();
    }
    //---------------------
    if(filenames.count() > 0)
    {
        models[TEMPLATEDESC]->resetAllList();
        corrModel->clearTable();
    }
    for(int i = 0; i < filenames.count() ;i++)
    {
        models[TEMPLATEDESC]->appendToList(filenames[i]);
    }
    if(models[TARGETDESC]->getListDescribing().count() > 0)
    {
        //TODO попробовать просто отобразить модель
        models[TEMPLATEDESC]->createModel();
        treed->loadModel(models[TEMPLATEDESC]);
        pLabel->setText("Template model was created");
        emit loadDescComplete();
    }
}


void converter::ElementTreeTargetActivated(const QModelIndex &index)
{
    qDebug()<<"index = "<<index.data(Qt::UserRole + 1);
    qDebug()<<"index.row() = "<<index.row();
    if(corrModel->applyTreeClick(iTarget) && models[TARGETDESC]->isSignificant(index.data(Qt::UserRole + 1)))
    {
        //TODO как-то нужно это добавить в таблицу
        corrModel->changeTargetValue(iTarget,rowId,index.data(Qt::UserRole + 1),countTV);
        countTV = false;
    }
    else
    {
        qDebug()<< " target table cell isn't checked or not applicable";
        this->statusBar()->showMessage("Target table cell isn't checked or not applicable",3000);

    }
}
void converter::ElementTreeTemplateActivated(const QModelIndex &index)
{
    qDebug()<<"index = "<<index.data(Qt::UserRole + 1);
    qDebug()<<"index.row() = "<<rowId;
    if(corrModel->applyTreeClick(iTemplate) && models[TEMPLATEDESC]->isSignificant(index.data(Qt::UserRole + 1)))
    {
        //Нужен только один элемент Лучше использовать QVariant ?
        corrModel->changeTemplateValue(iTemplate,rowId,index.data(Qt::UserRole + 1));
    }
    else
    {
        qDebug()<< " template table cell isn't checked or not applicable";
        this->statusBar()->showMessage("Template table cell isn't checked or not applicable",3000);
    }
}

void converter::ElementTableActivated(const QModelIndex &index)
{
    qDebug()<<"index = "<<index.data(Qt::UserRole + 1);
    //Определить row
    qDebug()<<" index.column():="<<index.column();
    qDebug()<<" index.row():="<<index.row();
    rowId = index.row();
    //TODO implement three methods
    switch(index.column())
    {
        case iTemplate:
        {
            corrModel->setApplyTreeClick(iTemplate);
            pLabel->setText("Change template variable. Row: "+QVariant(index.row() + 1).toString()+" Column: "+QVariant(index.column() + 1).toString());
        }
        break;
        case iTarget:
        {
            corrModel->setApplyTreeClick(iTarget);
            pLabel->setText("Change target variables. Row: "+QVariant(index.row() + 1).toString()+" Column: "+QVariant(index.column() + 1).toString());
            countTV = true;
        }
        break;
        case iFunction:
        {
            corrModel->setApplyTreeClick(iFunction);
            pLabel->setText("Change function. Row: "+QVariant(index.row() + 1).toString()+" Column: "+QVariant(index.column() + 1).toString());
            funcWidget->showNormal();
        }
        break;
    default:
        qWarning()<<" Strange column??";
    }

}
void converter::FunctionIsChecked(int id)
{
    qDebug()<<" change function in table. ID = "<<id;
    qDebug()<<"index.row() = "<<rowId;
    if(corrModel->applyTreeClick(iFunction) )
    {
        corrModel->changeFunctionValue(iFunction,rowId,id);
    }
    else
    {
        qDebug()<< " function table cell isn't checked or not applicable";
        this->statusBar()->showMessage("Function table cell isn't checked or not applicable",3000);
    }
}


void converter::FillTable()
{
    qDebug()<<" fill in table if it need";
    //сдклать isValid
    if(models[TARGETDESC]->isValid() && models[TARGETDESC]->isValid())
    {
        //TODO do something
        corrModel->fillInTable();
    }
    else
    {
        qWarning()<<" Desc loaded incomplete";
    }
}

void converter::on_actionLoad_Template_Data_triggered()
{
    qDebug();
    //Create file dialog
    if(!models[TEMPLATEDESC]->isValid())
    {
        qWarning()<<" Description wasn't loaded";
        this->statusBar()->showMessage("Template Description wasn't loaded",5000);
        return;
    }
    QFileDialog dialog(this);
    QStringList filenames;
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Text template data files (data_sprav_d.txt)"));
    dialog.setViewMode(QFileDialog::List);
    if(dialog.exec())
    {
        filenames = dialog.selectedFiles();
    }
    //Append data
    foreach(QString fname, filenames)
    {
        models[TEMPLATEDESC]->loadingData(fname);
    }
    // TODO make message when error happened
}

void converter::on_actionLoad_Target_Data_triggered()
{
    qDebug();
    //Create file dialog
    if(!models[TARGETDESC]->isValid())
    {
        qWarning()<<" Description wasn't loaded";
        this->statusBar()->showMessage("Target Description wasn't loaded",5000);
        return;
    }
    QFileDialog dialog(this);
    QStringList filenames;
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Text target data files (*_C_*.txt ; *_FG_*.txt ; *_O_*.txt ; *__S.txt ; *_FP_*.txt; F*.TXT)"));
    dialog.setViewMode(QFileDialog::List);
    if(dialog.exec())
    {
        filenames = dialog.selectedFiles();
    }

    //---------------------
    //Нужно добавлять только данные - Сделать кнопку резет


    foreach(QString fname, filenames)
    {
        models[TARGETDESC]->loadingData(fname);
    }
    //Попытаться убрать это
    if(models[TARGETDESC]->isValidDataTemp())
    {
        models[TARGETDESC]->dataPrepare();
        pLabel->setText("Target data successfully loaded");
    }
}

void converter::on_actionExport_template_data_to_DB_triggered()
{
    qDebug();
    if(models[TEMPLATEDESC]->isValidData())
    {
        //Make string request
        qDebug()<<"=======Start Loading Template======";
        queryModel = new querymodel(corrModel);
        queryModel->setAutoDelete(false);
        connect(queryModel, SIGNAL(makeRequestSignal()), this,
                SLOT(makeRequestSlot()), Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(queryModel);

        //TODO move to slot

        qDebug()<<"=======End Template======";
    }
    else
    {
        qDebug()<<" Template data wasn't loaded";
        this->statusBar()->showMessage("Template data wasn't loaded",3000);
    }
}

void converter::on_actionConvert_files_triggered()
{
    qDebug();
    //У нас должны быть загружены данные и модель демо версии
    if(models[TARGETDESC]->isValidData() && models[TEMPLATEDESC]->isValid())
    {
        //Make string request
        qDebug()<<"=======Start Converting======";
        queryModel = new querymodel(corrModel);
        queryModel->setAutoDelete(false);
        connect(queryModel, SIGNAL(makeRequestSignal()), this,
                SLOT(makeRequestSlot()), Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(queryModel);
        qDebug()<<"=======End Converting======";
    }
    else
    {
        qDebug()<<" Target data wasn't loaded";
        this->statusBar()->showMessage("Target data wasn't loaded",3000);
    }
}

void converter::on_actionExport_all_triggered()
{
    qDebug()<<" Start exporting...";
    on_actionExport_template_data_to_DB_triggered();
    on_actionConvert_files_triggered();
    qDebug()<<" End exporting";
}
void converter::completeAddingToDB(int aError,QString errStr)
{
    qDebug()<<" id = "<<aError<<" Msg: "<<errStr;
    pLabel->setText("Adding data was complete. ErrorMessage: " + errStr);

}

void converter::makeRequestSlot()
{
    qDebug();
    //Adding data to DB
    //MSSQL Query
    mssqlQuery = new mssqlquery(0,queryModel);
    //mssqlQuery4 = new mssqlquery(0,queryModel4);
    //TODO make connect to receive signal from object
    connect(mssqlQuery,SIGNAL(complete(int,QString)),this,
            SLOT(completeAddingToDB(int, QString)), Qt::QueuedConnection);
    QThreadPool::globalInstance()->start(mssqlQuery);
    if(queryModel)
    {
        delete queryModel;
        queryModel = NULL;
    }
}

void converter::init_load(QModelDescribing *loadedModel, TreeViewModel *tree)
{
    QStringList filenames;
    if(loadedModel == modelD)
    {
       filenames.append(demopath);
    }
    else if(loadedModel == model4)
    {
            filenames.append(target1path);
            filenames.append(target2path);
            filenames.append(target3path);
            filenames.append(target4path);
     }
    else if(loadedModel == modelP)
    {
            filenames.append(prosPathF1);
            filenames.append(prosPathF2);
            filenames.append(prosPathF5);
            filenames.append(prosPathF12);
    }


    foreach(QString fname, filenames)
    {
        loadedModel->appendToList(fname);
    }
    if(loadedModel->getListDescribing().count() > 0)
    {
        //TODO попробовать просто отобразить модель
        loadedModel->createModel();
        tree->loadModel(loadedModel);
        pLabel->setText("Model was loaded");
        emit loadDescComplete();
    }
}

void converter::init()
{
    //Template
    qDebug();
    init_load(models[TEMPLATEDESC], trees[TEMPLATEDESC]);
    init_load(models[TARGETDESC], trees[TARGETDESC]);
}

void converter::on_actionSave_correlation_model_triggered()
{
    //TEST IT
    QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setViewMode(QFileDialog::List);
        QString fname = dialog.getSaveFileName(this, tr("Save File"),
                                   QString(),
                                   tr("CorrelationModel files (*.cmf)"));
    if(!fname.endsWith(cmf))
    {
        fname += cmf;
    }
    QFile corrModelFile(fname);
    if(corrModelFile.open(QIODevice::WriteOnly))
    {
        QDataStream out(&corrModelFile);
        QVariantMap tableRecord;
        QVariantMap oneRecord;
        //target
        QVariantList iList = corrModel->getTargetModel()->getListSignificant();

        oneRecord.insert(TargetSign,iList);
        iList = corrModel->getTargetModel()->getListData();
        oneRecord.insert(TargetData, iList);
        iList = corrModel->getTargetModel()->getListDescribing();
        oneRecord.insert(TargetDesc,iList);

        //template
        iList = corrModel->getCurrentModel()->getListSignificant();
        oneRecord.insert(TemplateSign,iList);
        iList = corrModel->getCurrentModel()->getListData();
        oneRecord.insert(TemplateData, iList);
        iList = corrModel->getCurrentModel()->getListDescribing();
        oneRecord.insert(TemplateDesc,iList);

        //table
        tableRecord = corrModel->tableModelToMap();
        oneRecord.insert(TableRecord,tableRecord);
        out<<oneRecord;
        this->statusBar()->showMessage("Correlation table was saved",3000);
    }
    corrModelFile.close();
}

void converter::on_actionLoad_correlation_model_triggered()
{
    //Загрузить из файла модель таблицы
    //Удалить предыд таблицу и нарисовать новую
    QFileDialog dialog(this);
    QStringList filenames;
    QString fname = "";
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("CorrelationModel files (*.cmf)"));
    dialog.setViewMode(QFileDialog::List);
    if(dialog.exec())
    {
        filenames = dialog.selectedFiles();
    }
    foreach(QString f, filenames)
    {
        //Берём первый попавшийся файл
        fname = f;
        break;
    }
    if(fname == "")
    {
        qDebug()<<" File isn't loaded";
    }
    else
    {
        QFile cModelFile(fname);
        if(cModelFile.open(QIODevice::ReadOnly))
        {
            QDataStream  in(&cModelFile);
            QVariantMap record;
            in>>record;
            corrModel->fillInTable(record.value(TableRecord).toMap());
            this->statusBar()->showMessage("Correlation table was loaded",3000);
        }
        cModelFile.close();
    }
}

void converter::on_actionChange_DB_structure_triggered()
{
    qDebug();
    if(corrModel)
    {
        corrModel->clearTable();
        this->statusBar()->showMessage("Correlation table was cleared. Please load table",3000);
    }
}



//------------------------------------------------------------------------
//Func form
QWidgetFunction::QWidgetFunction():
        QWidget()
{}

void QWidgetFunction::closeEvent(QCloseEvent */*clEvent*/)
{
    qDebug()<<" ::func from closed";
    int id = nonef;
    QRadioButton *r1 = this->findChild<QRadioButton *>("AGE");
    if(r1 && r1->isChecked())
    {
        //TODO emit signal to converter
        qDebug()<<"::AGE";
        id = agef;
    }
    else
    {
        QRadioButton *r2 = this->findChild<QRadioButton *>("CONCAT");
        if(r2 && r2->isChecked())
        {
            qDebug()<<"::CONCAT";
            id = concatf;
        }
        else
        {
            QRadioButton *r3 = this->findChild<QRadioButton *>("NONE");
            if(r3 && r3->isChecked())
            {
                qDebug()<<"::NO FUNCTION";
                id = nonef;
            }
            else
            {
                qWarning()<<" Error was encountered";
                id = nonef;
            }
        }
    }
    emit FuncWasChecked(id);
}



