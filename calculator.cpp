#include "calculator.h"
#include <QDebug>
#include <QtWidgets>


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


QList <double> Calculator::calculateQuantities(QList<QString> wells,QList<double> conc, QList<double> volumes, double stockC, QString inUnit, QString outUnit, QString volumeUnit, double molarMass)
{

    //first calculate that all fields are valid and have common:
    //dilution name,sample name buffer name , stock concentration unit?, output concentration unit?

    this->targetConcentrations=conc;
    this->targetVolumes=volumes;
    this->stockConcentration=stockC;
    Converter::molarMass=molarMass;
    Converter converter;

    qDebug()<<"molarMass"<<Converter::molarMass;
    QString commonOutputUnit=converter.getCommonUnit(outUnit); //will return "g/L" or "M" depending on unit type
    QString commonInUnit=converter.getCommonUnit(inUnit); //will return "g/L" or "M" depending on unit type

//    if (!dilutionParametersCompatible()){
//        QMessageBox::warning(this, tr("Ooops!"),
//                      "Your sample concentration ("+ QString::number(conc.at(0)) + ") is \n greater than stock concentration(" +QString::number(stockC) + ")!" );
//    }
    qDebug()<<"calculatequantities";
    if (conc.at(0) > stockC ){
        QMessageBox::warning(this, tr("Ooops!"),
                      "Your sample concentration ("+ QString::number(conc.at(0)) + ") is \n greater than stock concentration(" +QString::number(stockC) + ")!" );
    }


    //first calculate the quantity
    intermSolVolumes.clear();
    bufferVolumes.clear();
    QString result;
    double Q=0; //total quantity
    double V;
    for (int i=0; i< conc.length(); i++)
    {
        //calculate the quantity to the common unit (mass or moles)
        double concC=converter.convert(conc.at(i),outUnit,commonOutputUnit);            // conc in common units
        //to obtain the quantity we multiply with user volumes in liters
        Q+=concC*converter.convertUserVolumeUnitsToLiter(volumes.at(i),volumeUnit);     //
    }
    //total Q is now in gramms or moles depending on unit type: "masspervol" or "molar"
    //we must convert it to the quantity chosen by the combo "output unit"
    //eg. if "ng/ml" is chosen it must be "ng", of "nM" is chosen it must be "nM"
    //...
    //so first find the mass unit
    QString userQuantityUnit=converter.getQuantityUnit(outUnit); //this will return "ng" or "nM" for instance
    qDebug()<<"userQuantityUnit"<<userQuantityUnit;
    //now we convert the value to that mass or molarity units
    double userQuantity=converter.convertQuantityToUserQuantity(Q,userQuantityUnit);
    result.append(tr("Total stock consumption for this dilution is: "));
    //result.append(QString::number(Q));
    //result.append("in common units g or mol\n");
    result.append(QString::number(userQuantity)); //add checking for moles
    result.append(userQuantityUnit+"\n");

    double stockCInCommonUnits = converter.convert(stockC,inUnit,commonOutputUnit); //"g/L" or "moles/L"

    double volInCommonUnits=Q/stockCInCommonUnits; //because Q is in common units
    //result.append("\n vol in common units" + QString::number(volInCommonUnits) );

    //now we convert this volume in liters to our output volume units
    V=converter.convertLiterToUserVolumeUnits(volInCommonUnits,volumeUnit);

    result.append("You will need  ");
    result.append(QString::number(V)+volumeUnit);
    result.append(" of stock solution.\n");
    double ivCommon, iv;
    //---------------------------------------------------
    //intermediate volume at max concentration
    ivCommon=(Q/converter.convert(conc.at(0), outUnit,commonOutputUnit)); //in unit is the stock conc unit, calc it to common because Q is in common
    qDebug()<<"outUnit "<<outUnit;
    qDebug()<<"ivCommon "<<ivCommon;
    qDebug()<<"commonInUnit "<<commonInUnit;
    qDebug()<<"conc in common"<<converter.convert(conc.at(0), inUnit,commonInUnit);
    qDebug()<<"inUnit "<<inUnit;

    //ivCommon is now in Litres..convert it to output units
    iv=converter.convertLiterToUserVolumeUnits(ivCommon,volumeUnit);
    qDebug()<<"iv "<<iv;
    qDebug()<<"volumeUnit "<<volumeUnit;

    intermSolVolumes.append(iv);
    qDebug()<<"iv common "<<ivCommon;
    qDebug()<<"iv  "<<iv;
    qDebug()<<"volumeUnit "<<volumeUnit;
    //add iv-V buffer to get to iv.
    result.append("Add ");


    result.append(QString::number(iv-V)+volumeUnit);
    if (V > iv ){
        QMessageBox::warning(this, tr("Ooops!"),
                      "Your stock concentration is too small. Continue only if you can handle negative volumes ;)" );
    }
    result.append(" of buffer to get to the total volume of " +QString::number(iv)+volumeUnit+  " for all dilutions. ");
    result.append("\n");
    result.append("Now: ");
    result.append("\n");
    for (int i=1; i <conc.length();i++)
    {
        iv=( converter.convert(targetConcentrations.at(i-1),inUnit, outUnit)*(intermSolVolumes.last()-targetVolumes.at(i-1))/converter.convert(targetConcentrations.at(i),inUnit, outUnit));
        if (targetConcentrations.at(i)==0) iv=0;
        intermSolVolumes.append(iv);
        // intermSolVolumes.append( targetConcentrations.at(i-1)*(intermSolVolumes.last()-targetVolumes.at(i-1))/targetConcentrations.at(i));

    }
    for (int i=0; i < conc.length()-1;i++)
    {
        result.append(QString::number(i+1)+".) ");
        result.append("Pipette" + QString::number(targetVolumes.at(i))+ volumeUnit+" of liquid to well "+wells.at(i)+ ". ");
        double buffToAdd=intermSolVolumes.at(i+1)-(intermSolVolumes.at(i)-targetVolumes.at(i));

        result.append("Add "+QString::number(buffToAdd) + volumeUnit+ " of buffer to the remaining " + QString::number(intermSolVolumes.at(i)-targetVolumes.at(i))+volumeUnit+".\n")  ;
        result.append("You now have "+ QString::number(intermSolVolumes.at(i+1))+ volumeUnit + " of liquid for the rest of the dilutions. Shake rattle and roll! \n");
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

