#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "logo.h"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog* ui;
    Logo* logo;
};

#endif // DIALOG_H
