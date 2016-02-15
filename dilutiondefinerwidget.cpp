#include "dilutiondefinerwidget.h"
#include "ui_dilutiondefiner.h"


DilutionDefiner::DilutionDefiner(ModQSortProxyModel *proxy, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DilutionDefiner)
{
    ui->setupUi(this);
    //proxyModel=new QSortFilterProxyModel;
    proxyModel=proxy;
    connect(ui->applyButton,SIGNAL(clicked()),this,SLOT(slotToEmitQuantities()) );
    ui->unitsVolCombo->addItem("ml");
    ui->unitsVolCombo->addItem("ul");
    ui->unitsVolCombo->addItem("nl");

    ui->unitsConcCombo->addItem("mg/ml");
    ui->unitsConcCombo->addItem("ug/ml");
    ui->unitsConcCombo->addItem("ng/ml");
    ui->unitsConcCombo->addItem("pg/ml");
    ui->unitsConcCombo->addItem("ng/ul");
    ui->unitsConcCombo->addItem("pg/ul");
    ui->unitsConcCombo->addItem("pg/nl");
    ui->unitsConcCombo->addItem("M");
    ui->unitsConcCombo->addItem("mM");
    ui->unitsConcCombo->addItem("uM");
    ui->unitsConcCombo->addItem("nM");
    ui->unitsConcCombo->addItem("pM");

    ui->outputUnitCombo->addItem("mg/ml");
    ui->outputUnitCombo->addItem("ug/ml");
    ui->outputUnitCombo->addItem("ng/ml");
    ui->outputUnitCombo->addItem("pg/ml");
    ui->outputUnitCombo->addItem("ng/ul");
    ui->outputUnitCombo->addItem("pg/ul");
    ui->outputUnitCombo->addItem("pg/nl");
    ui->outputUnitCombo->addItem("M");
    ui->outputUnitCombo->addItem("mM");
    ui->outputUnitCombo->addItem("uM");
    ui->outputUnitCombo->addItem("nM");
    ui->outputUnitCombo->addItem("pM");
}

DilutionDefiner::~DilutionDefiner()
{
    delete ui;
}

void DilutionDefiner::slotToEmitQuantities()
{
    qDebug()<<"dilutionDefinerWidget::calculateQuantities-slot";
    //emit sendSolutionName(ui->solutionNameCombo->currentText());
    for (int i=0; i<proxyModel->rowCount();i++){
        proxyModel->setData(proxyModel->index(i,proxyModel->DILUTION),ui->dilutionNameCombo->currentText());
        proxyModel->setData(proxyModel->index(i,proxyModel->SAMPLE),ui->sampleNameCombo->currentText());
        proxyModel->setData(proxyModel->index(i,proxyModel->BUFFER),ui->bufferNameCombo->currentText());
        proxyModel->setData(proxyModel->index(i,proxyModel->CONC_UNIT),ui->unitsConcCombo->currentText());
        proxyModel->setData(proxyModel->index(i,proxyModel->VOLUME),ui->totalVolumeSpin->value());
        proxyModel->setData(proxyModel->index(i,proxyModel->VOLUME_UNIT),ui-> unitsVolCombo ->currentText());
        proxyModel->setData(proxyModel->index(i,proxyModel->STOCK_CONC), ui-> quantitySpin ->value());
    }
    proxyModel->outputUnit=ui->outputUnitCombo->currentText();
    if ((ui->radioLinear->isChecked()==false) && ui->minValueSpin->value()==0){
        QMessageBox::warning(this, tr("Oooops!"),
                             tr("Can not start with zero with the log option"));
                return;
    }
    emit calculateQuantitiesParams(ui->radioLinear->isChecked(),ui->minValueSpin->value() , ui->maxValueSpin->value());
}


