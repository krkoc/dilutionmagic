#include <QtWidgets>
#include <QtPrintSupport>
#include <QDomDocument>
#include "MainWindow.h"
#include "selectionwidget.h"
#include "converter.h"
#include <QDebug>


MainWindow::MainWindow()
{
    Converter conv;
    conv.convert(100,"mg/ml","ng/ul");
    workInProgress=false;
    model = new Model(this);
    textEdit = new QPlainTextEdit;
    selectionWidget= new SelectionWidget;
    selectionWidget->setMinimumWidth(300);
    selectionWidget->setMaximumWidth(800);

    selectionWidget->setSourceModel(model->createMailModel(selectionWidget));
    definer = new DilutionDefiner(selectionWidget->proxyModel);
    definer->setMinimumWidth(220);
    definer->setMaximumWidth(220);
    plateView = new PlateView(selectionWidget->proxyModel,this) ;

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
    connect(definer->ui->applyButton,SIGNAL(clicked()),selectionWidget,SLOT( calculatePipetingProcedure()));
    connect(definer->ui->calculateButton,SIGNAL(clicked()),selectionWidget,SLOT( calculatePipetingProcedure()));
    connect(definer,SIGNAL(calculateQuantitiesParams(byte,double,double)),selectionWidget,SLOT(fillSelection(byte, double, double)));
    connect(definer->proxyModel,SIGNAL(modelModified()),this,SLOT(setWorkInProgress()));
    connect(plateView,SIGNAL(searchString(QString)),selectionWidget,SLOT(textFilterChanged(QString)));
    connect(selectionWidget->calc,SIGNAL(sendCalculationResults(QString)),this,SLOT(getCalculationResults(QString)));
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
    for (int i=0; i<colCount-1 ;i++) stdm->appendColumn(item);

    stdm->setHeaderData(1, Qt::Horizontal, QObject::tr("Dilution"));
    stdm->setHeaderData(2, Qt::Horizontal, QObject::tr("Sample"));
    stdm->setHeaderData(3, Qt::Horizontal,  QObject::tr("Buffer"));
    stdm->setHeaderData(4, Qt::Horizontal, QObject::tr("Concentration"));
    stdm->setHeaderData(5, Qt::Horizontal, QObject::tr("Conc. units"));
    stdm->setHeaderData(6, Qt::Horizontal, QObject::tr("Stock volume"));
    stdm->setHeaderData(7, Qt::Horizontal, QObject::tr("well volume"));
    stdm->setHeaderData(8, Qt::Horizontal,QObject::tr("Volume units"));
    stdm->setHeaderData(9, Qt::Horizontal,QObject::tr("Stock conc"));


    plateView->updateColors();
////    for (in)
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
    qDebug()<<headerStringList;
    headerStringList.replaceInStrings(" ","_");

    QDomDocument doc("PlanX");
    QDomElement root = doc.createElement("PlanX");
    doc.appendChild(root);
    qDebug()<<"hslist"<<headerStringList;
    //WELL_NAME, DILUTION, SAMPLE, BUFFER, QUANTITY, UNIT, STOCK_VOLUME, VOLUME
    for (int i=0; i<stdm->rowCount();i++){
        QDomElement tag = doc.createElement("Well");
        var=stdm->data(stdm->index(i,selectionWidget->proxyModel->WELL_NAME),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(selectionWidget->proxyModel->WELL_NAME),var.toString());
        var=stdm->data(stdm->index(i,selectionWidget->proxyModel->DILUTION),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(selectionWidget->proxyModel->DILUTION),var.toString());
        var=stdm->data(stdm->index(i,selectionWidget->proxyModel->SAMPLE),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(selectionWidget->proxyModel->SAMPLE),var.toString());
        var=stdm->data(stdm->index(i,selectionWidget->proxyModel->BUFFER),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(selectionWidget->proxyModel->BUFFER),var.toString());
        var=stdm->data(stdm->index(i,selectionWidget->proxyModel->QUANTITY),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(selectionWidget->proxyModel->QUANTITY),var.toString());
        var=stdm->data(stdm->index(i,selectionWidget->proxyModel->CONC_UNIT),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(selectionWidget->proxyModel->CONC_UNIT),var.toString());
        var=stdm->data(stdm->index(i,selectionWidget->proxyModel->STOCK_VOLUME),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(selectionWidget->proxyModel->STOCK_VOLUME),var.toString());
        var=stdm->data(stdm->index(i,selectionWidget->proxyModel->VOLUME),Qt::DisplayRole);
        tag.setAttribute(headerStringList.at(selectionWidget->proxyModel->VOLUME),var.toString());

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
    while( !node.isNull() )
    {
        qDebug()<<i++;
        if( node.isElement() )
        {
            //WELL_NAME, DILUTION, SAMPLE, BUFFER, QUANTITY, UNIT, STOCK_VOLUME, VOLUME
            QDomElement element = node.toElement();
            qDebug() << "ELEMENT" << element.tagName();
             selectionWidget-> proxyModel->setFilterRegExp(element.attribute("Well", "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,selectionWidget->proxyModel->SAMPLE),element.attribute( "Sample", "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,selectionWidget->proxyModel->DILUTION),element.attribute( "Dilution", "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,selectionWidget->proxyModel->BUFFER),element.attribute( "Buffer", "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,selectionWidget->proxyModel->STOCK_VOLUME),element.attribute( "Stock_volume", "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,selectionWidget->proxyModel->VOLUME),element.attribute( "Total_volume", "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,selectionWidget->proxyModel->CONC_UNIT),element.attribute( "Unit", "" ));
             selectionWidget-> proxyModel->setData(selectionWidget->proxyModel->index(0,selectionWidget->proxyModel->QUANTITY),element.attribute( "Quantity", "" ));
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
   QMessageBox::about(this, tr("About Dock Widgets"),
            tr("The <b>Dock Widgets</b> example demonstrates how to "
               "use Qt's dock widgets. You can enter your own text, "
               "click a customer to add a customer name and "
               "address, and click standard paragraphs to add them."));
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
    newLetterAct = new QAction(QIcon(":/images/new.png"), tr("&New Letter"),
                               this);
    newLetterAct->setShortcuts(QKeySequence::New);
    newLetterAct->setStatusTip(tr("Create a new form letter"));
    connect(newLetterAct, SIGNAL(triggered()), this, SLOT(newLetter()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open existing experiment"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));


    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current form letter"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    printAct = new QAction(QIcon(":/images/print.png"), tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the current form letter"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    undoAct = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last editing action"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newLetterAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    viewMenu = menuBar()->addMenu(tr("&View"));
    menuBar()->addSeparator();
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newLetterAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(openAct);

    fileToolBar->addAction(printAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(undoAct);
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
    QDockWidget *dock = new QDockWidget(tr("Selected well"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
    dock->setWidget(selectionWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("dilution"), this);
    dock->setWidget(definer);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());


//    dock = new QDockWidget(tr("plateview"), this);
//    dock->setWidget(plateView);
//    addDockWidget(Qt::TopDockWidgetArea, dock);
//    viewMenu->addAction(dock->toggleViewAction());


    dock = new QDockWidget(tr("textView"), this);
    dock->setWidget(textEdit);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

}
//! [9]
