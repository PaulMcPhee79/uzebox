# -------------------------------------------------
# Project created by QtCreator 2010-01-21T01:01:48
# -------------------------------------------------
TARGET = LePatch
TEMPLATE = app
SOURCES += main.cpp \
    LePatchDlg.cpp \
    Patcher.cpp \
    Interpreter.cpp
HEADERS += LePatchDlg.h \
    Patcher.h \
    Interpreter.h
FORMS += LePatchDlg.ui
RC_FILE = LePatch.rc
include(Resources/Resources.pri)
