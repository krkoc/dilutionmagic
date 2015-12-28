#include "paintedrect.h"
#include <QDebug>


double PaintedRect::rectHeight=40;

PaintedRect::PaintedRect()
{
}

QRectF PaintedRect::boundingRect() const
{
 return QRectF(0,0,rectHeight,rectHeight);
}

void PaintedRect::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    QPen pen;
    pen.setColor(QColor(0,162,230));
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawRoundedRect(1, 1, rectHeight, rectHeight, 5, 5);
    painter->drawText(6,15,wellSolutionName.toString().mid(0,7));
    painter->fillRect(10,rectHeight+1,5,-wellColumnValue.toDouble(),Qt::SolidPattern);

    if (this->isSelected())
       pen.setColor(QColor(255,88,9));
       painter->setPen(pen);
       painter->drawRoundedRect(1, 1, rectHeight, rectHeight, 5, 5);
       painter->fillRect(10,rectHeight+1,5,-wellColumnValue.toDouble(),Qt::SolidPattern);
}

void PaintedRect::highlightRect()
{
    QPainter painter;
    QStyleOptionGraphicsItem kurac;
    paint(&painter,&kurac);

}



PaintedRect::~PaintedRect()
{

}

