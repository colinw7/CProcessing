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
../../../CThread/include \
../../../COS/include \
../../../CUtil/include \
.

unix:LIBS += \
-L../../lib \
-L../../../COS/lib \
-L../../../CImageLib/lib \
-L../../../CThread/lib \
-L../../../CStrUtil/lib \
-L../../../CFile/lib \
-L../../../CRGBName/lib \
-lCProcessing -lCImageLib -lCThread -lCFile -lCOS -lCStrUtil -lCRGBName \
-lglut -lGLU -lGL -lpng -ljpeg
