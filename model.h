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

private:


signals:
   void sendModel(QStandardItemModel * model);


public slots:


};

#endif // MODEL_H
