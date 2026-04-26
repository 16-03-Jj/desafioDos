#include "Medidor.h"
#include "jugador.h"
#include "equipo.h"
#include "partido.h"
#include "grupo.h"
#include <iostream>
using namespace std;

Medidor::Medidor() {
    iteraciones = 0;
}

void Medidor::reset() {
    iteraciones = 0;
}

void Medidor::contarIteracion(int cantidad) {
    iteraciones += cantidad;
}

long long Medidor::getIteraciones() const {
    return iteraciones;
}

// Calcula el total de bytes ocupados por las estructuras principales
long long Medidor::calcularMemoria(int numEquipos,
                                   int numGrupos,
                                   int numPartidosR16,
                                   int numPartidosR8,
                                   int numPartidosQF,
                                   int numPartidosSF) const {
    long long total = 0;

    // Memoria por cada jugador
    long long memJugador = sizeof(Jugador) + sizeof(EstadisticasJugador);
    // nombre y apellido: promedio 15 chars cada uno
    memJugador += 15 + 15;

    // Memoria por cada equipo
    long long memEquipo = sizeof(Equipo)
                          + sizeof(EstadisticasEquipo)
                          + (26 * memJugador)  // 26 jugadores
                          + 20 + 20 + 30;      // pais, conf, dt

    // Memoria por cada ResPartidoEquipo
    long long memResPartido = sizeof(ResPartidoEquipo)
                              + (11 * sizeof(int))   // indicesJugadores
                              + (11 * sizeof(EstadisticasJugador)); // statsJugadores

    // Memoria por cada partido
    long long memPartido = sizeof(Partido)
                           + 12 + 6 + 30      // fecha, hora, sede
                           + (3 * 15)         // 3 árbitros
                           + (2 * memResPartido); // resEquipo1 y resEquipo2

    // Memoria por cada grupo
    long long memGrupo = sizeof(Grupo)
                         + (6 * memPartido);  // 6 partidos por grupo

    // Total
    total += numEquipos * memEquipo;
    total += numGrupos * memGrupo;
    total += numPartidosR16 * memPartido;
    total += numPartidosR8 * memPartido;
    total += numPartidosQF * memPartido;
    total += numPartidosSF * memPartido;
    total += 2 * memPartido; // final y tercer puesto

    return total;
}

void Medidor::imprimirReporte(int numEquipos, int numGrupos) const {
    cout << "\n--- Medicion de recursos ---" << endl;
    cout << "Iteraciones realizadas: " << iteraciones << endl;
    long long mem = calcularMemoria(numEquipos, numGrupos, 16, 8, 4, 2);
    cout << "Memoria consumida: " << mem << " bytes ("
         << mem / 1024 << " KB)" << endl;
    cout << "----------------------------" << endl;
}
