#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QStandardItemModel>
class Model : public QObject
{
    Q_OBJECT

public:
    explicit Model(QObject * parent=0);
    QStandardItemModel *model;
    void setEntry(QStandardItemModel *model, const QString &subject,const QString &sender, const double &conc, int index);
    QStandardItemModel *createMailModel(QObject *parent);
    QStringList headers;
    enum Fields {WELL_NAME, DILUTION, SAMPLE, BUFFER, QUANTITY, CONC_UNIT, STOCK_VOLUME, BUFFER_VOLUME,VOLUME, VOLUME_UNIT, STOCK_CONC, STOCK_CONC_UNITS };

    static Fields fields;
private:


signals:
   void sendModel(QStandardItemModel * model);


public slots:

 void updateLine(QStandardItem*);


};

#endif // MODEL_H
