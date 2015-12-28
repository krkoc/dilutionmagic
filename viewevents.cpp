#include "viewevents.h"

Viewevents::Viewevents(QWidget *parent) : QGraphicsView(parent=0)
{

}

void Viewevents::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseReleased();
    QGraphicsView::mouseReleaseEvent(event);
}

Viewevents::~Viewevents()
{

}



