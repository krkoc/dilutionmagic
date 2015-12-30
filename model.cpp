#include "model.h"
#include <QObject>
#include <QDebug>

Model::Model(QObject *parent) : QObject(parent)
{
    model = new QStandardItemModel;
    headers<<"Well"<<"Dilution_name"<<"Sample"<<"Buffer"<<"Concentration"<<"Concunits"<<"Stock_volume"<<"Well_volume"<<"Volume_units"<<"Stock_conc";

}


void Model::setEntry(QStandardItemModel *model, const QString &subject,
             const QString &sender, const double &conc, int index)
{
   // model->setData(model->index(index, 0), subject);
   // model->setData(model->index(index, 1), sender);
    //model->setData(model->index(index, 2), conc);
}




QStandardItemModel *Model::createMailModel(QObject *parent)
{
    model = new QStandardItemModel(96, 10, parent);
    int colCount = model->columnCount();
    QStringList modHdr;
    modHdr=headers.replaceInStrings("_"," ");
    for (int i=0; i<colCount ;i++){

        model->setHeaderData(i, Qt::Horizontal,modHdr.at(i));
    }



    //A
    model->setData(model->index(0,0),"A1");     model->setData(model->index(1,0),"A2");     model->setData(model->index(2,0),"A3");
    model->setData(model->index(3,0),"A4");     model->setData(model->index(4,0),"A5");    model->setData(model->index(5,0),"A6");
    model->setData(model->index(6,0),"A7");    model->setData(model->index(7,0),"A8");    model->setData(model->index(8,0),"A9");
    model->setData(model->index(9,0),"A_10");    model->setData(model->index(10,0),"A_11");    model->setData(model->index(11,0),"A_12");
    //B
    model->setData(model->index(12,0),"B1");    model->setData(model->index(13,0),"B2");    model->setData(model->index(14,0),"B3");
    model->setData(model->index(15,0),"B4");    model->setData(model->index(16,0),"B5");    model->setData(model->index(17,0),"B6");
    model->setData(model->index(18,0),"B7");    model->setData(model->index(19,0),"B8");    model->setData(model->index(20,0),"B9");
    model->setData(model->index(21,0),"B_10");    model->setData(model->index(22,0),"B_11");    model->setData(model->index(23,0),"B_12");

   //C
    model->setData(model->index(24,0),"C1");    model->setData(model->index(25,0),"C2");    model->setData(model->index(26,0),"C3");
    model->setData(model->index(27,0),"C4");    model->setData(model->index(28,0),"C5");    model->setData(model->index(29,0),"C6");
    model->setData(model->index(30,0),"C7");    model->setData(model->index(31,0),"C8");    model->setData(model->index(32,0),"C9");
    model->setData(model->index(33,0),"C_10");    model->setData(model->index(34,0),"C_11");    model->setData(model->index(35,0),"C_12");
    //D
    model->setData(model->index(36,0),"D1");    model->setData(model->index(37,0),"D2");    model->setData(model->index(38,0),"D3");
    model->setData(model->index(39,0),"D4");    model->setData(model->index(40,0),"D5");    model->setData(model->index(41,0),"D6");
    model->setData(model->index(42,0),"D7");    model->setData(model->index(43,0),"D8");    model->setData(model->index(44,0),"D9");
    model->setData(model->index(45,0),"D_10");    model->setData(model->index(46,0),"D_11");    model->setData(model->index(47,0),"D_12");

    model->setData(model->index(48,0),"E1");    model->setData(model->index(49,0),"E2");    model->setData(model->index(50,0),"E3");
    model->setData(model->index(51,0),"E4");    model->setData(model->index(52,0),"E5");    model->setData(model->index(53,0),"E6");
    model->setData(model->index(54,0),"E7");    model->setData(model->index(55,0),"E8");    model->setData(model->index(56,0),"E9");
    model->setData(model->index(57,0),"E_10");    model->setData(model->index(58,0),"E_11");    model->setData(model->index(59,0),"E_12");

    model->setData(model->index(60,0),"F1");    model->setData(model->index(61,0),"F2");    model->setData(model->index(62,0),"F3");
    model->setData(model->index(63,0),"F4");    model->setData(model->index(64,0),"F5");    model->setData(model->index(65,0),"F6");
    model->setData(model->index(66,0),"F7");    model->setData(model->index(67,0),"F8");    model->setData(model->index(68,0),"F9");
    model->setData(model->index(69,0),"F_10");    model->setData(model->index(70,0),"F_11");    model->setData(model->index(71,0),"F_12");

    model->setData(model->index(72,0),"G1");    model->setData(model->index(73,0),"G2");    model->setData(model->index(74,0),"G3");
    model->setData(model->index(75,0),"G4");    model->setData(model->index(76,0),"G5");    model->setData(model->index(77,0),"G6");
    model->setData(model->index(78,0),"G7");    model->setData(model->index(79,0),"G8");    model->setData(model->index(80,0),"G9");
    model->setData(model->index(81,0),"G_10");    model->setData(model->index(82,0),"G_11");    model->setData(model->index(83,0),"G_12");

    model->setData(model->index(84,0),"H1");    model->setData(model->index(85,0),"H2");    model->setData(model->index(86,0),"H3");
    model->setData(model->index(87,0),"H4");    model->setData(model->index(88,0),"H5");    model->setData(model->index(89,0),"H6");
    model->setData(model->index(90,0),"H7");    model->setData(model->index(91,0),"H8");    model->setData(model->index(92,0),"H9");
    model->setData(model->index(93,0),"H_10");    model->setData(model->index(94,0),"H_11");    model->setData(model->index(95,0),"H_12");

    return model;
}



