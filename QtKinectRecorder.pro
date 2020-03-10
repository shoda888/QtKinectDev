#-------------------------------------------------
#
# Project created by QtCreator 2016-12-02T19:33:09
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtKinectRecorder_V5
TEMPLATE = app
#DEFINES += Yamamoto





SOURCES += main.cpp\
        MainWindow.cpp \
    ImageRenderWidget.cpp \
    ImageSave.cpp \
    PosDataThread.cpp \
    DepthDataSaveFunc.cpp

HEADERS  += MainWindow.h \
    ImageRenderWidget.h \
    ImageSave.h \
    PosDataThread.h \
    DepthDataSaveFunc.h


FORMS    += MainWindow.ui \
    ImageRenderWidget.ui


INCLUDEPATH += 'C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/inc/'



LIBS += -L'C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x64/'
LIBS += -LC:/OpenCV2.4.9/build/x64/vc12/lib
LIBS += -L'C:/usr/kitamura/sMySoft/QtKinectRecorder/'

#LIBS += -L'C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x86/'
#LIBS += -LC:/OpenCV2.4.9/build/x86/vc12/lib

INCLUDEPATH += 'C:/OpenCV2.4.9/build/include/'
DEPENDPATH += 'C:/OpenCV2.4.9/build/include/'

CONFIG(release, debug|release):LIBS += opengl32.lib \
                                       glu32.lib \
                                       Kinect20.lib \
                                       opencv_core249.lib \
                                       opencv_highgui249.lib \
                                      # libpng16.lib \
                                        zlib.lib

else:CONFIG(debug, debug|release): LIBS += opengl32.lib \
                                                glu32.lib \
                                                Kinect20.lib \
                                                opencv_core249d.lib \
                                                opencv_highgui249d.lib



