#include <QDebug>
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

    //TODO implement it
    //Correlation model
    corrModel = new CorrelationModel(this,modelD,model4);
    connect(corrModel,SIGNAL(doubleClicked(const QModelIndex &)),this,
            SLOT(ElementTableActivated(const QModelIndex &)),Qt::QueuedConnection);


    connect(this,SIGNAL(loadDescComplete()),this,
            SLOT(FillTable()),Qt::QueuedConnection);

    //QueryModel
   // queryModel = new querymodel(corrModel);
    //queryModel4 = new querymodel(model4);
    //connect(mssqlQuery4,SIGNAL(complete(int,QString)),this,
            //SLOT(completeAddingToDB(int, QString)), Qt::QueuedConnection);


    //Put tree model on widget
    layout  = new QHBoxLayout;
    //template
    layout->addWidget(treed);
    //table - corrModel
    layout->addWidget(corrModel);
    //target
    layout->addWidget(tree4);
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
    //Delete corr model
    if(corrModel) delete corrModel;
    //Delete query model
    //if(queryModel) delete queryModel;
    //if(queryModel4) delete queryModel4;

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
    qDebug()<<Q_FUNC_INFO;
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
        model4->resetAllList();
        corrModel->clearTable();
    }
    for(int i = 0; i < filenames.count() ;i++)
    {
        model4->appendToList(filenames[i]);
    }
    if(model4->getListDescribing().count() > 0)
    {
        //TODO попробовать просто отобразить модель
        model4->createModel();
        tree4->loadModel(model4);
        pLabel->setText("Target model was created");
        emit loadDescComplete();
    }

    //Получить данные из модели
}

void converter::on_actionOpen_template_triggered()
{
    //Template file
    qDebug()<<Q_FUNC_INFO;
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
        modelD->resetAllList();
        corrModel->clearTable();
    }
    for(int i = 0; i < filenames.count() ;i++)
    {
        modelD->appendToList(filenames[i]);
    }
    if(modelD->getListDescribing().count() > 0)
    {
        //TODO попробовать просто отобразить модель
        modelD->createModel();
        treed->loadModel(modelD);
        pLabel->setText("Template model was created");
        emit loadDescComplete();
    }
}


void converter::ElementTreeTargetActivated(const QModelIndex &index)
{
    qDebug()<<Q_FUNC_INFO<<"index = "<<index.data(Qt::UserRole + 1);
    qDebug()<<Q_FUNC_INFO<<"index.row() = "<<rowId;
    if(corrModel->applyTreeClick(iTarget) && model4->isSignificant(index.data(Qt::UserRole + 1)))
    {
        //TODO как-то нужно это добавить в таблицу
        corrModel->changeTargetValue(iTarget,rowId,index.data(Qt::UserRole + 1),countTV);
        countTV = false;
    }
    else
    {
        qDebug()<<Q_FUNC_INFO<< " target table cell isn't checked or not applicable";
        this->statusBar()->showMessage("Target table cell isn't checked or not applicable",3000);

    }
}
void converter::ElementTreeTemplateActivated(const QModelIndex &index)
{
    qDebug()<<Q_FUNC_INFO<<"index = "<<index.data(Qt::UserRole + 1);
    qDebug()<<Q_FUNC_INFO<<"index.row() = "<<rowId;
    if(corrModel->applyTreeClick(iTemplate) && modelD->isSignificant(index.data(Qt::UserRole + 1)))
    {
        //Нужен только один элемент Лучше использовать QVariant ?
        corrModel->changeTemplateValue(iTemplate,rowId,index.data(Qt::UserRole + 1));
    }
    else
    {
        qDebug()<<Q_FUNC_INFO<< " template table cell isn't checked or not applicable";
        this->statusBar()->showMessage("Template table cell isn't checked or not applicable",3000);
    }
}

void converter::ElementTableActivated(const QModelIndex &index)
{
    qDebug()<<Q_FUNC_INFO<<"index = "<<index.data(Qt::UserRole + 1);
    //Определить row
    qDebug()<<Q_FUNC_INFO<<" index.column():="<<index.column();
    qDebug()<<Q_FUNC_INFO<<" index.row():="<<index.row();
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
        qWarning()<<Q_FUNC_INFO<<" Strange column??";
    }

}
void converter::FunctionIsChecked(int id)
{
    qDebug()<<Q_FUNC_INFO<<" change function in table. ID = "<<id;
    qDebug()<<Q_FUNC_INFO<<"index.row() = "<<rowId;
    if(corrModel->applyTreeClick(iFunction) )
    {
        corrModel->changeFunctionValue(iFunction,rowId,id);
    }
    else
    {
        qDebug()<<Q_FUNC_INFO<< " function table cell isn't checked or not applicable";
        this->statusBar()->showMessage("Function table cell isn't checked or not applicable",3000);
    }
}


void converter::FillTable()
{
    qDebug()<<Q_FUNC_INFO<<" fill in table if it need";
    //сдклать isValid
    if(model4->isValid() && modelD->isValid())
    {
        //TODO do something
        corrModel->fillInTable();
    }
    else
    {
        qWarning()<<Q_FUNC_INFO<<" Desc loaded incomplete";
    }
}

