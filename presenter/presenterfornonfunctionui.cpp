#include <QModelIndex>
#include <QDesktopWidget>
#include <QApplication>
#include <QFileDialog>

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

    _view->corrModel = new CorrelationModelNoFunction((QWidget*)_view,
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
    _view->trees[TARGETDESC] = new TreeViewModel((QWidget*)_view, TARGETDESC);
    _view->trees[TEMPLATEDESC] = new TreeViewModel((QWidget*)_view, TEMPLATEDESC);
}

void PresenterForNonFunctionUI::freeObjects()
{
    //Delete target
    if (_view->model4) delete _view->model4;
    //Delete template
    if (_view->modelD) delete _view->modelD;

    //Delete template pros
    if (_view->modelP) delete _view->modelP;

    if (_view->modelJURA) delete _view->modelJURA;

    //Delete corr model
    if (_view->corrModel) delete _view->corrModel;
    if (_view->models) delete [] _view->models;
    if (_view->trees[TARGETDESC]) delete _view->trees[TARGETDESC];
    if (_view->trees[TEMPLATEDESC]) delete _view->trees[TEMPLATEDESC];
    if (_view->trees) delete []_view->trees;

    //Delete additional form
    if (AddTableForm) delete AddTableForm;
}

void PresenterForNonFunctionUI::ElementTableActivated(const QModelIndex& index)
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
                _view->updateTextLabel(tr("Change template variable. Row: ") + QVariant(index.row() + 1).toString() + tr(" Column: ") + QVariant(index.column() + 1).toString());
            }
            break;
        case iTarget:
            {
                CORR_MODEL->setApplyTreeClick(iTarget);
                _view->updateTextLabel(tr("Change target variables. Row: ") + QVariant(index.row() + 1).toString() + tr(" Column: ") + QVariant(index.column() + 1).toString());
                Presenters::_view->countTV = true;
                if (!AddTableForm->isVisible())
                {
                    emit sendDataToAddCorrForm(index);
                    AddTableForm->show();
                }
                else
                {
                    _view->updateTextLabel(tr("You don't have open more than one additional correlation form!"));
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
    connect(AddTableForm, SIGNAL(sendDataToMainTableForm(QStandardItem*, int, int)), this,
            SLOT(getDataFromAddCorrelationTable(QStandardItem*, int, int)), Qt::QueuedConnection);
}

void PresenterForNonFunctionUI::getDataFromAddCorrelationTable(QStandardItem* item, int mainRow, int mainColumn)
{
    qDebug() << "Calling correlation model to fix table model";
    qDebug() << item->data(Qt::UserRole + 1);
    qDebug() << "ROW - " << mainRow;
    qDebug() << "COLUMN - " << mainColumn;
    CORR_MODEL->saveDataForTarget(item->data(Qt::UserRole + 1), mainRow, mainColumn);
}

void PresenterForNonFunctionUI::ElementTreeTemplateActivated(const QModelIndex& index)
{
    qDebug() << "index = " << index.data(Qt::UserRole + 1);
    if (MODELS(TEMPLATEDESC)->isVisibleElement(index.data(Qt::UserRole + 1)))
    {
        emit sendChangedDataToAddCorrForm(iTemplate, index.data(Qt::UserRole + 1).toMap().value(id).toString());
    }
    else
    {
        qDebug() << " template table cell isn't checked or not applicable";
        _view->updateTextLabel(tr("Template table cell isn't checked or not applicable"));
    }
}

void PresenterForNonFunctionUI::ElementTreeTargetActivated(const QModelIndex& index)
{
    qDebug() << "index = " << index.data(Qt::UserRole + 1);
    qDebug() << "index.row() = " << index.row();
    if (MODELS(TARGETDESC)->isVisibleElement(index.data(Qt::UserRole + 1)))
    {
        emit sendChangedDataToAddCorrForm(iTarget, index.data(Qt::UserRole + 1).toMap().value(name).toString());
    }
    else
    {
        qDebug() << " target table cell isn't checked or not applicable";
        _view->updateTextLabel(tr("Target table cell isn't checked or not applicable"));
    }
}

//------------------------------------------------------------------------
//Addition correlation Form
AdditionCorrelationTable::AdditionCorrelationTable(Presenters* presenter, IView* _view):
    QWidget()
{
    QDesktopWidget* d = QApplication::desktop();
    setGeometry(d->width() / 2, d->height() / 3, 300, 100);
    setWindowIcon(_view->windowIcon());

    viewMainCorrs = new QTableView(this);
    tableModel = new QStandardItemModel(this);
    viewMainCorrs->setModel(tableModel);

    cLayout = new QVBoxLayout;
    cLayout->addWidget(viewMainCorrs);
    setLayout(cLayout);

    isWaitChanges = false; // Не ждём изменений


    connect(presenter, SIGNAL(sendDataToAddCorrForm(const QModelIndex&)), this,
            SLOT(processDataFromMainPresenter(const QModelIndex&)), Qt::QueuedConnection);

    connect(viewMainCorrs, SIGNAL(doubleClicked(const QModelIndex&)), this,
            SLOT(elementInCorrTableClear(const QModelIndex&)), Qt::QueuedConnection);

    connect(viewMainCorrs, SIGNAL(clicked(const QModelIndex&)), this,
            SLOT(elementInCorrTableActivated(const QModelIndex&)), Qt::QueuedConnection);

    connect(presenter, SIGNAL(sendChangedDataToAddCorrForm(int, const QString&)), this,
            SLOT(updateElementInCorrTable(int, const QString&)), Qt::QueuedConnection);


}

void AdditionCorrelationTable::updateElementInCorrTable(int descriptionId, const QString& value)
{
    qDebug() << "START";
    bool isApplicableChanges = ((descriptionId == iTarget && activeRow == 0) ||
                                (descriptionId == iTemplate && activeRow == 3));
    if (isWaitChanges && isApplicableChanges)
    {
        QStandardItem* item = new QStandardItem(value);
        item->setEditable(false);
        tableModel->setItem(activeRow, activeColumn, item);
    }
    qDebug() << "END";
}

void AdditionCorrelationTable::elementInCorrTableActivated(const QModelIndex& index)
{
    qDebug();
    bool isEditable = (index.row() == 1 || index.row() == 2);
    activeRow = index.row();
    activeColumn = index.column();
    if (isEditable)
    {
        isWaitChanges = false;
    }
    else
    {
        isWaitChanges = true;
    }
}

void AdditionCorrelationTable::closeEvent(QCloseEvent */*clEvent*/)
{
    qDebug() << " ::AdditionalCorrelation table was closed";
    isWaitChanges = false;
    QStandardItem* item  = new QStandardItem;
    item->setData(fillCorrelationMap(), Qt::UserRole + 1);
    emit sendDataToMainTableForm(item, mainRow, mainColumn);
}

QVariantMap AdditionCorrelationTable::fillCorrelationMap()
{
    QVariantMap corrMap;
    QVariantList names = QVariantList();
    QVariantList codes = QVariantList();
    QVariantList corrs = QVariantList();
    QVariantList depids = QVariantList();

    int rowCount = tableModel->rowCount();
    int colCount = tableModel->columnCount();
    for (int i = 0; i < rowCount; ++i)
        for (int j = 0; j < colCount; ++j)
        {
            if (tableModel->item(i, j) != NULL)
            {
                switch (i)
                {
                    case 0:
                        names.append(tableModel->item(i, j)->text());
                        break;
                    case 1:
                        codes.append(tableModel->item(i, j)->text());
                        break;
                    case 2:
                        corrs.append(tableModel->item(i, j)->text());
                        break;
                    case 3:
                        depids.append(tableModel->item(i, j)->text());
                        break;
                    default:
                        break;
                }
            }
        }
    corrMap.insert(itemNamesInForm, names);
    corrMap.insert(codesInForm, codes);
    corrMap.insert(corrValuesInForm, corrs);
    corrMap.insert(dependIdsInForm, depids);
    corrMap.insert(templateNameInForm, nameOfTarget);
    corrMap.insert(typeNameInForm, typeOfElement);

    return corrMap;
}

void AdditionCorrelationTable::processDataFromMainPresenter(const QModelIndex& tableData)
{
    qDebug() << " dataSended is corrAdditionMap";

    tableModel->clear();
    tableModel = createModelFromData(tableData.data(Qt::UserRole + 1));
    tableModel->setHorizontalHeaderLabels(QStringList() << tr("Heads") << tr("Depend fields"));
    tableModel->setVerticalHeaderLabels(QStringList() << tr("Names") << tr("Codes") << tr("Values") << tr("Depend ID"));

    viewMainCorrs->resizeColumnsToContents();
    nameOfTarget = tableData.data(Qt::UserRole + 1).toMap().value(templateNameInForm).toString();
    typeOfElement = tableData.data(Qt::UserRole + 1).toMap().value(typeNameInForm).toString();
    this->setWindowTitle(nameOfTarget + " :: " + typeOfElement);

    mainRow = tableData.row();
    mainColumn = tableData.column();
}

QStandardItemModel* AdditionCorrelationTable::createModelFromData(const QVariant& corrData)
{
    QStandardItemModel* tModel = tableModel;
    QList<QStandardItem*> tNames;
    QList<QStandardItem*> tCodes;
    QList<QStandardItem*> tValues;
    QList<QStandardItem*> tDepIds;

    if (!corrData.isNull())
    {
        QVariantList names = corrData.toMap().value(itemNamesInForm).toList();
        QVariantList codes = corrData.toMap().value(codesInForm).toList();
        QVariantList corrs = corrData.toMap().value(corrValuesInForm).toList();
        QVariantList depids = corrData.toMap().value(dependIdsInForm).toList();

        tNames = fillRowInAdditionalTable(names, 0);
        tCodes = fillRowInAdditionalTable(codes, 1);
        tValues = fillRowInAdditionalTable(corrs, 2);
        tDepIds = fillRowInAdditionalTable(depids, 3);
    }

    tModel->insertRow(0, tNames);
    tModel->insertRow(1, tCodes);
    tModel->insertRow(2, tValues);
    tModel->insertRow(3, tDepIds);

    return tModel;
}

QList<QStandardItem*> AdditionCorrelationTable::fillRowInAdditionalTable(QVariantList& elfill, int row)
{
    QList<QStandardItem*> tmpList;
    QStandardItem* item = new QStandardItem(elfill.takeFirst().toString());
    bool isEditable = (row == 1 || row == 2);
    item->setEditable(isEditable);
    tmpList.append(item);
    if (elfill.count() > 0)
    {
        for (int i = 0; i < elfill.count(); ++i)
        {
            QStandardItem* itLoc = new QStandardItem(elfill.at(i).toString());
            itLoc->setEditable(isEditable);
            tmpList.append(itLoc);
        }
    }
    return tmpList;
}

void AdditionCorrelationTable::elementInCorrTableClear(const QModelIndex& elementData)
{
    QStandardItem* item = new QStandardItem;
    bool isEditable = (elementData.row() == 1 || elementData.row() == 2);
    item->setEditable(isEditable);
    tableModel->setItem(elementData.row(), elementData.column(), item);
}
