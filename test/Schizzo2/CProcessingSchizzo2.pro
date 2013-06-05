TEMPLATE = app

TARGET = 

DEPENDPATH += .

QT += opengl

#CONFIG += debug

# Input
SOURCES += \
CProcessingSchizzo2.cpp \

HEADERS += \

DESTDIR     = .
OBJECTS_DIR = .

INCLUDEPATH += \
../include \
../../../CProcessing/include \
../../../CImageLib/include \
../../../CMath/include \
../../../COS/include \
../../../CUtil/include \
.

unix:LIBS += \
-L../../lib \
-L../../../COS/lib \
-L../../../CImageLib/lib \
-L../../../CStrUtil/lib \
-L../../../CFile/lib \
-lCProcessing -lCImageLib -lCFile -lCOS -lCStrUtil \
-lglut -lGLU -lGL
