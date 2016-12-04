#-------------------------------------------------
#
# Project created by QtCreator 2009-12-24T17:20:02
#
#-------------------------------------------------

TARGET = LePlatz
QT += network
TEMPLATE = app
SOURCES += main.cpp \
    PlatzWin.cpp
HEADERS += PlatzWin.h \
    PlatzEnums.h
FORMS += PlatzWin.ui
RC_FILE = LePlatz.rc

Debug:DEFINES += LEPLATZ_DEBUG
Debug:include(ModelTest/ModelTest.pri)
include(DataModel/DataModel.pri)
include(TextView/TextView.pri)
include(GraphicsView/GraphicsView.pri)
include(GraphicsList/GraphicsList.pri)
include(WorldCompiler/WorldCompiler.pri)
include(StatePersister/StatePersister.pri)
include(Resources/Resources.pri)
include(Widgets/Widgets.pri)
include(About/About.pri)
include(Updater/Updater.pri)
