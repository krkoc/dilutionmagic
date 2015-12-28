#include "calculator.h"
#include <QDebug>
#include <QtWidgets>



void Calculator::setCalculationParameters(QList<QString> wells, QList<double> targetConc, QList<double> targetVol, double stockConc, QString inUnit, QString outUnit)
{
    qDebug()<<"setcalculatorparametrs initiated";
    this->targetConcentrations=targetConc;
    this->targetVolumes=targetVol;
    this->stockConcentration=stockConc;
    calculateQuantities(wells,targetConcentrations,targetVolumes,stockConcentration,inUnit,outUnit);
}

bool Calculator::allItemsEqual(QStringList list)
{
    bool result=true;
    foreach (QString string, list){
        if (string != list.first()){
            result=false;
            break;
        }
    }
    return result;
}


QList <double> Calculator::calculateQuantities(QList<QString> wells,QList<double> conc, QList<double> volumes, double stockC, QString inUnit, QString outUnit)
{
    qDebug()<<"calculatequantities";

    if (conc.at(0)>stockC ){
        QString str;
        QMessageBox::warning(this, tr("Ooops!"),
                      "Your sample concentration ("+ QString::number(conc.at(0)) + ") is \n greater than stock concentration(" +QString::number(stockC) + ")!" );
    }
    Converter converter;
    //first calculate the quantity
    intermSolVolumes.clear();
    bufferVolumes.clear();
    qDebug()<<"calculateQuantitiesInitiated";
    QString result;
    double Q=0;
    double V;
    qDebug()<<"conc length"<<conc.length();
    for (int i=0; i< conc.length(); i++)
    {
        //calculate the quantity (mass or moles)
        double concC=converter.convert(conc.at(i),inUnit,inUnit.left(2) + "/"+ outUnit);
        Q+=concC*volumes.at(i);
    }
    result.append(tr("Total sample consumption for this dilution is: "));
    result.append(QString::number(Q));
    result.append(inUnit.left(2));
    result.append("\n");

    V=Q/stockC; //volume in units with stock quantity and sample volume
    result.append("You will need  ");
    result.append(QString::number(V)+outUnit);

    result.append(" of stock solution.\n");
    double iv;
    iv=(Q/converter.convert(conc.at(0),inUnit,inUnit.left(2) + "/"+ outUnit));

    intermSolVolumes.append(iv);
    //add iv-V buffer to get to iv.
    result.append("Add ");
    result.append(QString::number(iv-V)+outUnit);
    result.append(" of buffer to get to the total volume of " +QString::number(iv)+outUnit+  " for all dilutions. ");
    result.append("\n");
    result.append("===================================================");
    result.append("\n");


    for (int i=1; i <conc.length();i++)
    {

        iv=( converter.convert(targetConcentrations.at(i-1),inUnit,inUnit.left(2) + "/"+ outUnit)*(intermSolVolumes.last()-targetVolumes.at(i-1))/converter.convert(targetConcentrations.at(i),inUnit,inUnit.left(2) + "/"+ outUnit));
        if (targetConcentrations.at(i)==0) iv=0;
        intermSolVolumes.append(iv);
        // intermSolVolumes.append( targetConcentrations.at(i-1)*(intermSolVolumes.last()-targetVolumes.at(i-1))/targetConcentrations.at(i));

    }
    for (int i=0; i < conc.length()-1;i++)
    {
        result.append(QString::number(i+1)+".) ");
        result.append("Pipette" + QString::number(targetVolumes.at(i))+ outUnit+" of liquid to well "+wells.at(i)+ ". ");
        result.append("Add "+QString::number(intermSolVolumes.at(i+1)-(intermSolVolumes.at(i)-targetVolumes.at(i))) + " to the remaining " + QString::number(intermSolVolumes.at(i)-targetVolumes.at(i))+outUnit+".\n")  ;
        result.append("You now have "+ QString::number(intermSolVolumes.at(i+1))+ outUnit + " of liquid for the rest of the dilutions. Shake rattle and roll! \n");
          result.append("\n");
        if (i == conc.length()-2 ){
            result.append(QString::number(i+2)+".)");
               if ( intermSolVolumes.at(i+1) > 0 ) result.append("Pipette " + QString::number(targetVolumes.at(i+1))+ outUnit + " to well "+ wells.at(i+1));
               if ( intermSolVolumes.at(i+1) == 0 ) result.append("Pipette " + QString::number(targetVolumes.at(i+1))+ outUnit + "of buffer to well "+ wells.at(i+1));
        }

    }
    qDebug()<<result;
    qDebug()<<volumes;
    qDebug()<<conc;

    emit sendCalculationResults(result);
    return conc;
}


