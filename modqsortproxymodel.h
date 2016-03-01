#ifndef MODQSORTPROXYMODEL_H
#define MODQSORTPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QMap>

class ModQSortProxyModel : public QSortFilterProxyModel
{
Q_OBJECT
public:

//    headers<<"Well"<<"Dilution_name"<<"Sample"<<"Buffer"<<"Concentration"<<"Concunits"<<"Stock_volume"<<"Well_volume"<<"Volume_units"<<"Stock_conc";

    enum Fields {WELL_NAME, DILUTION, SAMPLE, BUFFER, QUANTITY, CONC_UNIT, STOCK_VOLUME,BUFFER_VOLUME, VOLUME, VOLUME_UNIT, STOCK_CONC };
    Fields fields;
    ModQSortProxyModel(QObject *parent=0);
    QString outputUnit;
    double molarMass;
    int i;
    ~ModQSortProxyModel();
    bool	setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);


signals:
    void modelModified();


};

#endif // MODQSORTPROXYMODEL_H
