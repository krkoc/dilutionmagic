/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


//  enum Fields {WELL_NAME, DILUTION, SAMPLE, BUFFER, QUANTITY, UNIT, STOCK_VOLUME, VOLUME };
#include <QtWidgets>
#include <QDebug>
#include <QLocale>
#include "selectionwidget.h"
#include <QtMath>

//! [0]
SelectionWidget::SelectionWidget()
{
    proxyModel = new ModQSortProxyModel(this);
    sourceView = new QTableView;
    sourceView->setAlternatingRowColors(true);
    QHBoxLayout *sourceLayout = new QHBoxLayout;

    proxyView = new QTableView;
    //proxyView->setRootIsDecorated(false);
    proxyView->setAlternatingRowColors(true);
    proxyView->setModel(proxyModel);
    proxyView->setSortingEnabled(true);

    //! [5]
    selMod = proxyView->selectionModel();
    //! [6]
    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(proxyView);
    setLayout(mainLayout);

    resize(500, 450);
    proxyModel->setFilterRegExp("");
    calc = new Calculator;
}


void SelectionWidget::printChange()
{
    qDebug()<<"changed";
}

void SelectionWidget::setSourceModel(QStandardItemModel *model)
{
    proxyModel->setSourceModel(model);
    sourceView->setModel(model);
    QVariant quantity;
    //quantity=sourceView->model()->data(sourceView->model()->index(1,2),Qt::DisplayRole);
    //sourceView->setSelection(QRect(0,0,1,1));
}

void SelectionWidget::textFilterChanged(QString st)
{
    qDebug()<<"textFiltChanged; "<<st;
    //if (st=="") st="fake string that yealds empty selection";
    proxyModel->setFilterRegExp(st);
    proxyView->selectColumn(0);
    proxyModel->sort(proxyModel->WELL_NAME,Qt::AscendingOrder);
}
//! [8]


void SelectionWidget::dateFilterChanged()
{

}

void SelectionWidget::calculatePipetingProcedure()
{
    qDebug()<<"pipeting procedure initiated";
    Qt::SortOrder SortOrder = proxyModel->sortOrder();
    proxyModel->sort(proxyModel->QUANTITY,Qt::DescendingOrder);
    //make instructions
    QStandardItem item;
    QStandardItemModel *stdm;
    stdm=reinterpret_cast<QStandardItemModel*>(proxyModel);
    QVariant conc, vol, well;
    QList <double> concentrations;
    QList <double> volumes;
    QList <QString> wells;
    QList <QVariant> concUnitList;
    QList <QVariant> volUnitList;
    QList <QVariant> stockConcentrationList;

    QString concUnit, volUnit;
    double stockConcentration;
    for (int i=0; i<stdm->rowCount();i++){
      conc=stdm->data(stdm->index(i,proxyModel->QUANTITY),Qt::DisplayRole);
      vol=stdm->data(stdm->index(i,proxyModel->VOLUME),Qt::DisplayRole);
      well=stdm->data(stdm->index(i,proxyModel->WELL_NAME),Qt::DisplayRole);
      concUnitList.append(stdm->data(stdm->index(i,proxyModel->CONC_UNIT),Qt::DisplayRole));
      volUnitList.append(stdm->data(stdm->index(i,proxyModel->VOLUME_UNIT),Qt::DisplayRole));
      stockConcentrationList.append(stdm->data(stdm->index(i,proxyModel->STOCK_CONC),Qt::DisplayRole));

      concentrations.append(conc.toDouble());
      volumes.append(vol.toDouble());
      wells.append(well.toString());
    }


    stockConcentration = stockConcentrationList.first().toDouble();
    volUnit=volUnitList.first().toString();
    concUnit=concUnitList.first().toString();
    calc->setCalculationParameters(wells, concentrations, volumes, stockConcentration, concUnit , volUnit);
    proxyModel->sort(proxyModel->WELL_NAME,SortOrder);
}


void SelectionWidget::fillSelection(byte mode, double minVal, double maxVal)
{
    qDebug()<<"SelectionWidget::fillSelection()";
    QList <QModelIndex> indexList;
    indexList=selMod->selectedIndexes();
    qDebug()<<"minVal"<<minVal;
    qDebug()<<"maxVal"<<maxVal;
    double step= (maxVal-minVal)/(indexList.length()-1);
    int i=0;
    if (mode==1){
        qDebug()<<"linear mode is set";
    foreach (QModelIndex index, indexList){
        proxyModel->setData(proxyModel->index(index.row(),proxyModel->QUANTITY),minVal+i*step);
        i++;
    }
    }
    else {
        qDebug()<<"exponential mode is set";
        double factor=qPow(maxVal/minVal,1/(double)(indexList.count()-1));

        qDebug()<<"factor"<<factor;
        foreach (QModelIndex index, indexList){
        proxyModel->setData(proxyModel->index(index.row(),proxyModel->QUANTITY),minVal*qPow(factor,i));
        i++;
        }
    }

}

//! [9]
