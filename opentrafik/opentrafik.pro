#-------------------------------------------------
#
# Project created by QtCreator 2013-05-28T21:52:13
#
#-------------------------------------------------

QT       += core gui svg sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

ICON = icons/icon.png
RC_FILE = icons/opentrafik_icon.rc

TARGET = opentrafik
TEMPLATE = app

include(qtsingleapplication/src/qtsingleapplication.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    dialogaddwarehouse.cpp \
    dialogsellitem.cpp \
    dialogwarehouse.cpp \
    taxcombobox.cpp \
    dialogaddnewbatchitem.cpp \
    dialogwarehouseitemsbatchitems.cpp \
    sqlhelper.cpp \
    common.cpp \
    dialogwarehouseitemhistory.cpp \
    dialogreport.cpp \
    dialogdatabasesettings.cpp \
    dialogguisettings.cpp \
    dialoglogin.cpp \
    dialogabout.cpp \
    barcodelineedit.cpp \
    dialogusers.cpp \
    dialogadduser.cpp \
    ecrdufilereader.cpp \
    ecrfilemonitor.cpp \
    systemtray.cpp \
    dialogecrsettings.cpp \
    dialogtaxedit.cpp \
    warehouseitemnamecompleter.cpp \
    dialogitemdetails.cpp \
    dialogcorrector.cpp \
    dialogshopdetails.cpp \
    dialogaddpricebruttoin.cpp \
    dialogimports.cpp \
    dialogmovers.cpp \
    dialogbase.cpp \
    dialogimportdetails.cpp \
    dialogbackup.cpp \
    dialogadvancedwarehousefilter.cpp \
    dialogsuppliers.cpp \
    dialoginvoices.cpp \
    suppliercombobox.cpp \
    dialogaddinvoice.cpp \
    dialogprintlabels.cpp

HEADERS  += mainwindow.h \
    dialogaddwarehouse.h \
    dialogsellitem.h \
    dialogwarehouse.h \
    taxcombobox.h \
    dialogaddnewbatchitem.h \
    dialogwarehouseitemsbatchitems.h \
    sqlhelper.h \
    common.h \
    dialogwarehouseitemhistory.h \
    dialogreport.h \
    dialogdatabasesettings.h \
    dialogguisettings.h \
    dialoglogin.h \
    dialogabout.h \
    barcodelineedit.h \
    dialogusers.h \
    dialogadduser.h \
    ecrdufilereader.h \
    ecrfilemonitor.h \
    systemtray.h \
    dialogecrsettings.h \
    dialogtaxedit.h \
    warehouseitemnamecompleter.h \
    dialogitemdetails.h \
    dialogcorrector.h \
    dialogshopdetails.h \
    dialogaddpricebruttoin.h \
    svnversion.h \
    dialogimports.h \
    dialogmovers.h \
    dialogbase.h \
    dialogimportdetails.h \
    build_number.h \
    dialogbackup.h \
    dialogadvancedwarehousefilter.h \
    dialogsuppliers.h \
    dialoginvoices.h \
    suppliercombobox.h \
    dialogaddinvoice.h \
    dialogprintlabels.h

FORMS    += mainwindow.ui \
    dialogaddwarehouse.ui \
    dialogsellitem.ui \
    dialogwarehouse.ui \
    dialogaddnewbatchitem.ui \
    dialogwarehouseitemsbatchitems.ui \
    dialogwarehouseitemhistory.ui \
    dialogreport.ui \
    dialogdatabasesettings.ui \
    dialogguisettings.ui \
    dialoglogin.ui \
    dialogabout.ui \
    dialogusers.ui \
    dialogadduser.ui \
    dialogecrsettings.ui \
    dialogtaxedit.ui \
    dialogitemdetails.ui \
    dialogcorrector.ui \
    dialogshopdetails.ui \
    dialogaddpricebruttoin.ui \
    dialogimports.ui \
    dialogmovers.ui \
    dialogimportdetails.ui \
    dialogbackup.ui \
    dialogadvancedwarehousefilter.ui \
    dialogsuppliers.ui \
    dialoginvoices.ui \
    dialogaddinvoice.ui \
    dialogprintlabels.ui

RESOURCES += \
    icons/icons.qrc \
    translations.qrc

win32 {
    build_nr.commands = $$PWD/increase_build.bat $$PWD
}

linux {
    build_nr.commands = $$PWD/increase_build.sh $$PWD
    build_nr.depends = FORCE
    QMAKE_EXTRA_TARGETS += build_nr
    PRE_TARGETDEPS += build_nr
}




TRANSLATIONS = opentrafik_hu.ts
