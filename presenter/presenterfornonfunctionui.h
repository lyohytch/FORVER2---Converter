#ifndef PRESENTERFORNONFUNCTIONUI_H
#define PRESENTERFORNONFUNCTIONUI_H

#include "presenter.h"

class PresenterForNonFunctionUI: public Presenters
{
    Q_OBJECT
public:
    PresenterForNonFunctionUI(IView *view) : Presenters(view) {}
protected:
    virtual void allocateCorrelationModel();
    virtual QVariant switchFunction(int /*id*/, const QVariantList& /*parameters*/) { return QVariantList(); }
    virtual void allocateMemory();
    virtual void setModelsAndTreesObjects();
protected slots:
    virtual void freeObjects();
};

#endif // PRESENTERFORNONFUNCTIONUI_H