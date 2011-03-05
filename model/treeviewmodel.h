#ifndef TREEVIEWMODEL_H
#define TREEVIEWMODEL_H
#include <QTreeView>
#include <QStandardItemModel>
#include <QStringList>
#include "qmodeldescribing.h"

//! TreeViewModel - класс для визуального представления QStandardItemModel на форме
/**
  * Класс используется для отображения объектов QModelDescribing на форме приложения
  */

class TreeViewModel: public QTreeView
{
        Q_OBJECT

    public:
        /**
          * Конструктор по умолчанию
          * \param parent указатель на родительский объект QWidget
          * \param isT 1 - отображать как приёмник, 0 - отображать как источник
          */
        TreeViewModel(QWidget* parent = 0, bool isT = false);
        ~TreeViewModel() {};
        /**
          * Загружает модель. Модель уже должна быть сформирована из файлов описания
          * \param model указатель на QStandardItemModel, который будет моделью для TreeViewModel
          */
        void loadModel(QStandardItemModel* model);
    private:
        bool isTemplate;

};

#endif // TREEVIEWMODEL_H
