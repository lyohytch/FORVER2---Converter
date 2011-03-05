#ifndef CONVERTER_H
#define CONVERTER_H
#include <QApplication>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QLabel>
#include <QRadioButton>
#include <QVariantList>

#include "iview.h"

namespace Ui
{
    class converter;
}

class converter : public IView
{
        Q_OBJECT

    public:
        converter(QWidget* parent = 0, QApplication *app = 0);
        ~converter();

        virtual void removeWidgets();
        virtual void addWidgets();
        virtual void updateTextLabel(const QString &txtMsg);
        virtual void englishLanguageSet(bool check);
        virtual void russianLanguageSet(bool check);

    protected:
        void changeEvent(QEvent* e);

    private:
        Ui::converter* ui;
        QHBoxLayout* layout;
        QLabel *pLabel;


        //Init functions
        void init(QApplication *app);
        void init_setup_main_form();
        void init_setup_desktop_widgets();

    private slots:
        //void on_actionChange_DB_structure_triggered();
        void on_actionRussian_triggered(bool checked);
        void on_actionEnglish_triggered(bool checked);
        void on_actionLoad_correlation_model_triggered();
        void on_actionSave_correlation_model_triggered();
        void on_actionExport_all_triggered();
        void on_actionConvert_files_triggered();
        void on_actionExport_template_data_to_DB_triggered();
        void on_actionLoad_Target_Data_triggered();
        void on_actionLoad_Template_Data_triggered();
        void on_actionOpen_template_triggered();
        void on_actionOpen_triggered();
    signals:
        void loadDataComplete();

};

#endif // CONVERTER_H
