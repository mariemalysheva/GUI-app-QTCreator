#include "logo.h"
#include <QPainter>

Logo::Logo(QWidget *parent) : QWidget(parent)
{

}

void Logo::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QColor color(53, 80, 112);
    painter.setBrush(color);
    painter.drawRect(0, 0, width(), height());
    QColor color2 = Qt::white;
    static const QLineF l1 = {
        QPointF(0,0),
        QPointF(200,200)
    };
    static const QLineF l2 = {
        QPointF(30,0),
        QPointF(0,30)
    };
    painter.setBrush(color2);
    painter.drawLine(l1);
    painter.drawLine(l2);



}
