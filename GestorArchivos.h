#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H

#include "equipo.h"

class GestorArchivos {
public:
    GestorArchivos();

    int leerCSV(const char* ruta, Equipo* equipos, int maxEquipos);
    void guardarDatos(const char* ruta, Equipo* equipos, int numEquipos);
    void guardarJugadores(const char* ruta, Equipo* equipos, int numEquipos);
    void cargarJugadores(const char* ruta, Equipo* equipos, int numEquipos);
};

#endif // GESTORARCHIVOS_H
