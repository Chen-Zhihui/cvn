TEMPLATE = app
TARGET = Apputilqt

QT += network \
      xml \
      multimedia \
      multimediawidgets \
      widgets \
      printsupport

INCLUDEPATH = $$PWD/include

HEADERS = \
include/Cvn/Apputilqt/Player.h \
include/Cvn/Apputilqt/PlayerControls.h \
include/Cvn/Apputilqt/PlaylistModel.h \
include/Cvn/Apputilqt/VideoWidget.h \
include/Cvn/Apputilqt/HistogramWidget.h \
include/Cvn/Apputilqt/QAudioLevel.h \
include/Cvn/Apputilqt/ImageViewer.h \
include/Cvn/Apputilqt/DirView.h \
include/Cvn/Apputilqt/ImageListModel.h \
include/Cvn/Apputilqt/FocusWidget.h \
    src/imageitemdelegate.h
#include/Cvn/Apputilqt/QtApplication.h \
#include/Cvn/Apputilqt/QtScheduler.h \
#include/Cvn/Apputilqt/QtRx.h \
#src/AppWidget.h


SOURCES = \
src/DirView.cpp \
src/Player.cpp \
src/PlaylistModel.cpp \
src/PlayerControls.cpp \
src/VideoWidget.cpp \
src/HistogramWidget.cpp \
src/ImageViewer.cpp \
    src/apputilqtwidget.cpp \
    src/ImageListModel.cpp \
    src/imageitemdelegate.cpp

#src/AppWidget.cpp \
#src/QtApplication.cpp \
#src/QtScheduler.cpp \
#src/QtRx.cpp \
