TEMPLATE = lib

TARGET = 

DEPENDPATH += .

QT += opengl

#CONFIG += debug
CONFIG += staticlib

# Input
SOURCES += \
CProcessingCore.cpp \
CProcessing.cpp \
CProcessingTimer.cpp \
CProcessingWindow3D.cpp \
CProcessingWindow.cpp \
CQImageFilter.cpp \
CQGLControl.cpp \
CGLTexture.cpp \
CGLUtil.cpp \

HEADERS += \
../include/Array1D.h \
../include/Array2D.h \
../include/Array3D.h \
../include/ArrayList.h \
../include/CProcessingCore.h \
../include/Interface.h \
../include/IRefPtr.h \
../include/PObject.h \
../include/PVector.h \
../include/String.h \
CProcessingTimer.h \
CProcessingWindow3D.h \
CProcessingWindow.h \
Dimension.h \
LinkedList.h \
Point.h \
Polygon.h \
Rectangle.h \
RefPtr.h \
CQImageFilter.h \
CQGLControl.h \
CGLTexture.h \
CGLUtil.h \

OBJECTS_DIR = ../obj

DESTDIR = ../lib

INCLUDEPATH += \
../include \
../../CMath/include \
../../CFile/include \
../../CFont/include \
../../CImageLib/include \
../../CThread/include \
../../COS/include \
../../CStrUtil/include \
../../CRGBName/include \
../../CUtil/include \
.
