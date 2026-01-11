QT       += core gui widgets printsupport svg xml

CONFIG += c++17

# 设置包含路径
INCLUDEPATH += src/core \
               src/graphics \
               src/commands \
               src/models \
               src/managers \
               src/ui

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/core/conceptnode.cpp \
    src/core/conceptedge.cpp \
    src/core/conceptmap.cpp \
    src/core/conceptmapserializer.cpp \
    src/graphics/graphicsnode.cpp \
    src/graphics/graphicsedge.cpp \
    src/graphics/graphicsscene.cpp \
    src/graphics/graphicsview.cpp \
    src/commands/addnodecommand.cpp \
    src/commands/addedgecommand.cpp \
    src/commands/deletenodecommand.cpp \
    src/commands/deleteedgecommand.cpp \
    src/commands/movenodecommand.cpp \
    src/models/nodemodel.cpp \
    src/models/edgemodel.cpp \
    src/models/mapmodel.cpp \
    src/managers/stylemanager.cpp \
    src/managers/filemanager.cpp \
    src/ui/mainwindow.cpp \
    src/ui/toolbar.cpp \
    src/ui/propertypanel.cpp \
    src/ui/main.cpp

HEADERS += \
    src/core/conceptnode.h \
    src/core/conceptedge.h \
    src/core/conceptmap.h \
    src/graphics/graphicsnode.h \
    src/graphics/graphicsedge.h \
    src/graphics/graphicsscene.h \
    src/graphics/graphicsview.h \
    src/commands/addnodecommand.h \
    src/commands/addedgecommand.h \
    src/commands/deletenodecommand.h \
    src/commands/deleteedgecommand.h \
    src/commands/movenodecommand.h \
    src/models/nodemodel.h \
    src/models/edgemodel.h \
    src/models/mapmodel.h \
    src/managers/stylemanager.h \
    src/managers/filemanager.h \
    src/ui/mainwindow.h \
    src/ui/toolbar.h \
    src/ui/propertypanel.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/icons/icons.qrc
