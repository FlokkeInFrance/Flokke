QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    basemanager.cpp \
    datawizard.cpp \
    debugger.cpp \
    linguist.cpp \
    main.cpp \
    mainwindow.cpp \
    connecttobase.cpp \
    centreop.cpp \
    checkbase.cpp \
    man_boats_photos.cpp \
    man_cruiseplanning.cpp \
    man_ingredients_tools.cpp \
    man_menu_and_dishes.cpp \
    man_shelves_conversions.cpp \
    man_storage_provision.cpp \
    myparameters.cpp \
    w_cruiseorganiser.cpp \
    w_europeandateeditor.cpp \
    w_maincruisewindows.cpp

HEADERS += \
    basemanager.h \
    datawizard.h \
    debugger.h \
    linguist.h \
    mainwindow.h \
    connecttobase.h \
    centreop.h \
    checkbase.h \
    man_boats_photos.h \
    man_cruiseplanning.h \
    man_ingredients_tools.h \
    man_menu_and_dishes.h \
    man_shelves_conversions.h \
    man_storage_provision.h \
    myparameters.h \
    w_cruiseorganiser.h \
    w_europeandateeditor.h \
    w_maincruisewindows.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
