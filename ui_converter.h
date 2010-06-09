/********************************************************************************
** Form generated from reading UI file 'converter.ui'
**
** Created: Sun 6. Jun 12:29:35 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONVERTER_H
#define UI_CONVERTER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_converter
{
public:
    QAction *actionAbout;
    QAction *actionClose;
    QAction *actionFAQ;
    QAction *actionOpen;
    QAction *actionOpen_template;
    QAction *actionOpen_files_to_convert;
    QAction *actionConvert_files;
    QAction *actionChange_DB_structure;
    QAction *actionLoad_Template_Data;
    QAction *actionLoad_Target_Data;
    QAction *actionExport_template_data_to_DB;
    QAction *actionLoad_Target_Data_from_DB;
    QAction *actionExport_all;
    QAction *actionSave_correlation_model;
    QAction *actionLoad_correlation_model;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuExecute;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *converter)
    {
        if (converter->objectName().isEmpty())
            converter->setObjectName(QString::fromUtf8("converter"));
        converter->setWindowModality(Qt::WindowModal);
        converter->resize(1024, 768);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(converter->sizePolicy().hasHeightForWidth());
        converter->setSizePolicy(sizePolicy);
        converter->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks);
        actionAbout = new QAction(converter);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionClose = new QAction(converter);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionFAQ = new QAction(converter);
        actionFAQ->setObjectName(QString::fromUtf8("actionFAQ"));
        actionOpen = new QAction(converter);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionOpen_template = new QAction(converter);
        actionOpen_template->setObjectName(QString::fromUtf8("actionOpen_template"));
        actionOpen_files_to_convert = new QAction(converter);
        actionOpen_files_to_convert->setObjectName(QString::fromUtf8("actionOpen_files_to_convert"));
        actionConvert_files = new QAction(converter);
        actionConvert_files->setObjectName(QString::fromUtf8("actionConvert_files"));
        actionChange_DB_structure = new QAction(converter);
        actionChange_DB_structure->setObjectName(QString::fromUtf8("actionChange_DB_structure"));
        actionLoad_Template_Data = new QAction(converter);
        actionLoad_Template_Data->setObjectName(QString::fromUtf8("actionLoad_Template_Data"));
        actionLoad_Target_Data = new QAction(converter);
        actionLoad_Target_Data->setObjectName(QString::fromUtf8("actionLoad_Target_Data"));
        actionExport_template_data_to_DB = new QAction(converter);
        actionExport_template_data_to_DB->setObjectName(QString::fromUtf8("actionExport_template_data_to_DB"));
        actionLoad_Target_Data_from_DB = new QAction(converter);
        actionLoad_Target_Data_from_DB->setObjectName(QString::fromUtf8("actionLoad_Target_Data_from_DB"));
        actionExport_all = new QAction(converter);
        actionExport_all->setObjectName(QString::fromUtf8("actionExport_all"));
        actionSave_correlation_model = new QAction(converter);
        actionSave_correlation_model->setObjectName(QString::fromUtf8("actionSave_correlation_model"));
        actionLoad_correlation_model = new QAction(converter);
        actionLoad_correlation_model->setObjectName(QString::fromUtf8("actionLoad_correlation_model"));
        centralWidget = new QWidget(converter);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        converter->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(converter);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuExecute = new QMenu(menuBar);
        menuExecute->setObjectName(QString::fromUtf8("menuExecute"));
        converter->setMenuBar(menuBar);
        mainToolBar = new QToolBar(converter);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        converter->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(converter);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        converter->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuExecute->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen_template);
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionClose);
        menuHelp->addAction(actionFAQ);
        menuHelp->addAction(actionAbout);
        menuExecute->addAction(actionLoad_Target_Data_from_DB);
        menuExecute->addAction(actionLoad_Target_Data);
        menuExecute->addAction(actionConvert_files);
        menuExecute->addSeparator();
        menuExecute->addAction(actionLoad_Template_Data);
        menuExecute->addAction(actionExport_template_data_to_DB);
        menuExecute->addSeparator();
        menuExecute->addAction(actionExport_all);
        menuExecute->addAction(actionChange_DB_structure);
        menuExecute->addSeparator();
        menuExecute->addAction(actionSave_correlation_model);
        menuExecute->addAction(actionLoad_correlation_model);

        retranslateUi(converter);
        QObject::connect(actionClose, SIGNAL(triggered()), converter, SLOT(close()));

        QMetaObject::connectSlotsByName(converter);
    } // setupUi

    void retranslateUi(QMainWindow *converter)
    {
        converter->setWindowTitle(QApplication::translate("converter", "converter", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("converter", "About", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("converter", "Close", 0, QApplication::UnicodeUTF8));
        actionClose->setShortcut(QApplication::translate("converter", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionFAQ->setText(QApplication::translate("converter", "FAQ", 0, QApplication::UnicodeUTF8));
        actionFAQ->setShortcut(QApplication::translate("converter", "F11", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("converter", "Open target file...", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("converter", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionOpen_template->setText(QApplication::translate("converter", "Open template...", 0, QApplication::UnicodeUTF8));
        actionOpen_template->setShortcut(QApplication::translate("converter", "Ctrl+Shift+O", 0, QApplication::UnicodeUTF8));
        actionOpen_files_to_convert->setText(QApplication::translate("converter", "Open files to convert...", 0, QApplication::UnicodeUTF8));
        actionConvert_files->setText(QApplication::translate("converter", "Convert and export target data to DB ", 0, QApplication::UnicodeUTF8));
        actionChange_DB_structure->setText(QApplication::translate("converter", "Change DB structure", 0, QApplication::UnicodeUTF8));
        actionLoad_Template_Data->setText(QApplication::translate("converter", "Load Template Data from FILE", 0, QApplication::UnicodeUTF8));
        actionLoad_Target_Data->setText(QApplication::translate("converter", "Load Target Data from FILE", 0, QApplication::UnicodeUTF8));
        actionExport_template_data_to_DB->setText(QApplication::translate("converter", "Export template data to DB", 0, QApplication::UnicodeUTF8));
        actionLoad_Target_Data_from_DB->setText(QApplication::translate("converter", "Load Target Data from DB", 0, QApplication::UnicodeUTF8));
        actionExport_all->setText(QApplication::translate("converter", "Export all to DB", 0, QApplication::UnicodeUTF8));
        actionSave_correlation_model->setText(QApplication::translate("converter", "Save correlation model", 0, QApplication::UnicodeUTF8));
        actionLoad_correlation_model->setText(QApplication::translate("converter", "Load correlation model", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("converter", "File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("converter", "Help", 0, QApplication::UnicodeUTF8));
        menuExecute->setTitle(QApplication::translate("converter", "Execute", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class converter: public Ui_converter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONVERTER_H
