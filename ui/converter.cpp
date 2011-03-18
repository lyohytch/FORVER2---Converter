#include <QDesktopWidget>
#include <QTreeView>
#include <QTableView>

#include "converter.h"
#include "ui_converter.h"
#include "constants.h"
#include "constants_presenter.h"

#include "presenterfornonfunctionui.h"

converter::converter(QWidget* parent, QApplication* app) :
    IView(parent),
    ui(new Ui::converter)
{
    pLabel = NULL;
    layout = NULL;
    init(app);
}

converter::~converter()
{

    emit FreeObjects();

    //Delete layout
    if (layout) delete layout;

    if (ui) delete ui;
}

void converter::changeEvent(QEvent* e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void converter::on_actionOpen_triggered()
{
    //TODO: fix problem when download
    //Target file
    emit OnOpenTargetFiles();
    //Получить данные из модели
}

void converter::removeWidgets()
{
    //template
    layout->removeWidget((QTreeView*)trees[TEMPLATEDESC]);
    //table - corrModel
    layout->removeWidget((QTableView*)corrModel);
    //target
    layout->removeWidget((QTreeView*)trees[TARGETDESC]);

}

void converter::addWidgets()
{
    //template
    layout->addWidget((QTreeView*)trees[TEMPLATEDESC]);
    //table - corrModel
    layout->addWidget((QTableView*)corrModel);
    //target
    layout->addWidget((QTreeView*)trees[TARGETDESC]);

}

void converter::updateTextLabel(const QString& txtMsg)
{
    if (!pLabel)
    {
        pLabel = new QLabel;
    }
    pLabel->setText(txtMsg);
}

void converter::russianLanguageSet(bool check)
{
    QAction* action = this->findChild<QAction*>(actionRussian);
    action->setChecked(check);
}

void converter::englishLanguageSet(bool check)
{
    QAction* action = this->findChild<QAction*>(actionEnglish);
    action->setChecked(check);
}

void converter::on_actionOpen_template_triggered()
{
    //TODO: fix problem when download
    emit OnOpenTemplateFiles();
}

void converter::on_actionLoad_Template_Data_triggered()
{
    emit OnLoadTemplateData();
}

void converter::on_actionLoad_Target_Data_triggered()
{
    emit OnLoadTargetData();
}

void converter::on_actionExport_template_data_to_DB_triggered()
{
    emit OnExportTemplateDataToDB();
}

void converter::on_actionConvert_files_triggered()
{
    emit OnExportTargetDataToDB();
}

void converter::on_actionExport_all_triggered()
{
    qDebug() << " Start exporting...";
    on_actionExport_template_data_to_DB_triggered();
    on_actionConvert_files_triggered();
    qDebug() << " End exporting";
}


void converter::init(QApplication* app)
{
    //Template
    qDebug();

    //new PresenterForFunctionUI(this);
    new PresenterForNonFunctionUI(this, app);

    ui->setupUi(this);

    emit CreateObjects();

    init_setup_main_form();
    init_setup_desktop_widgets();
}

void converter::init_setup_main_form()
{
    //Setup Main Form position
    QDesktopWidget* d = QApplication::desktop();
    this->setGeometry(100, 100, d->width() / 3, d->height() - 400);

    //Set Language checkbox
    QSettings settings(applicationIni, QSettings::IniFormat);
    QVariant engSetted = settings.value(English);
    QVariant rusSetted = settings.value(Russian);
    bool isSettingsNone = engSetted.isNull() || rusSetted.isNull();
    if (!isSettingsNone && (!engSetted.toBool() && rusSetted.toBool()))
    {
        russianLanguageSet(true);
        englishLanguageSet(false);
    }
}



void converter::init_setup_desktop_widgets()
{
    //Put tree model on widget
    layout = new QHBoxLayout;
    this->centralWidget()->setLayout(layout);
    if (!pLabel)
    {
        pLabel = new QLabel;
    }
    updateTextLabel(tr("Application started. Please load target and template files to start converting"));
    this->statusBar()->addWidget(pLabel);
}


void converter::on_actionSave_correlation_model_triggered()
{
    emit OnSaveCorrelationTable();
}

void converter::on_actionLoad_correlation_model_triggered()
{
    emit OnLoadCorrelationTable();
}

void converter::on_actionEnglish_triggered(bool checked)
{
    emit OnEnglishChecked(checked);
}

void converter::on_actionRussian_triggered(bool checked)
{
    emit OnRussianChecked(checked);
}
