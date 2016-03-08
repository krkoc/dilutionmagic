#include <QtWidgets>
#include <QtPrintSupport>
#include <QDomDocument>
#include "MainWindow.h"
#include "selectionwidget.h"
#include "converter.h"
#include <QDebug>


MainWindow::MainWindow()
{
    Converter converter;
    converter.molarMass=1000;
    qDebug()<<"converting.... "<<converter.convert(1,"ng/ul","mM");

    workInProgress=false;
    model = new Model(this);
    textEdit = new QPlainTextEdit;
    selectionWidget= new SelectionWidget;
    selectionWidget->setMinimumWidth(300);
    selectionWidget->setMaximumWidth(800);
    selectionWidget->setSourceModel(model->createMailModel(selectionWidget));
    definer = new DilutionDefiner(selectionWidget->proxyModel);
    definer->setMinimumWidth(320);
    plateView = new PlateView(selectionWidget->proxyModel,this) ;


    plateView->setMinimumSize(600,500);
    setCentralWidget(plateView);
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWidgets();
    setWindowTitle(tr("Dilution Magic"));
    //newLetter();
    setUnifiedTitleAndToolBarOnMac(true);
    connect(definer->ui->applyButton,SIGNAL(clicked()),plateView,SLOT(updateColors()));

    connect(definer->ui->calculateButton,SIGNAL(clicked()),selectionWidget,SLOT( calculatePipetingProcedure()));
    connect(definer,SIGNAL(calculateQuantitiesParams(byte,double,double)),selectionWidget,SLOT(fillSelection(byte, double, double)));
    //connect(definer->proxyModel,SIGNAL(modelModified()),this,SLOT(setWorkInProgress()));
    connect(plateView,SIGNAL(searchString(QString)),selectionWidget,SLOT(textFilterChanged(QString)));
    connect(selectionWidget->calc,SIGNAL(sendCalculationResults(QString)),this,SLOT(getCalculationResults(QString)));
    connect(definer->ui->applyButton,SIGNAL(clicked()),model,SLOT(updateLine(QStandardItem*)) );
}

//! [1]

//! [2]
void MainWindow::newLetter()
{
   QList<QStandardItem*> item;
    QStandardItemModel *stdm;

    stdm=reinterpret_cast<QStandardItemModel*>(selectionWidget->proxyModel->sourceModel());
    int colCount= stdm->columnCount();
    stdm->removeColumns(1,colCount-1);
    for (int i=1; i<colCount  ;i++){
        stdm->appendColumn(item);
        stdm->setHeaderData(i, Qt::Horizontal,model->headers.at(i));
    }

    plateView->updateColors();

//    stdm->setData(selectionWidget->proxyModel->index(0,selectionWidget->proxyModel->SAMPLE),"" );

}

void MainWindow::print()
{
    QPrinter printer;

    QPrintDialog dlg(&printer, this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }


    statusBar()->showMessage(tr("Ready"), 2000);

}
//! [3]

