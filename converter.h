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

    static QString  getUnitType(QString units);
    static double   commonMassperVol(double conc, QString unit, QString direction="Forward");
    static double   commonMolarity(double conc, QString unit, QString direction="Forward");
    static double   getConversionFactor(QString unitIn, QString unitOut);
    static double   convert(double conc, QString inUnit, QString outUnit);
    static double   convertUserVolumeUnitsToLiter(double volume, QString unit);
    static double   convertLiterToUserVolumeUnits(double volume, QString unit);
    static double   convertUserUnitsToGramms(double mass, QString unit);
    static double   convertGrammsToUserUnits(double mass, QString unit);
    static double   convertMolToUserMolarity(double molarity, QString unit);
    static double   convertUserMolarityToMol(double molarity, QString unit);
    static double   convertQuantityToUserQuantity(double quantity, QString unit);
    static double   convertUserQuantityToQuantity(double quantity, QString unit);

    static QString  getQuantityUnit(QString unit);
    static QString  getCommonUnit(QString unit);
    static double molarMass;



signals:

public slots:
};

#endif // CONVERTER_H
