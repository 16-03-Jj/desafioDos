QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Equipo.cpp \
        EstadisticasEquipo.cpp \
        EstadisticasJugador.cpp \
        GestorArchivos.cpp \
        Grupo.cpp \
        Jugador.cpp \
        Partido.cpp \
        ResPartidoEquipo.cpp \
        Torneo.cpp \
        main.cpp \
        medidor.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Equipo.h \
    EstadisticasEquipo.h \
    EstadisticasJugador.h \
    GestorArchivos.h \
    Grupo.h \
    Jugador.h \
    Partido.h \
    ResPartidoEquipo.h \
    Torneo.h \
    medidor.h
