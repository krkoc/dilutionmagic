#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>
#include <QMap>

class Converter : public QObject
{
    Q_OBJECT

public:
    explicit Converter(QObject *parent = 0);
    explicit Converter(QString inUnits, QString outUnits, QObject *parent = 0);
    ~Converter();
    QString  getUnitType(QString units);
    double   commonMassperVol(double conc, QString unit, QString direction="Forward");
    double   commonMolarity(double conc, QString unit, QString direction="Forward");
    double   getConversionFactor(QString unitIn, QString unitOut);
    double   convert(double conc, QString inUnit, QString outUnit);
    double   convertUserVolumeUnitsToLiter(double volume, QString unit);
    double   convertLiterToUserVolumeUnits(double volume, QString unit);
    double   convertUserUnitsToGramms(double mass, QString unit);
    double   convertGrammsToUserUnits(double mass, QString unit);
    double   convertMolToUserMolarity(double molarity, QString unit);
    double   convertUserMolarityToMol(double molarity, QString unit);
    double   convertQuantityToUserQuantity(double quantity, QString unit);
    double   convertUserQuantityToQuantity(double quantity, QString unit);
    QString  getQuantityUnit(QString unit);
    QString  getCommonUnit(QString unit);

    double molarMass=-2;

private:
     QMap <QString, double> unitMassMap;
     QMap <QString, double> unitVolMap;
     QMap <QString, double> unitMolMap;
     QMap <QString, int> unitTypeToConversionMatrixDictionary;
     double conversionFactorMatrix [2][2]; //conversion factors for moving through variables


signals:

public slots:
};

#endif // CONVERTER_H
