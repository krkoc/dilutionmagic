#include "plateviewwidget.h"
#include <qDebug>

PlateView::PlateView(ModQSortProxyModel *proxy, QWidget *parent) :
    QWidget(parent)
{
    rows=8;
    cols=12;
    proxyModel=proxy;
    stringList<<"A1"<<"A2"<<"A3"<<"A4"<<"A5"<<"A6"<<"A7"<<"A8"<<"A9"<<"A_10"<<"A_11"<<"A_12"
              <<"B1"<<"B2"<<"B3"<<"B4"<<"B5"<<"B6"<<"B7"<<"B8"<<"B9"<<"B_10"<<"B_11"<<"B_12"
              <<"C1"<<"C2"<<"C3"<<"C4"<<"C5"<<"C6"<<"C7"<<"C8"<<"C9"<<"C_10"<<"C_11"<<"C_12"
              <<"D1"<<"D2"<<"D3"<<"D4"<<"D5"<<"D6"<<"D7"<<"D8"<<"D9"<<"D_10"<<"D_11"<<"D_12"
              <<"E1"<<"E2"<<"E3"<<"E4"<<"E5"<<"E6"<<"E7"<<"E8"<<"E9"<<"E_10"<<"E_11"<<"E_12"
              <<"F1"<<"F2"<<"F3"<<"F4"<<"F5"<<"F6"<<"F7"<<"F8"<<"F9"<<"F_10"<<"F_11"<<"F_12"
              <<"G1"<<"G2"<<"G3"<<"G4"<<"G5"<<"G6"<<"G7"<<"G8"<<"G9"<<"G_10"<<"G_11"<<"G_12"
              <<"H1"<<"H2"<<"H3"<<"H4"<<"H5"<<"H6"<<"H7"<<"H8"<<"H9"<<"H_10"<<"H_11"<<"H_12";

    layout = new QVBoxLayout;
    graphicsView = new Viewevents(this);
    graphicsView->setParent(this);
    layout->addWidget(graphicsView);
    table= new QTableView;
    layout->addWidget(table);
    scene = new QGraphicsScene(this);
     graphicsView->setGeometry(QRect(0,0,parent->width(),parent->height()));
    graphicsView->show();
    graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    //graphicsView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    //graphicsView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    graphicsView->setScene(scene);
    scene->setSceneRect(0,0,PaintedRect::rectHeight*20,graphicsView->height());
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setRubberBandSelectionMode(Qt::ContainsItemShape);
    QStringList rowString;
    rowString<<"A"<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"<<"H";
    QFont font ("Tahoma",20, 10);
    for (int i=0; i < rows; i++){
        QGraphicsTextItem * io = new QGraphicsTextItem;
        io->setPos(20, 40+i*(PaintedRect::rectHeight+10));
        io->setFont(font);
        io->setPlainText(rowString.at(i));
        scene->addItem(io);
        for (int j=0;j<cols;j++){
            if(i==0){
                QGraphicsTextItem * io = new QGraphicsTextItem;
             io->setPos(40+j*(PaintedRect::rectHeight+10),20);
             io->setFont(font);
             io->setPlainText(QString::number(j+1));
              scene->addItem(io);
            }
            wellItem = new PaintedRect;
            wellItem->setX(50+j*(PaintedRect::rectHeight+10));
            wellItem->setY(50+i*(PaintedRect::rectHeight+10));
            wellItem->setFlags(QGraphicsItem::ItemIsSelectable);
            scene->addItem(wellItem);
            plateList<<wellItem;
        }
    }
    connect(graphicsView,SIGNAL(mouseReleased()),this,SLOT(getSearchString()));
}

void PlateView::getSearchString()
{
    QString ba;
    int i=0;
    foreach (PaintedRect *wellItem, plateList)
    {
    if (scene->selectedItems().indexOf(wellItem)>-1 )
     {
        wellItem->highlightRect();
        qDebug()<<stringList.at(i);
        ba+=stringList.at(i)+"|";}
        i++;
    }
    ba.remove(ba.size()-1,1);
    qDebug()<<"search string:"<<ba;
    emit searchString(ba);
}



void PlateView::updateColors()
{
  QStandardItem item;
  QStandardItemModel *stdm;
  stdm=reinterpret_cast<QStandardItemModel*>(proxyModel->sourceModel());
  QVariant quantity, solutionName;
  QVariant max=(double)0.01;

  for (int i=0; i<stdm->rowCount();i++){
    quantity=stdm->data(stdm->index(i, proxyModel->QUANTITY),Qt::DisplayRole);
    if (quantity.toDouble() > max.toDouble()) max=quantity;
  }
  for (int i=0; i<stdm->rowCount();i++){
    quantity=stdm->data(stdm->index(i,proxyModel->QUANTITY),Qt::DisplayRole);

    solutionName=stdm->data(stdm->index(i,proxyModel->DILUTION),Qt::DisplayRole);
    plateList.at(i)->wellColumnValue=QVariant(quantity.toDouble()* PaintedRect::rectHeight/max.toDouble());

    plateList.at(i)->wellSolutionName=solutionName;
    plateList.at(i)->update();
  }
  graphicsView->update();
}



PlateView::~PlateView()
{

}
