#ifndef CONVERTER_H
#define CONVERTER_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QLabel>
#include <QRadioButton>

#include "qmodeldescribing.h"
#include "treeviewmodel.h"
#include "correlationmodel.h"
#include "querymodel.h"
#include "mssqlquery.h"

namespace Ui {
    class converter;
}

//Класс для выбора функции
class QWidgetFunction : public QWidget
{
    Q_OBJECT
public:
    QWidgetFunction();
    ~QWidgetFunction() {};
protected:
    virtual void closeEvent(QCloseEvent */*clEvent*/);
signals:
    void FuncWasChecked(int id);
};

class converter : public QMainWindow {
    Q_OBJECT
public:
    converter(QWidget *parent = 0);
    ~converter();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::converter *ui;
    QHBoxLayout *layout;

    //Function
    QVBoxLayout *fLayout;
    QWidgetFunction *funcWidget;
    QRadioButton *a1;
    QRadioButton *a2;
    QRadioButton *a3;
    //Target
    QModelDescribingOld4 *model4;
    TreeViewModel *tree4;
    //Template
    QModelDescribingDemo *modelD;
    TreeViewModel *treed;
    //Template proc
    QModelDescribingPros *modelP;

    //Describing template
    QModelDescribing **models;
    TreeViewModel **trees;

    TreeViewModel *treep;

    //General Correlation Model
    CorrelationModel *corrModel;

    //QueryModel QModelDescring->StringList
    querymodel *queryModel;
    mssqlquery *mssqlQuery;

    //querymodel *queryModel4;
    //mssqlquery *mssqlQuery4;

    bool isTargetRow;
    bool isTemplateRow;

    QLabel *pLabel;

    QVariantList rowTargetList;
    QVariantList rowTemplateList;
    QVariantList rowFunctionList;



    bool applyTreeClick(int id);
    void setApplyTreeClick(int id);
    void init_load(QModelDescribing *loadedModel, TreeViewModel *tree);
    void init();
    int rowId;//   
    bool countTV;//  

private slots:
    void on_actionChange_DB_structure_triggered();
    void on_actionLoad_correlation_model_triggered();
    void on_actionSave_correlation_model_triggered();
    void on_actionExport_all_triggered();
    void on_actionConvert_files_triggered();
    void on_actionExport_template_data_to_DB_triggered();
    void on_actionLoad_Target_Data_triggered();
    void on_actionLoad_Template_Data_triggered();
    void on_actionOpen_template_triggered();
    void on_actionOpen_triggered();
    void ElementTreeTargetActivated(const QModelIndex &index);
    void ElementTreeTemplateActivated(const QModelIndex &index);
    void ElementTableActivated(const QModelIndex &index);
    void FunctionIsChecked(int id);
    void FillTable();
    void completeAddingToDB(int aError, QString errStr);
    void makeRequestSlot();
signals:
    void loadDataComplete();
    void loadDescComplete();
};

#endif // CONVERTER_H
