#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QWidget>
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QString>

#include <QSet>
#include <QFileDialog>
#include <QTableWidget>

#include <iostream>
#include <dialog.h>
#include <QTextStream>
#include <QItemSelectionModel>




void MainWindow::showPage(int page){

    int firstRow = page*10;
    int lastRow = firstRow+10;
    if (lastRow > ui->tableView->model()->rowCount())
        lastRow = ui->tableView->model()->rowCount();
    for(int i=0; i<firstRow;++i){
        ui ->tableView->hideRow(i);
    }
    for(int i=lastRow; i< ui->tableView->model()->rowCount(); ++i){
        ui ->tableView->hideRow(i);
}
    for(int i=firstRow;i<lastRow; ++i){
        ui->tableView->showRow(i);
    }
    for(int i=0; i<7; ++i){
        ui->tableView->resizeColumnToContents(i);
    }
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui-> spinBox->setMaximum((ui->tableView->model()->rowCount()-1)/10);
    ui->deleteIdx->setMaximum(ui->tableView->model()->rowCount());
    ui->tableView->selectRow(ui->deleteIdx->value()-1);





}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    anime = new model(this);
    searchResults = new model(this);
    ui->tableView->setModel(anime);


}



MainWindow::~MainWindow()
{
    delete ui;
    delete anime;
    delete searchResults;

}


void MainWindow::on_findButton_clicked(){

    searchResults ->clear();
    QString searchString = ui->searchBar->text();
    QString genreString = ui->lineEdit->text();

    anime->search(searchString, genreString, searchResults);
    ui->tableView->setModel(searchResults);


    showPage(0);
    ui->spinBox->setValue(0);
    ui->deleteIdx->setValue(0);
    ui->sortButton->setStyleSheet("none");
    ui-> deleteIdx->setMaximum(ui->tableView->model()->rowCount());
}


void MainWindow::on_spinBox_valueChanged(int page){

    if (ui->spinBox->value()<0)
        return;
    showPage(page);
    //ui->deleteIdx->setValue(page*10+1);

}


void MainWindow::on_action_about_triggered()
{
    Dialog about;
    about.exec();
}

void MainWindow::on_sortButton_clicked()
{
    searchResults->modelSorted();
    ui->tableView->setModel(searchResults);

    showPage(ui->spinBox->value());
    if (!searchResults->isEmpty())
        ui->sortButton->setStyleSheet("background-color:green; border-radius:5px; padding:0px; min-width:5em;");


 }



void MainWindow::on_loadButton_clicked()
{
        QString path = QFileDialog::getOpenFileName(this);
        if (path.isNull())
            return;

        anime ->clear();


        ui->tableView->setModel(nullptr);
        anime->fillModelWithData(path);
        searchResults->fillModelWithData(path);
        ui->tableView->setModel(anime);

        ui->sortButton->setStyleSheet("none");

        showPage(0);
        ui->spinBox->setValue(0);
        ui->deleteIdx->setValue(0);
        ui->searchBar->clear();
        ui->lineEdit->clear();

}


void MainWindow::on_actionSave_triggered()
{

    QString path = QFileDialog::getOpenFileName(this);
    if (path.isNull())
        return;

    searchResults->saveModelAsFile(path);
}


void MainWindow::on_clearButton_clicked()
{
    if (anime->isEmpty())
        return;
    if (searchResults->rowCount()==anime->rowCount()){
        anime->clear();
        ui->tableView->setModel(anime);
        showPage(0);
        return;
    }
    int i=0;
    while(i<searchResults->rowCount()){
        anime->deleteFromAll(searchResults, i);
        ++i;
    }
    searchResults->clear();
    ui->sortButton->setStyleSheet("none");

    showPage(0);
    ui->spinBox->setValue(0);
    ui->deleteIdx->setValue(0);
}


void MainWindow::on_deleteRowButton_clicked()
{
    int pos = ui->deleteIdx->value()-1;
    if (pos>=ui->tableView->model()->rowCount() or pos<0)
        return;
    anime->deleteFromAll(searchResults, pos);
    searchResults -> deleteRow(pos);
    ui->tableView->setModel(searchResults);
    showPage(ui->spinBox->value());

    ui->deleteIdx->setMaximum(ui->tableView->model()->rowCount());
    ui->tableView->selectRow(ui->deleteIdx->value()-1);
}

void MainWindow::on_deleteIdx_valueChanged(int num)
{    

    ui-> deleteIdx->setMaximum(ui->tableView->model()->rowCount());

    ui-> spinBox->setValue((num-1)/10);

    ui->tableView->selectRow(num-1);
    if (num == 0)
        ui->tableView->clearSelection();


}


void MainWindow::on_actionAll_triggered()
{
    QString path = QFileDialog::getOpenFileName(this);
    if (path.isNull())
        return;

    anime->saveModelAsFile(path);
}

void MainWindow::on_showAllButton_clicked()
{
    ui->tableView->setModel(anime);
    showPage(0);

    ui->deleteIdx->setMaximum(ui->tableView->model()->rowCount());
    ui->spinBox->setValue(0);
    ui->deleteIdx->setValue(0);
    //ui->spinBox
    ui->sortButton->setStyleSheet("none");
    ui->searchBar->clear();
    ui->lineEdit->clear();

}
