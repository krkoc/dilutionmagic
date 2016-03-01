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
    bool allItemsEqual(QStringList list);
    QList <double> calculateQuantities(QList<QString> wells,QList<double> concInUnits, QList<double> volumes, double stockCIn, QString inUnit, QString outUnit, QString volumeUnit, double molarMass);

private:

    QList <double> targetConcentrations;
    QList <double> targetVolumes;

signals:
    void sendCalculationResults(QString);


};

#endif // CALCULATOR_H
