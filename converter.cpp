#include "converter.h"
#include <QDebug>

Converter::Converter(QObject *parent) : QObject(parent)
{
    unitMassMap["mg"]=10E3;
    unitMassMap["ug"]=1;
    unitMassMap["ng"]=10E-3;
    unitMassMap["pg"]=10E-6;
    unitVolMap["ml"]=10E3;
    unitVolMap["ul"]=1;
    unitVolMap["nl"]=10E-3;
    unitVolMap["pl"]=10E-6;
    unitMolMap["mM"]=10E3;
    unitMolMap["uM"]=1;
    unitMolMap["nM"]=10E-3;
    unitMolMap["pM"]=10E-6;
}

double Converter::convert(double quantity, QString inputUnit, QString outputUnit)
{
    int indexOfSlash;
    double result;
    QString massInUnit, volInUnit, massOutUnit, volOutUnit;
    indexOfSlash= inputUnit.indexOf("/");
    if (indexOfSlash >0){
        massInUnit = inputUnit.mid(0,indexOfSlash);
        volInUnit  = inputUnit.mid(indexOfSlash+1);
    }
    indexOfSlash= outputUnit.indexOf("/");
    if (indexOfSlash >0){
        massOutUnit = outputUnit.mid(0,indexOfSlash);
        volOutUnit  = outputUnit.mid(indexOfSlash+1);
    }
  //  if (indexOfSlash = -1)
//    qDebug()<<massInUnit;
//    qDebug()<<volInUnit;
//    qDebug()<<massOutUnit;
//    qDebug()<<volOutUnit;
//    qDebug()<<unitVolMap[volOutUnit];
//    qDebug()<<unitVolMap[volInUnit];
//    qDebug()<<unitMassMap[massOutUnit];
//    qDebug()<<unitMassMap[massInUnit];
    double y = unitVolMap[volOutUnit]/unitVolMap[volInUnit];
//    qDebug()<<"y"<<y;
    double x = unitMassMap[massOutUnit]/unitMassMap[massInUnit];
//    qDebug()<<"x"<<x;
    result=quantity*(y/x);
//    qDebug()<<result;
    return result;
}




Converter::~Converter()
{

}

