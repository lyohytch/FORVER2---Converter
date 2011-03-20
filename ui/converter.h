#ifndef CONVERTER_H
#define CONVERTER_H

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>

#include "iview.h"

namespace Ui
{
    class converter;
}
//! converter интерфейс, реализующий интерфейс IView
/**
  * Класс, который реализует интерфейс IView
  */
class converter : public IView
{
        Q_OBJECT

    public:
        /**
          * Конструктор по-умолчанию. Инициализирует основную форму, меню и другие виджеты на форме
          * \param parent указатель на родительский виджет
          * \param app указатель на QApplication. Используется для перевода приложения
          */
        converter(QWidget* parent = 0, QApplication *app = 0);
        ~converter();
        /**
          * Удалить какие-нибудь виджеты
          */
        virtual void removeWidgets();
        /**
          * Добавить какие-нибудь виджеты
          */
        virtual void addWidgets();
        /**
          * Обновить сообщение на форме
          * \param txtMsg текстовое сообщение
          */
        virtual void updateTextLabel(const QString &txtMsg);
        /**
          * Установить или нет английский язык
          * \param check true - установить, false - иначе
          */
        virtual void englishLanguageSet(bool check);
        /**
          * Установить или нет русский язык
          * \param check true - установить, false - иначе
          */
        virtual void russianLanguageSet(bool check);

    protected:
        /**
          * Обработка сигнала QEvent
          * \param e сигнал QEvent
          * \note Данная функция не используется
          */
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
};

#endif // CONVERTER_H
