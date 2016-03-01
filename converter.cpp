#include <QDebug>
#include <QtMath>
#include <QMap>
#include "converter.h"

static QMap <QString, double> unitMassMap;
static  QMap <QString, double> unitVolMap;
static QMap <QString, double> unitMolMap;
static QMap <QString, int> unitTypeToConversionMatrixDictionary;
static double conversionFactorMatrix [2][2]; //conversion factors for moving through variables


double Converter::molarMass=1;


Converter::Converter(QObject *parent) : QObject(parent)
{

    unitMassMap["g"]=1;
    unitMassMap["mg"]=1E-3;
    unitMassMap["ug"]=1E-6;
    unitMassMap["ng"]=1E-9;
    unitMassMap["pg"]=1E-12;
    qDebug()<<"and the winner is=============="<<unitMassMap["mg"];
    unitVolMap["L"]=1;
    unitVolMap["ml"]=1E-3;
    unitVolMap["ul"]=1E-6;
    unitVolMap["nl"]=1E-9;
    unitVolMap["pl"]=1E-12;

    unitMolMap["M"]  = 1;
    unitMolMap["mM"]  = 1E-3;
    unitMolMap["uM"] = 1E-6;
    unitMolMap["nM"] = 1E-9;
    unitMolMap["pM"] = 1E-12;

    unitTypeToConversionMatrixDictionary["maspervol"]=0;
    unitTypeToConversionMatrixDictionary["molar"]=1;

    //define conversion matrix:
    conversionFactorMatrix[0][0]=1;            //from x1g/y1L -> x2g/y2L
    conversionFactorMatrix[0][1]=molarMass;    //from x mol/L  - y g/L
    conversionFactorMatrix[1][0]=1/molarMass;  //from  x g/L - y mol/L
    conversionFactorMatrix[1][1]=1;            //from x1g/y1L -> x2g/y2L
}

Converter::Converter(QString inUnits, QString outUnits, QObject *parent)
{
    Converter();

}



QString Converter::getUnitType(QString unit)
{
    int indexOfSlash;
    indexOfSlash= unit.indexOf("/");
    if (indexOfSlash > 0) //we are dealing with mass/volume units at INPUT
    {
        return "masspervol";
    }
    else if (indexOfSlash == -1){
         if (unit.contains("M",Qt::CaseSensitive)){
         return "molar";
         }
    }
}

QString Converter::getCommonUnit(QString unit)
{
    if (getUnitType(unit)== "masspervol")
    {
        return "g/L";
    }
    else if (getUnitType(unit)=="molar")
    {
        return "M";
    }
}


QString Converter::getQuantityUnit(QString unit)
{
    if (getUnitType(unit)== "masspervol")
    {
        int indexOfSlash;
        indexOfSlash = unit.indexOf("/");
        return unit.mid(0,indexOfSlash); //get the part before the slash: "pg", "ng", "ug", "mg"
    }
    else if (getUnitType(unit)=="molar")
    {
         return unit.left(2)+"";
    }
}


double Converter::commonMassperVol(double conc, QString unit, QString direction)
{
    QString massUnit, volUnit;
    double quantity;
    int indexOfSlash;
    indexOfSlash= unit.indexOf("/");        //find the position of the slash
    massUnit = unit.mid(0,indexOfSlash);    //read the mass unit
    volUnit  = unit.mid(indexOfSlash+1);    //read the vol unit
    if (direction =="Forward"){
        quantity=conc * unitMassMap[massUnit]/unitVolMap[volUnit]; //grams (per liter)
        return quantity;
    }
    else if (direction=="Reverse"){
        quantity=conc*qPow(unitMassMap[massUnit]/unitVolMap[volUnit],-1 ); //xg / yl
        return quantity;
    }
}


