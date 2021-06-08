#include "dialog.h"
#include "ui_dialog.h"
#include <QBoxLayout>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    logo = new Logo(ui->frame);
    ui->horizontalLayout_2->addWidget(logo);
    setFixedSize(QSize(400, 300));
    ui->frame->setStyleSheet("none");


}

Dialog::~Dialog()
{
    delete ui;
}

