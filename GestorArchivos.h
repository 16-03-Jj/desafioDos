#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H

#include "Equipo.h"

class GestorArchivos {
public:
    GestorArchivos();

    // Lee el CSV y llena el arreglo de equipos
    // Retorna la cantidad de equipos leídos
    int leerCSV(const char* ruta, Equipo* equipos, int maxEquipos);

    // Guarda las estadísticas actualizadas de todos los equipos
    void guardarDatos(const char* ruta, Equipo* equipos, int numEquipos);
};

#endif // GESTORARCHIVOS_H
