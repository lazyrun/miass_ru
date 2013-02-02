TEMPLATE = app
TARGET = miass_ru
DESTDIR = bin

CONFIG += debug_and_release precompile_header
CONFIG -= flat
QT += xml network
win32:QMAKE_CXXFLAGS += -MP
TRANSLATIONS += translations/miass_ru.ts
win32:RC_FILE = miass_ru.rc

PRECOMPILED_HEADER  = qtincludes.h
INCLUDEPATH  += ./ gui
DEPENDPATH   += ./ gui

RESOURCES += resources/miass_ru.qrc

# Input
HEADERS += qtincludes.h \
           $$files(gui/*.h)

SOURCES += main.cpp \
           $$files(gui/*.cpp)

CONFIG(debug, debug|release) {
     TARGET = $$join(TARGET,,,_d)
     OBJECTS_DIR = obj/Debug
     UI_DIR = obj/Debug/ui
     MOC_DIR = obj/Debug/moc
     RCC_DIR = obj/Debug
     DEPENDPATH += . obj/Debug/ui obj/Debug/moc
} else {
     OBJECTS_DIR = obj/Release
     UI_DIR = obj/Release/ui
     MOC_DIR = obj/Release/moc
     RCC_DIR = obj/Debug
     DEPENDPATH += . obj/Release/ui obj/Release/moc
}
include(gui/CheckTableModel/CheckTableModel.pri)