#include "RespartidoEquipo.h"
#include <iostream>
using namespace std;

// Constructor por defecto
// Reserva memoria para los 11 índices y las 11 stats
ResPartidoEquipo::ResPartidoEquipo() {
    golesFavor = 0;
    golesContra = 0;
    posesion = 0.0f;
    // Arreglo de 11 índices, uno por cada jugador convocado
    indicesJugadores = new int[NUM_CONVOCADOS];
    // Arreglo de 11 stats, una por cada jugador convocado
    statsJugadores = new EstadisticasJugador[NUM_CONVOCADOS];
    // Inicializa todos los índices en -1
    // -1 significa que aún no se asignó ningún jugador
    for (int i = 0; i < NUM_CONVOCADOS; i++) {
        indicesJugadores[i] = -1;
    }
}

// Constructor de copia
ResPartidoEquipo::ResPartidoEquipo(const ResPartidoEquipo& origen) {
    golesFavor = origen.golesFavor;
    golesContra = origen.golesContra;
    posesion = origen.posesion;
    indicesJugadores = new int[NUM_CONVOCADOS];
    statsJugadores = new EstadisticasJugador[NUM_CONVOCADOS];
    for (int i = 0; i < NUM_CONVOCADOS; i++) {
        indicesJugadores[i] = origen.indicesJugadores[i];
        statsJugadores[i] = origen.statsJugadores[i];
    }
}

// Destructor
ResPartidoEquipo::~ResPartidoEquipo() {
    delete[] indicesJugadores;
    delete[] statsJugadores;
}

// Operador de asignación
ResPartidoEquipo& ResPartidoEquipo::operator=(const ResPartidoEquipo& origen) {
    if (this != &origen) {
        // Libera memoria antigua
        delete[] indicesJugadores;
        delete[] statsJugadores;
        // Copia valores simples
        golesFavor = origen.golesFavor;
        golesContra = origen.golesContra;
        posesion = origen.posesion;
        // Reserva nueva memoria y copia arreglos
        indicesJugadores = new int[NUM_CONVOCADOS];
        statsJugadores = new EstadisticasJugador[NUM_CONVOCADOS];
        for (int i = 0; i < NUM_CONVOCADOS; i++) {
            indicesJugadores[i] = origen.indicesJugadores[i];
            statsJugadores[i] = origen.statsJugadores[i];
        }
    }
    return *this;
}

// Getters
int ResPartidoEquipo::getGolesFavor() const { return golesFavor; }
int ResPartidoEquipo::getGolesContra() const { return golesContra; }
float ResPartidoEquipo::getPosesion() const { return posesion; }

// Devuelve el índice del jugador en la posición pos del arreglo de convocados
int ResPartidoEquipo::getIndiceJugador(int pos) const {
    return indicesJugadores[pos];
}

// Devuelve las stats del jugador en la posición pos
EstadisticasJugador& ResPartidoEquipo::getStatsJugador(int pos) {
    return statsJugadores[pos];
}
const EstadisticasJugador& ResPartidoEquipo::getStatsJugador(int pos) const {
    return statsJugadores[pos];
}

// Setters
void ResPartidoEquipo::setGolesFavor(int g) { golesFavor = g; }
void ResPartidoEquipo::setGolesContra(int g) { golesContra = g; }
void ResPartidoEquipo::setPosesion(float p) { posesion = p; }

// Asigna el índice de un jugador convocado en la posición pos
void ResPartidoEquipo::setIndiceJugador(int pos, int indice) {
    indicesJugadores[pos] = indice;
}

// Reinicia todo a cero para reutilizar en un nuevo partido
void ResPartidoEquipo::reiniciar() {
    golesFavor = 0;
    golesContra = 0;
    posesion = 0.0f;
    for (int i = 0; i < NUM_CONVOCADOS; i++) {
        indicesJugadores[i] = -1;
        statsJugadores[i].reiniciar();
    }
}

// Imprime el resultado del equipo en este partido
void ResPartidoEquipo::imprimir() const {
    cout << "Goles: " << golesFavor
         << " | Posesion: " << posesion << "%"
         << endl;
    cout << "Jugadores convocados (indices): ";
    for (int i = 0; i < NUM_CONVOCADOS; i++) {
        cout << indicesJugadores[i] << " ";
    }
    cout << endl;
}
