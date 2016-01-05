#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "plateviewwidget.h"
#include "model.h"
#include "selectionwidget.h"
#include "dilutiondefinerwidget.h"
#include "ui_dilutiondefiner.h"
#include "calculator.h"

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    Model *model;
    SelectionWidget *selectionWidget;
    QTimer *debugTimer;
    QPlainTextEdit *textEdit;

private slots:
    void newLetter();
    void save();
    void open();
    void loadFile(const QString &fileName);
    void print();
    void undo();
    void about();
    void setWorkInProgress();
    void getCalculationResults(QString);
    void resizeEvent(QResizeEvent *e);


private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWidgets();
    DilutionDefiner *definer;
    PlateView *plateView;
    QListWidget *customerList;
    QListWidget *paragraphsList;
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QAction *newLetterAct;
    QAction *saveAct;
    QAction *openAct;
    QAction *undoAct;
    QAction *aboutAct;
    QAction *quitAct;


    bool workInProgress;
};
//! [0]

#endif
