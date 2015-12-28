#ifndef VIEWEVENTS_H
#define VIEWEVENTS_H

#include <QObject>
#include <QGraphicsView>
#include <QTableView>
#include <QVBoxLayout>

class Viewevents : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Viewevents(QWidget * parent =0);
    void mouseReleaseEvent(QMouseEvent * event);
    ~Viewevents();
signals:

void mouseReleased();


};

#endif // VIEWEVENTS_H
