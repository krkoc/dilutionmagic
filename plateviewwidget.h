#ifndef  PLATEVIEWWIDGET_H
#define PLATEVIEWWIDGET_H

#include <QWidget>
#include <QtCore>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QList>
#include "viewevents.h"
#include "paintedrect.h"
#include <QStandardItemModel>
#include "modqsortproxymodel.h"


class PlateView : public QWidget
{
    Q_OBJECT

public:
    explicit PlateView(ModQSortProxyModel *proxy, QWidget *parent = 0);
    ~PlateView();
    QGraphicsScene *scene;
    Viewevents * graphicsView;
    PaintedRect * wellItem;
    QList <PaintedRect*> plateList;
    QStringList stringList;

    QVBoxLayout *layout;
    QTableView *table;
    ModQSortProxyModel *proxyModel;


signals:
void searchString(QString);

public slots:

void getSearchString();
void updateColors();
private:
    int rows, cols;

};

#endif // PLATEVIEW
