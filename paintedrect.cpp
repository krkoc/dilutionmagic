#include <QDebug>
#include <QRadialGradient>
#include "paintedrect.h"


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


    QRadialGradient gradient(QPoint(20,20),50);

    if (this->isSelected()){
       pen.setColor(QColor(255,88,9));
       painter->setPen(pen);
       painter->drawRoundedRect(1, 1, rectHeight, rectHeight, 5, 5);
       //painter->fillRect(30,rectHeight+1,5,-wellColumnValue.toDouble(),QBrush(Qt::blue, Qt::SolidPattern));
       gradient.setColorAt(0.8,Qt::white);
       gradient.setColorAt(0.0,QColor(255-2.5*wellColumnValue.toDouble(),255,255));
        painter->fillRect(QRect(1, 1, rectHeight, rectHeight),gradient  );
         painter->drawRoundedRect(1, 1, rectHeight, rectHeight, 5, 5);
     }
     //for all others do
       gradient.setColorAt(0.8,Qt::white);
       gradient.setColorAt(0.0,QColor(255-2.5*wellColumnValue.toDouble(),255,255));
       painter->fillRect(QRect(1, 1, rectHeight, rectHeight),gradient  );
       pen.setColor(QColor(20,20,20));
       QFont font;
       font.setBold(true);
       painter->setFont(font);
       painter->drawText(3,15,wellSolutionName.toString().mid(0,5));
       painter->drawText(3,25,wellSolutionName.toString().mid(5,5));
       //painter->drawText(3,35,wellColumnValue.toString());
       pen.setColor(QColor(0,162,230));
        painter->drawRoundedRect(1, 1, rectHeight, rectHeight, 5, 5);
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

