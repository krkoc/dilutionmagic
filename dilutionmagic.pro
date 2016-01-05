QT += widgets xml
qtHaveModule(printsupport): QT += printsupport

win32:RC_ICONS += dmicon.ico

HEADERS =     viewevents.h \
    model.h \
    selectionwidget.h \
    paintedrect.h \
    dilutiondefinerwidget.h \
    plateviewwidget.h \
    mainwindow.h \
    calculator.h \
    modqsortproxymodel.h \
    converter.h

SOURCES     = main.cpp \
    viewevents.cpp \
    model.cpp \
    selectionwidget.cpp \
    paintedrect.cpp \
    dilutiondefinerwidget.cpp \
    plateviewwidget.cpp \
    mainwindow.cpp \
    calculator.cpp \
    modqsortproxymodel.cpp \
    converter.cpp
RESOURCES       = dockwidgets.qrc

# install
# target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainSelectionWidgets/dockwidgets
INSTALLS += target

FORMS += \
    dilutiondefiner.ui
