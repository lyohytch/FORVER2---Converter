#include <QModelIndex>

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
    // TODO: create factory of objects
    //Multiple case. Old Spravka's files//DEPRECATED
    _view->model4 = new QModelDescribingOld4();

    //One to one case. Demo version // DEPRECATED
    _view->modelD = new QModelDescribingDemo();

    //Cases from prosecutor's office
    _view->modelP = new QModelDescribingPros();


    //New Spravka's files.
    _view->modelJURA = new QModelDescribingFromJura();

    _view->models = new QObject*[2];

    _view->models[TARGETDESC] = _view->modelP;
    _view->models[TEMPLATEDESC] = _view->modelJURA;

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
}