void converter::on_actionLoad_Template_Data_triggered()
{
    qDebug()<<Q_FUNC_INFO;
    //Create file dialog
    if(!modelD->isValid())
    {
        qWarning()<<Q_FUNC_INFO<<" Description wasn't loaded";
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
    //---------------------
    if(filenames.count() > 0)
    {
        //Reset data
        modelD->resetDataList();
    }
    for(int i = 0; i < filenames.count();i++)
    {
        modelD->loadingData(filenames[i]);
    }
}

void converter::on_actionLoad_Target_Data_triggered()
{
    qDebug()<<Q_FUNC_INFO;
    qDebug()<<Q_FUNC_INFO;
    //Create file dialog
    if(!model4->isValid())
    {
        qWarning()<<Q_FUNC_INFO<<" Description wasn't loaded";
        this->statusBar()->showMessage("Target Description wasn't loaded",5000);
        return;
    }
    QFileDialog dialog(this);
    QStringList filenames;
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Text target data files (*_C_*.txt ; *_FG_*.txt ; *_O_*.txt ; *__S.txt ; *_FP_*.txt)"));
    dialog.setViewMode(QFileDialog::List);
    if(dialog.exec())
    {
        filenames = dialog.selectedFiles();
    }
    //---------------------
    foreach(QString fname, filenames)
    {
        model4->loadingData(fname);
    }
    if(model4->isValidDataTemp())
    {
        //Reset data
        model4->resetDataList();
        model4->dataPrepare();
        pLabel->setText("Target data successfully loaded");
    }
}

void converter::on_actionExport_template_data_to_DB_triggered()
{
    qDebug()<<Q_FUNC_INFO;
    if(modelD->isValidData())
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
        qDebug()<<Q_FUNC_INFO<<" Template data wasn't loaded";
        this->statusBar()->showMessage("Template data wasn't loaded",3000);
    }
}

void converter::on_actionConvert_files_triggered()
{
    qDebug()<<Q_FUNC_INFO;//У нас должны быть загружены данные и модель демо версии
    if(model4->isValidData() && modelD->isValid())
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
        qDebug()<<Q_FUNC_INFO<<" Target data wasn't loaded";
        this->statusBar()->showMessage("Target data wasn't loaded",3000);
    }
}

void converter::on_actionExport_all_triggered()
{
    qDebug()<<Q_FUNC_INFO<<" Start exporting...";
    on_actionExport_template_data_to_DB_triggered();
    on_actionConvert_files_triggered();
    qDebug()<<Q_FUNC_INFO<<" End exporting";
}
void converter::completeAddingToDB(int aError,QString errStr)
{
    qDebug()<<Q_FUNC_INFO<<" id = "<<aError<<" Msg: "<<errStr;
    pLabel->setText("Adding data was complete. ErrorMessage: " + errStr);

}

void converter::makeRequestSlot()
{
    qDebug()<<Q_FUNC_INFO;
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

void converter::init()
{
    //Template
    qDebug()<<Q_FUNC_INFO;
    QStringList filenames;    
    filenames.clear();
    filenames.append(demopath);
    for(int i = 0; i < filenames.count() ;i++)
    {
        modelD->appendToList(filenames[i]);
    }
    if(modelD->getListDescribing().count() > 0)
    {
        //TODO попробовать просто отобразить модель
        modelD->createModel();
        treed->loadModel(modelD);
        pLabel->setText("Template model was created");
        emit loadDescComplete();
    }

    //Target
    filenames.clear();
    filenames.append(target1path);
    filenames.append(target2path);
    filenames.append(target3path);
    filenames.append(target4path);
    for(int i = 0; i < filenames.count() ;i++)
    {
        model4->appendToList(filenames[i]);
    }
    if(model4->getListDescribing().count() > 0)
    {
        //TODO попробовать просто отобразить модель
        model4->createModel();
        tree4->loadModel(model4);
        pLabel->setText("Target model was created");
        emit loadDescComplete();
    }

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
        qDebug()<<Q_FUNC_INFO<<" File isn't loaded";
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
    qDebug()<<Q_FUNC_INFO;
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
    qDebug()<<Q_FUNC_INFO<<" ::func from closed";
    int id = nonef;
    QRadioButton *r1 = this->findChild<QRadioButton *>("AGE");
    if(r1 && r1->isChecked())
    {
        //TODO emit signal to converter
        qDebug()<<Q_FUNC_INFO<<"::AGE";
        id = agef;
    }
    else
    {
        QRadioButton *r2 = this->findChild<QRadioButton *>("CONCAT");
        if(r2 && r2->isChecked())
        {
            qDebug()<<Q_FUNC_INFO<<"::CONCAT";
            id = concatf;
        }
        else
        {
            QRadioButton *r3 = this->findChild<QRadioButton *>("NONE");
            if(r3 && r3->isChecked())
            {
                qDebug()<<Q_FUNC_INFO<<"::NO FUNCTION";
                id = nonef;
            }
            else
            {
                qWarning()<<Q_FUNC_INFO<<" Error was encountered";
                id = nonef;
            }
        }
    }
    emit FuncWasChecked(id);
}



