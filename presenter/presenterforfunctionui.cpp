#include <QDesktopWidget>
#include <QApplication>

#include "presenterforfunctionui.h"
#include "constants.h"
#include "constants_presenter.h"
#include "correlationmodelfunction.h"
#include "correlationmodel.h"

void PresenterForFunctionUI::setupFunctionWidget()
{
    //Create window to specify function
    funcWidget = new QWidgetFunction;
    fLayout = new QVBoxLayout;
    QDesktopWidget* d = QApplication::desktop();
    funcWidget->setGeometry(d->width() / 2, d->height() / 3, 300, 100);
    a1 = new QRadioButton("AGE", funcWidget);
    a2 = new QRadioButton("CONCAT", funcWidget);
    a3 = new QRadioButton("NO FUNCTION", funcWidget);
    a1->setObjectName("AGE");
    a2->setObjectName("CONCAT");
    a3->setObjectName("NONE");
    a3->setChecked(true);
    fLayout->addWidget(a1);
    fLayout->addWidget(a2);
    fLayout->addWidget(a3);
    funcWidget->setLayout(fLayout);
    connect(funcWidget, SIGNAL(FuncWasChecked(int)), this, SLOT(FunctionIsChecked(int)), Qt::QueuedConnection);
}

void PresenterForFunctionUI::createObjects()
{
    Presenters::createObjects();
    setupFunctionWidget();
}

void PresenterForFunctionUI::freeObjects()
{
    Presenters::freeObjects();
    //Delete func form
    if (a1) delete a1;
    if (a2) delete a2;
    if (a3) delete a3;
    if (fLayout) delete fLayout;
    if (funcWidget) delete funcWidget;
}

void PresenterForFunctionUI::allocateCorrelationModel()
{
    _view->corrModel = new CorrelationModelFunction((QWidget*)_view,
            MODELS(TEMPLATEDESC),
            MODELS(TARGETDESC));
    connect(CORR_MODEL, SIGNAL(doubleClicked(const QModelIndex&)), this,
            SLOT(ElementTableActivated(const QModelIndex&)), Qt::QueuedConnection);
}

void PresenterForFunctionUI::ElementTableActivated(const QModelIndex& index)
{
    qDebug() << "index = " << index.data(Qt::UserRole + 1);
    //Определить row
    qDebug() << " index.column():=" << index.column();
    qDebug() << " index.row():=" << index.row();
    Presenters::_view->activeRowId = index.row();
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
            }
            break;
        case iFunction:
            {
                CORR_MODEL->setApplyTreeClick(iFunction);
                _view->updateTextLabel(tr("Change function. Row: ") + QVariant(index.row() + 1).toString() + tr(" Column: ") + QVariant(index.column() + 1).toString());
                funcWidget->showNormal();
            }
            break;
        default:
            qWarning() << " Strange column??";
    }
}

void PresenterForFunctionUI::FunctionIsChecked(int functionId)
{
    qDebug() << " change function in table. ID = " << functionId;
    qDebug() << "index.row() = " << _view->activeRowId;
    if (CORR_MODEL->applyTreeClick(iFunction))
    {
        CORR_MODEL->changeFunctionValue(iFunction, _view->activeRowId, functionId);
    }
    else
    {
        qDebug() << " function table cell isn't checked or not applicable";
        _view->updateTextLabel(tr("Function table cell isn't checked or not applicable"));
    }
}

//------------------------------------------------------------------------
//Func form
QWidgetFunction::QWidgetFunction():
    QWidget()
{}

void QWidgetFunction::closeEvent(QCloseEvent */*clEvent*/)
{
    qDebug() << " ::func from closed";
    int id = nonef;
    QRadioButton* r1 = this->findChild<QRadioButton*>(tr("AGE"));
    if (r1 && r1->isChecked())
    {
        //TODO emit signal to converter
        qDebug() << "::AGE";
        id = agef;
    }
    else
    {
        QRadioButton* r2 = this->findChild<QRadioButton*>(tr("CONCAT"));
        if (r2 && r2->isChecked())
        {
            qDebug() << "::CONCAT";
            id = concatf;
        }
        else
        {
            QRadioButton* r3 = this->findChild<QRadioButton*>(tr("NONE"));
            if (r3 && r3->isChecked())
            {
                qDebug() << "::NO FUNCTION";
                id = nonef;
            }
            else
            {
                qWarning() << " Error was encountered";
                id = nonef;
            }
        }
    }
    emit FuncWasChecked(id);
}