double Converter::commonMolarity(double conc, QString unit, QString direction)
{
    QString molar;
    QString volUnit;
    double quantity;
    molar=unit.left(2); //returns "M" or "uM" or "uM"...
    volUnit="L";
    if (direction =="Forward"){
        quantity=conc *unitMolMap[molar]/unitVolMap[volUnit]; //negative quantity means missing data
        return quantity;
    }
    else if (direction =="Reverse"){
        quantity=conc * qPow(unitMolMap[molar]/unitVolMap[volUnit],-1); //negative quantity means missing data
        return quantity;
    }
}


double Converter::getConversionFactor(QString unitIn, QString unitOut)
{
    QString commonUnitIn, commonUnitOut;
    int row, column;
    commonUnitIn=getUnitType(unitIn);
    commonUnitOut=getUnitType(unitOut);
    column=unitTypeToConversionMatrixDictionary[commonUnitIn];
    row=unitTypeToConversionMatrixDictionary[commonUnitOut];
    return conversionFactorMatrix[column][row];
}


double Converter::convert(double conc, QString inUnit, QString outUnit)
{
    QString commonIn, commonOut;
    double conversionFactor;
    double commonConc;
    //determine commonUnitTypes
    commonIn=getUnitType(inUnit);
    commonOut=getUnitType(outUnit);
    qDebug()<<"commonIn"<<commonIn;
    qDebug()<<"commonOut"<<commonOut;
    if (commonIn=="masspervol"){
        commonConc=commonMassperVol(conc,  inUnit, "Forward"); //conver to g/L if "masspervol"
        qDebug()<<"commonConcMasspervol"<<commonConc;
    }
    else if (commonIn=="molar"){
        commonConc=commonMolarity(conc,  inUnit, "Forward"); //convert to mol/L if "molar"
        qDebug()<<"commonConcMolarity"<<commonConc;
    }
    //get the conversionFactor from the conversionFactorMatrix
    conversionFactor=getConversionFactor(inUnit, outUnit);
    qDebug()<<"conversionFactor"<<conversionFactor;
    //convert to the out unit common unit
    if (commonOut=="masspervol"){
        return (conversionFactor) * commonMassperVol(commonConc,  outUnit, "Reverse"); //conver from g/L if "masspervol"
        qDebug()<<"commonConcMasspervol"<<commonMassperVol(commonConc,  outUnit, "Reverse");
    }
    else if (commonOut=="molar"){
        return (conversionFactor) *commonMolarity(commonConc,  outUnit, "Reverse"); //convert from mol/L if "molar"
        qDebug()<<"commonMolar"<<commonMassperVol(commonConc,  outUnit, "Reverse");
    }


}

double Converter::convertUserVolumeUnitsToLiter(double volume, QString unit)
{
    return volume * unitVolMap[unit];
}


double Converter::convertLiterToUserVolumeUnits(double volume, QString unit)
{
    return volume / unitVolMap[unit];
}


double Converter::convertUserUnitsToGramms(double mass, QString unit)
{
      return mass * unitMassMap[unit];
}

double Converter::convertGrammsToUserUnits(double mass, QString unit)
{
      return mass / unitMassMap[unit];
}

double Converter::convertUserMolarityToMol(double molarity, QString unit)
{
        return molarity * unitMolMap[unit];
}


double Converter::convertMolToUserMolarity(double molarity, QString unit)
{
        return molarity / unitMolMap[unit];
}

double Converter::convertQuantityToUserQuantity(double quantity, QString unit)
{
    if (unit.contains("g",Qt::CaseSensitive))
    {
        return convertGrammsToUserUnits(quantity,unit);
    }
    else if (unit.contains("M",Qt::CaseSensitive))
    {
        return convertMolToUserMolarity(quantity,unit);
    }
}


double Converter::convertUserQuantityToQuantity(double quantity, QString unit)
{
    if (unit.contains("g",Qt::CaseSensitive))
    {
        return convertUserUnitsToGramms(quantity,unit);
    }
    else if (unit.contains("M",Qt::CaseSensitive))
    {
        return convertUserMolarityToMol(quantity,unit);
    }
}


Converter::~Converter()
{

}

