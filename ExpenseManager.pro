QT += core widgets

CONFIG += c++11

TARGET = ExpenseManager
TEMPLATE = app

SOURCES += \
    main.cpp \
    CSVreader.cpp \
    mainwindow.cpp

HEADERS += \
    CSVreader.h \
    mainwindow.h

