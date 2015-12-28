#-------------------------------------------------
#
# Project created by QtCreator 2015-06-15T23:55:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = psytestLauncher
TEMPLATE = app

INCLUDEPATH += model\
    view\
    helper


SOURCES += main.cpp\
    helper/jsonhelper.cpp \
    model/section.cpp \
    model/iqaitem.cpp \
    model/qaitemfixed.cpp \
    model/qaitemarbitrary.cpp \
    model/qaitemfixedimages.cpp \
    model/dictionary.cpp \
    view/dictionarydialog.cpp \
    view/dictionaryrow.cpp \
    view/mainwindow.cpp \
    model/test.cpp \
    model/ratedanswer.cpp \
    model/isection.cpp \
    model/interpretation.cpp \
    model/conversiontable.cpp

HEADERS  += helper/jsonhelper.h \
    model/iqaitem.h \
    model/qaitemfixed.h \
    model/qaitemarbitrary.h \
    model/qaitemfixedimages.h \
    model/section.h \
    model/dictionary.h \
    view/dictionarydialog.h \
    view/dictionaryrow.h \
    view/mainwindow.h \
    model/test.h \
    model/ratedanswer.h \
    model/isection.h \
    model/interpretation.h \
    model/conversiontable.h

FORMS  += view/mainwindow.ui \
    view/dictionarydialog.ui \
    view/dictionaryrow.ui

RESOURCES += \
    resources.qrc

# Tell the qcustomplot header that it will be used as library:
DEFINES += QCUSTOMPLOT_USE_LIBRARY
# Link with debug version of qcustomplot if compiling in debug mode, else with release library:
CONFIG(debug, release|debug) {
  win32:QCPLIB = qcustomplotd1
  else: QCPLIB = qcustomplotd
} else {
  win32:QCPLIB = qcustomplot1
  else: QCPLIB = qcustomplot
}
LIBS += -L./ -l$$QCPLIB
LIBS += ../quazip-0.7.1/quazip/release/libquazip.a
