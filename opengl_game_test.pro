QT       += core gui openglwidgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lopengl32

SOURCES += \
    camera.cpp \
    cameracontroller.cpp \
    main.cpp \
    objectgroup.cpp \
    oglwidget.cpp \
    sceneobject.cpp \
    skybox.cpp \
    widget.cpp

HEADERS += \
    Textures.h \
    Transfom.h \
    camera.h \
    cameracontroller.h \
    objectgroup.h \
    oglwidget.h \
    sceneobject.h \
    skybox.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    models.qrc \
    shaders.qrc \
    skyboxes.qrc \
    textures.qrc
