#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"
#include <QSortFilterProxyModel>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_findButton_clicked();

    void showPage(int page);

    void on_spinBox_valueChanged(int page);

    void on_action_about_triggered();

    void on_sortButton_clicked();


    void on_loadButton_clicked();

    void on_actionSave_triggered();

    void on_clearButton_clicked();

    void on_deleteRowButton_clicked();



    void on_deleteIdx_valueChanged(int arg1);

    void on_actionAll_triggered();

    void on_showAllButton_clicked();

private:
    Ui::MainWindow *ui;
    model *anime;
    model *searchResults;


};
#endif // MAINWINDOW_H
