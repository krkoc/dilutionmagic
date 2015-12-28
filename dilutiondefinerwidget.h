#ifndef DILUTIONDEFINERWIDGET_H
#define DILUTIONDEFINERWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include "model.h"
#include  "ModQSortProxyModel.h"

namespace Ui {
class DilutionDefiner;
}



class DilutionDefiner : public QWidget
{
    Q_OBJECT


public:
    explicit DilutionDefiner(ModQSortProxyModel * proxy,QWidget *parent = 0);
    ~DilutionDefiner();
    Ui::DilutionDefiner *ui;
    ModQSortProxyModel *proxyModel;

private slots:
    void slotToEmitQuantities();


signals:
    void calculateQuantitiesParams(byte  mode, double minVal, double maxVal); //0 linear, 1 log
    void sendSolutionName(QString solName);

private:


};


#endif // DILUTIONDEFINER_H
