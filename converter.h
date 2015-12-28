#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>
#include <QMap>

class Converter : public QObject
{
    Q_OBJECT
public:
    explicit Converter(QObject *parent = 0);
    ~Converter();
    double convert(double quantity, QString inputUnit,QString outputUnit);
private:
     QMap <QString, double> unitMassMap;
     QMap <QString, double> unitVolMap;
      QMap <QString, double> unitMolMap;


signals:

public slots:
};

#endif // CONVERTER_H
