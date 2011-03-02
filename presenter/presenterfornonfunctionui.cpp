#include <QModelIndex>
#include <QDesktopWidget>
#include <QApplication>

#include "presenterfornonfunctionui.h"
#include "constants.h"
#include "correlationmodelnofunction.h"
#include "qmodeldescribingDEMO.h"
#include "qmodeldescribingfromjura.h"
#include "qmodeldescribingPROS.h"
#include "qmodeldescribingOLD4.h"
#include "treeviewmodel.h"

// TODO: нельзя менять местами таргет и темлейт. Сделать это
void PresenterForNonFunctionUI::allocateCorrelationModel()
{

    _view->corrModel = new CorrelationModelNoFunction((QWidget *)_view,
                                                    MODELS(TEMPLATEDESC),
                                                    MODELS(TARGETDESC));
    connect(CORR_MODEL, SIGNAL(doubleClicked(const QModelIndex&)), this,
            SLOT(ElementTableActivated(const QModelIndex&)), Qt::QueuedConnection);
}

void PresenterForNonFunctionUI::createObjects()
{
    Presenters::createObjects();
    setupAddTableForm();
}

void PresenterForNonFunctionUI::allocateMemory()
{
    Presenters::allocateMemory();
    //New Spravka's files.
    _view->modelJURA = new QModelDescribingFromJura();
}

void PresenterForNonFunctionUI::setModelsAndTreesObjects()
{
     _view->models[TARGETDESC] = _view->modelP;
     _view->models[TEMPLATEDESC] = _view->modelJURA;

//    _view->models[TARGETDESC] = _view->modelP;
//    _view->models[TEMPLATEDESC] = _view->modelD;

    _view->trees = new QObject*[2];
    _view->trees[TARGETDESC] = new TreeViewModel((QWidget *)_view, TARGETDESC);
    _view->trees[TEMPLATEDESC] = new TreeViewModel((QWidget *)_view, TEMPLATEDESC);
}

void PresenterForNonFunctionUI::freeObjects()
{
    //Delete target
    if (_view->model4) delete _view->model4;
    //Delete template
    if (_view->modelD) delete _view->modelD;

    //Delete template pros
    if (_view->modelP) delete _view->modelP;

    if(_view->modelJURA) delete _view->modelJURA;

    //Delete corr model
    if (_view->corrModel) delete _view->corrModel;
    if (_view->models) delete _view->models;
    if (_view->trees[TARGETDESC]) delete _view->trees[TARGETDESC];
    if (_view->trees[TEMPLATEDESC]) delete _view->trees[TEMPLATEDESC];
    if (_view->trees) delete _view->trees;

    //Delete additional form
    if(AddTableForm) delete AddTableForm;
}

void PresenterForNonFunctionUI::ElementTableActivated(const QModelIndex & index)
{
    qDebug() << "index = " << index.data(Qt::UserRole + 1);
    //Определить row
    qDebug() << " index.column():=" << index.column();
    qDebug() << " index.row():=" << index.row();
    Presenters::_view->activeRowId = index.row();//???

    //TODO implement three methods
    switch (index.column())
    {
        case iTemplate:
            {
                CORR_MODEL->setApplyTreeClick(iTemplate);
                _view->updateTextLabel("Change template variable. Row: " + QVariant(index.row() + 1).toString() + " Column: " + QVariant(index.column() + 1).toString());
            }
            break;
        case iTarget:
            {
                CORR_MODEL->setApplyTreeClick(iTarget);
                _view->updateTextLabel("Change target variables. Row: " + QVariant(index.row() + 1).toString() + " Column: " + QVariant(index.column() + 1).toString());
                Presenters::_view->countTV = true;
                if( !AddTableForm->isVisible() )
                {
                    emit sendDataToAddCorrForm(index);
                    AddTableForm->show();
                }
                else
                {
                    _view->updateTextLabel("You don't have open more than one additional correlation form!");
                }

            }
            break;
        default:
            qWarning() << " Strange column??";
    }
}

void PresenterForNonFunctionUI::setupAddTableForm()
{
    AddTableForm = new AdditionCorrelationTable(this, _view);
    connect(AddTableForm, SIGNAL(sendDataToMainTableForm(const QModelIndex &)),this,
            SLOT(getDataFromAddCorrelationTable(const QModelIndex &)), Qt::QueuedConnection);
}

