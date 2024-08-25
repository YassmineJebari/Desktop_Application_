#-------------------------------------------------
#
# Project created by QtCreator 2018-10-26T21:45:23
#
#-------------------------------------------------


QT       += core gui sql network printsupport charts serialport
QT  +=charts multimedia multimediawidgets sensors
QT       +=svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    application.cpp \
    arduino.cpp \
    binome1.cpp \
    borne.cpp \
    calculatrice.cpp \
    capteurultrason.cpp \
    chatbox.cpp \
    client.cpp \
    connection.cpp \
    deuxiemeinterface.cpp \
    dialog.cpp \
    employe.cpp \
    gestionborne.cpp \
    gestionrendezvu.cpp \
    historique.cpp \
    inteface_produit.cpp \
    interfaceclient.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    notifications.cpp \
    produits.cpp \
    qrcode.cpp \
    rdv.cpp \
    smtp.cpp

HEADERS += \
    application.h \
    arduino.h \
    binome1.h \
    borne.h \
    calculatrice.h \
    capteurultrason.h \
    chatbox.h \
    client.h \
    connection.h \
    deuxiemeinterface.h \
    dialog.h \
    employe.h \
    gestionborne.h \
    gestionrendezvu.h \
    historique.h \
    inteface_produit.h \
    interfaceclient.h \
    login.h \
    mainwindow.h \
    notifications.h \
    produits.h \
    qrcode.h \
    rdv.h \
    smtp.h

FORMS += \
    application.ui \
    binome1.ui \
    calculatrice.ui \
    capteurultrason.ui \
    chatbox.ui \
    deuxiemeinterface.ui \
    dialog.ui \
    gestionborne.ui \
    gestionrendezvu.ui \
    inteface_produit.ui \
    interfaceclient.ui \
    mainwindow.ui

QMAKE_CXXFLAGS += -std=gnu++11


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




RESOURCES += \
    resource.qrc
