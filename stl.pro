QT +=opengl
HEADERS += \
    face.h \
    model.h \
    openglwidget.h \
    mywidget.h \
    camera.h

SOURCES += \
    face.cpp \
    model.cpp \
    openglwidget.cpp \
    mywidget.cpp \
    main.cpp \
    camera.cpp

LIBS += opengl32.lib
LIBS += glu32.lib

RC_FILE +=stlico.rc