void PresenterForNonFunctionUI::getDataFromAddCorrelationTable(const QModelIndex &index)
{
    qDebug()<<"Calling correlation model to fix table model";
    qDebug()<<index.data(Qt::UserRole + 1);
    qDebug()<<"index.row() - "<<index.row();
    qDebug()<<"index.column() - "<<index.column();
}

//------------------------------------------------------------------------
//Addition correlation Form
AdditionCorrelationTable::AdditionCorrelationTable(Presenters *presenter, IView *_view):
    QWidget()
{
    QDesktopWidget* d = QApplication::desktop();
    setGeometry(d->width() / 2, d->height() / 3, 300, 100);
    setWindowIcon(_view->windowIcon());

    viewMainCorrs = new QTableView(this);
    tableModel = new QStandardItemModel;
    viewMainCorrs->setModel(tableModel);

    cLayout = new QVBoxLayout;
    cLayout->addWidget(viewMainCorrs);
    setLayout(cLayout);

    connect(presenter, SIGNAL(sendDataToAddCorrForm(const QModelIndex &) ), this,
            SLOT(processDataFromMainPresenter(const QModelIndex &)), Qt::QueuedConnection);

    connect(viewMainCorrs, SIGNAL(doubleClicked(const QModelIndex&)), this,
            SLOT(elementInCorrTableActivated(const QModelIndex&)), Qt::QueuedConnection);
}

void AdditionCorrelationTable::closeEvent(QCloseEvent */*clEvent*/)
{
    qDebug() << " ::AdditionalCorrelation table was closed";
    emit sendDataToMainTableForm(corrAdditionMap);
}

void AdditionCorrelationTable::processDataFromMainPresenter(const QModelIndex & tableData)
{
    corrAdditionMap = tableData;
    qDebug()<<" dataSended is corrAdditionMap";

    tableModel->clear();
    tableModel = createModelFromData(corrAdditionMap.data(Qt::UserRole + 1));
    tableModel->setHorizontalHeaderLabels(QStringList()<<"Heads"<<"Depend fields");
    tableModel->setVerticalHeaderLabels(QStringList()<<"Names"<<"Codes"<<"Values"<<"Depend ID");

    viewMainCorrs->resizeColumnsToContents();
    this->setWindowTitle(corrAdditionMap.data(Qt::UserRole + 1).toMap().value("templateName").toString() + " :: " + corrAdditionMap.data(Qt::UserRole + 1).toMap().value("typeName").toString());
}

QStandardItemModel* AdditionCorrelationTable::createModelFromData(const QVariant &corrData)
{
   QStandardItemModel *tModel = tableModel;
   QList<QStandardItem *> tNames;
   QList<QStandardItem *> tCodes;
   QList<QStandardItem *> tValues;
   QList<QStandardItem *> tDepIds;

   if( !corrData.isNull())
   {
       QVariantList names = corrData.toMap().value("itemNames").toList();
       QVariantList codes = corrData.toMap().value("codeValues").toList();
       QVariantList corrs = corrData.toMap().value("corrValues").toList();
       QVariantList depids = corrData.toMap().value("dependIds").toList();

       tNames = fillRowInAdditionalTable(names);
       tCodes = fillRowInAdditionalTable(codes);
       tValues = fillRowInAdditionalTable(corrs);
       tDepIds = fillRowInAdditionalTable(depids);
   }

   tModel->insertRow(0, tNames);
   tModel->insertRow(1, tCodes);
   tModel->insertRow(2, tValues);
   tModel->insertRow(3, tDepIds);

   return tModel;
}

QList<QStandardItem *> AdditionCorrelationTable::fillRowInAdditionalTable(QVariantList & elfill)
{
   QList<QStandardItem *> tmpList;
   tmpList.append(new QStandardItem(elfill.takeFirst().toString()));
   if(elfill.count() > 0)
   {
       for(int i = 0; i < elfill.count(); ++i)
       {
            tmpList.append( new QStandardItem(elfill.at(i).toString()));
       }
   }
   return tmpList;
}

void AdditionCorrelationTable::elementInCorrTableActivated(const QModelIndex &elementData)
{
    //process clicking
    Q_UNUSED(elementData);
    qDebug();
}
