#ifndef PAINTEDRECT_H
#define PAINTEDRECT_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include "model.h"

class PaintedRect : public QGraphicsItem
{
public:

    explicit PaintedRect();
    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    void highlightRect();
    ~PaintedRect();
    QVariant wellColumnValue, wellSolutionName;
    static double rectHeight;

signals:

public slots:


private:
QVariant bla;

};

#endif // PAINTEDRECT_H