//! [4]
void MainWindow::save()
{
    QStandardItem item;
    QStandardItemModel *stdm;
    QVariant var;
    QList<QVariant> headerList;
    QStringList headerStringList;
    stdm=reinterpret_cast<QStandardItemModel*>(selectionWidget->proxyModel->sourceModel());
    for (int j=0; j<stdm->columnCount();j++){
        headerList.append(stdm->headerData(j,Qt::Horizontal,Qt::DisplayRole));
    }
    foreach (QVariant header, headerList){
        headerStringList.append(header.toString());
    }
   // qDebug()<<headerStringList;
    headerStringList=model->headers;
    headerStringList.replaceInStrings(" ","_");

    QDomDocument doc("DilutionMagicDoc");
    QDomElement root = doc.createElement("DilutionMagicDoc");
    doc.appendChild(root);
    qDebug()<<"hslist"<<headerStringList;
    //   WELL_NAME, DILUTION, SAMPLE, BUFFER, QUANTITY, CONC_UNIT, STOCK_VOLUME, BUFFER_VOLUME,VOLUME, VOLUME_UNIT, STOCK_CONC, STOCK_CONC_UNITS };


    for (int i=0; i<stdm->rowCount();i++){
        QDomElement tag = doc.createElement("Well");
        var=stdm->data(stdm->index(i,Model::WELL_NAME),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at( Model::WELL_NAME),var.toString());

        var=stdm->data(stdm->index(i,Model::DILUTION),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(Model::DILUTION),var.toString());

        var=stdm->data(stdm->index(i,Model::SAMPLE),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(Model::SAMPLE),var.toString());

        var=stdm->data(stdm->index(i,Model::BUFFER),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(Model::BUFFER),var.toString());

        var=stdm->data(stdm->index(i,Model::QUANTITY),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(Model::QUANTITY),var.toString());

        var=stdm->data(stdm->index(i,Model::CONC_UNIT),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(Model::CONC_UNIT),var.toString());

        var=stdm->data(stdm->index(i,Model::STOCK_VOLUME),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(Model::STOCK_VOLUME),var.toString());

        var=stdm->data(stdm->index(i,Model::BUFFER_VOLUME),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(Model::BUFFER_VOLUME),var.toString());

        var=stdm->data(stdm->index(i,Model::VOLUME),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(Model::VOLUME),var.toString());

        var=stdm->data(stdm->index(i,Model::VOLUME_UNIT ),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(Model::VOLUME_UNIT),var.toString());

        var=stdm->data(stdm->index(i,Model::STOCK_CONC),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(Model::STOCK_CONC),var.toString());


        var=stdm->data(stdm->index(i,Model::STOCK_CONC_UNITS),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(Model::STOCK_CONC_UNITS),var.toString());
        root.appendChild(tag);

       }


    QString xml = doc.toString();
    qDebug()<<xml;
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Choose a file name"), ".",
                                                    tr("XML (*.xml)"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Dock Widgets"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QByteArray bla="";
    bla.append(xml);
    file.write(QByteArray(bla)  );
    file.close();

    statusBar()->showMessage(tr("Saved '%1'").arg(fileName), 2000);
    workInProgress=false;
}
//! [4]


void MainWindow::open()
//! [7] //! [8]
{
     if (workInProgress) {
         QMessageBox::warning(this, tr("Warning"),
                              tr("Experiment was modified, but not saved!" ));


     }
  //  if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
 //   }
}


//! [5]
void MainWindow::undo()
{

}
//! [5]


void MainWindow::loadFile(const QString &fileName)
//! [42] //! [43]
{
    QFile file(fileName);
    QDomDocument doc;
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    //call function here in selectionwidget class


    QDomElement root = doc.documentElement(); //root of xml
    qDebug()<<"are we here?";
    qDebug()<<root.tagName();
    QDomNode node = root.firstChild();

    // Here we append a new element to the end of the document
    int i=0;
    QStringList headerStringList = model->headers.replaceInStrings(" ","_");
    while( !node.isNull() )
    {
        qDebug()<<i++;
        if( node.isElement() )
        {
            //   WELL_NAME, DILUTION, SAMPLE, BUFFER, QUANTITY, CONC_UNIT, STOCK_VOLUME, BUFFER_VOLUME,VOLUME, VOLUME_UNIT, STOCK_CONC, STOCK_CONC_UNITS };

            QDomElement element = node.toElement();

            qDebug() << "ELEMENT" << element.tagName();
             selectionWidget-> proxyModel->setFilterRegExp(element.attribute("Well", "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,Model::SAMPLE),element.attribute(headerStringList.at(Model::SAMPLE), "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,Model::DILUTION),element.attribute(headerStringList.at(Model::DILUTION), "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,Model::BUFFER),element.attribute(headerStringList.at(Model::BUFFER), "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,Model::QUANTITY),element.attribute(headerStringList.at(Model::QUANTITY), "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,Model::CONC_UNIT),element.attribute( headerStringList.at(Model::CONC_UNIT), "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,Model::STOCK_VOLUME),element.attribute(headerStringList.at(Model::STOCK_VOLUME), "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,Model::BUFFER_VOLUME),element.attribute(headerStringList.at(Model::BUFFER_VOLUME), "" ));


             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,Model::VOLUME ),element.attribute(headerStringList.at(Model::VOLUME ), "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,Model::VOLUME_UNIT ),element.attribute(headerStringList.at(Model::VOLUME_UNIT ), "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,Model::STOCK_CONC ),element.attribute(headerStringList.at(Model::STOCK_CONC ), "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,Model::STOCK_CONC_UNITS ),element.attribute(headerStringList.at(Model::STOCK_CONC_UNITS ), "" ));
             qDebug()<<model->headers.at(Model::DILUTION);

        }
        if( node.isText() )
        {
            QDomText text = node.toText();
            qDebug() << text.data();
        }
    node = node.nextSibling();
}
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    //setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    plateView->updateColors();
}


void MainWindow::about()
{
   QMessageBox::about(this, tr("About DilutionMagic"),
            tr("DilutionMagic is a free dilution calculator. "
               "It can do serial dilutions <b> with non-constant sample volumes</b> "
               "and <b>custom concentration steps.</b> Let DilutionMagic do the math!"));
}

void MainWindow::setWorkInProgress()
{
    workInProgress=true;
}

void MainWindow::getCalculationResults(QString text)
{
    textEdit->setPlainText(text);
    qDebug()<<":getCalculationResults";
}



void MainWindow::createActions()
{
    newLetterAct = new QAction(QIcon(":/images/new.png"), tr("&New Plate"),this);
    newLetterAct->setShortcuts(QKeySequence::New);
    newLetterAct->setStatusTip(tr("Create a new plate"));
    connect(newLetterAct, SIGNAL(triggered()), this, SLOT(newLetter()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open existing experiment"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));


    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current form plate"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

//   aboutQtAct = new QAction(tr("About &Qt"), this);

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newLetterAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);
    //  editMenu = menuBar()->addMenu(tr("&Edit"));
    viewMenu = menuBar()->addMenu(tr("&View"));
    menuBar()->addSeparator();
    helpMenu = menuBar()->addMenu(tr("&About"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newLetterAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(openAct);
}

//! [8]
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}
//! [8]

//! [9]
void MainWindow::createDockWidgets()
{
    QDockWidget *dock = new QDockWidget(tr("Table View"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
    dock->setWidget(selectionWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("Dilution"), this);
    dock->setWidget(definer);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());


    dock = new QDockWidget(tr("Text View"), this);
    dock->setWidget(textEdit);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

}


void MainWindow::resizeEvent(QResizeEvent *e)  // virtual
{
   plateView->graphicsView->setGeometry(0,0,centralWidget()->width(),centralWidget()->height());
   QMainWindow::resizeEvent(e);
}
//! [9]
