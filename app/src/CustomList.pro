QT += core gui widgets

TARGET = CustomList
TEMPLATE = app

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    ListView.cpp \
    Delegate.cpp

HEADERS += \
    MainWindow.h \
    ListView.h \
    Delegate.h

RESOURCES += \
    resources.qrc
