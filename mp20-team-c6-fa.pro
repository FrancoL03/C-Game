QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets\
                                        multimedia

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CustomDialog.cpp \
    EnemyItem.cpp \
    GraphicView.cpp \
    HealthPackItem.cpp \
    MainWindow.cpp \
    PEnemyItem.cpp \
    ProtagonistItem.cpp \
    TileItem.cpp \
    WorldModel.cpp \
    main.cpp \
    ./Pathfinder/pathfinder.cpp \
    ./Pathfinder/PathNode.cpp

HEADERS += \
    CustomDialog.h \
    EnemyItem.h \
    GraphicView.h \
    HealthPackItem.h \
    MainWindow.h \
    PEnemyItem.h \
    ProtagonistItem.h \
    TileItem.h \
    WorldModel.h\
    ./Pathfinder/pathfinder.h \
    ./Pathfinder/PathNode.h
FORMS += \
    mainwindow.ui

RESOURCES += \
    images.qrc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/build-world-Desktop_Qt_5_15_1_MinGW_64_bit-Debug/release/ -lworld
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/build-world-Desktop_Qt_5_15_1_MinGW_64_bit-Debug/debug/ -lworld
else:unix: LIBS += -L$$PWD/build-world-Desktop_Qt_5_15_1_MinGW_64_bit-Debug/ -lworld

INCLUDEPATH += $$PWD/build-world-Desktop_Qt_5_15_1_MinGW_64_bit-Debug/debug
DEPENDPATH += $$PWD/build-world-Desktop_Qt_5_15_1_MinGW_64_bit-Debug/debug
