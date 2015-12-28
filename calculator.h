#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>

#include "converter.h"

class Calculator : public QWidget
{
Q_OBJECT
public:
    QList <double> bufferVolumes; //this is returned
    QList <double> intermSolVolumes;
    double stockConcentration;
    void setCalculationParameters(QList<QString> wells,QList <double> targetConc, QList <double> targetVolumes, double stockConcentration,QString inUnit, QString outUnit);
    bool allItemsEqual(QStringList list);
private:

    QList <double> calculateQuantities(QList<QString> wells,QList <double> conc, QList <double> volumes, double stockC, QString inUnit, QString outUnit);
    QList <double> targetConcentrations;
    QList <double> targetVolumes;

signals:
    void sendCalculationResults(QString);


};

#endif // CALCULATOR_H
