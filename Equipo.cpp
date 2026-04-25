#include "Equipo.h"
#include <cstring>
#include <iostream>
using namespace std;

// Constructor por defecto
Equipo::Equipo() {
    pais = new char[20];
    confederacion = new char[20];
    directorTecnico = new char[30];
    strncpy(pais, "sinPais", 8);
    strncpy(confederacion, "sinConf", 8);
    strncpy(directorTecnico, "sinDT", 6);
    rankingFIFA = 0;
    // Reserva memoria para los 26 jugadores
    // Cada jugador se inicializa con su propio constructor por defecto
    jugadores = new Jugador[NUM_JUGADORES];
}

// Constructor con parámetros
Equipo::Equipo(const char* p, const char* conf,
               const char* dt, int ranking) {
    pais = new char[strlen(p) + 1];
    strncpy(pais, p, strlen(p) + 1);
    confederacion = new char[strlen(conf) + 1];
    strncpy(confederacion, conf, strlen(conf) + 1);
    directorTecnico = new char[strlen(dt) + 1];
    strncpy(directorTecnico, dt, strlen(dt) + 1);
    rankingFIFA = ranking;
    jugadores = new Jugador[NUM_JUGADORES];
}

// Constructor de copia
Equipo::Equipo(const Equipo& origen) {
    pais = new char[strlen(origen.pais) + 1];
    strncpy(pais, origen.pais, strlen(origen.pais) + 1);
    confederacion = new char[strlen(origen.confederacion) + 1];
    strncpy(confederacion, origen.confederacion,
            strlen(origen.confederacion) + 1);
    directorTecnico = new char[strlen(origen.directorTecnico) + 1];
    strncpy(directorTecnico, origen.directorTecnico,
            strlen(origen.directorTecnico) + 1);
    rankingFIFA = origen.rankingFIFA;
    // Copia los 26 jugadores uno por uno
    // Aquí se usa el operator= de Jugador que ya definimos
    jugadores = new Jugador[NUM_JUGADORES];
    for (int i = 0; i < NUM_JUGADORES; i++) {
        jugadores[i] = origen.jugadores[i];
    }
    statsHistoricas = origen.statsHistoricas;
}

// Destructor
// Libera toda la memoria dinámica del equipo
Equipo::~Equipo() {
    delete[] pais;
    delete[] confederacion;
    delete[] directorTecnico;
    // Esto también llama al destructor de cada Jugador automáticamente
    delete[] jugadores;
}

// Operador de asignación
Equipo& Equipo::operator=(const Equipo& origen) {
    if (this != &origen) {
        // Libera memoria antigua
        delete[] pais;
        delete[] confederacion;
        delete[] directorTecnico;
        delete[] jugadores;
        // Reserva nueva memoria y copia
        pais = new char[strlen(origen.pais) + 1];
        strncpy(pais, origen.pais, strlen(origen.pais) + 1);
        confederacion = new char[strlen(origen.confederacion) + 1];
        strncpy(confederacion, origen.confederacion,
                strlen(origen.confederacion) + 1);
        directorTecnico = new char[strlen(origen.directorTecnico) + 1];
        strncpy(directorTecnico, origen.directorTecnico,
                strlen(origen.directorTecnico) + 1);
        rankingFIFA = origen.rankingFIFA;
        jugadores = new Jugador[NUM_JUGADORES];
        for (int i = 0; i < NUM_JUGADORES; i++) {
            jugadores[i] = origen.jugadores[i];
        }
        statsHistoricas = origen.statsHistoricas;
    }
    return *this;
}

// Devuelve true si este equipo tiene mejor ranking que el otro
// Mejor ranking = número más bajo (1 es mejor que 10)
bool Equipo::operator<(const Equipo& origen) const {
    return rankingFIFA < origen.rankingFIFA;
}

// Getters
const char* Equipo::getPais() const { return pais; }
const char* Equipo::getConfederacion() const { return confederacion; }
const char* Equipo::getDT() const { return directorTecnico; }
int Equipo::getRankingFIFA() const { return rankingFIFA; }
Jugador* Equipo::getJugadores() const { return jugadores; }
EstadisticasEquipo& Equipo::getStats() { return statsHistoricas; }
const EstadisticasEquipo& Equipo::getStats() const { return statsHistoricas; }

// Setters
void Equipo::setPais(const char* p) {
    delete[] pais;
    pais = new char[strlen(p) + 1];
    strncpy(pais, p, strlen(p) + 1);
}

void Equipo::setConfederacion(const char* c) {
    delete[] confederacion;
    confederacion = new char[strlen(c) + 1];
    strncpy(confederacion, c, strlen(c) + 1);
}

void Equipo::setDT(const char* dt) {
    delete[] directorTecnico;
    directorTecnico = new char[strlen(dt) + 1];
    strncpy(directorTecnico, dt, strlen(dt) + 1);
}

void Equipo::setRankingFIFA(int r) { rankingFIFA = r; }

// Inicializa los 26 jugadores artificialmente como pide el enunciado:
// - Nombres: "nombre1", "nombre2", ... "nombre26"
// - Apellidos: "apellido1", "apellido2", ... "apellido26"
// - Camisetas: 1, 2, 3, ... 26
// - Goles: repartidos uniformemente del total histórico del equipo
void Equipo::inicializarJugadores() {
    int golesTotal = statsHistoricas.getGolesFavor();
    // División entera: cuántos goles mínimo le tocan a cada jugador
    int golesBase = golesTotal / NUM_JUGADORES;
    // El sobrante de la división se reparte entre los primeros jugadores
    int golesExtra = golesTotal % NUM_JUGADORES;

    for (int i = 0; i < NUM_JUGADORES; i++) {
        char nombre[20], apellido[20];
        // snprintf es como printf pero guarda el texto en una variable
        snprintf(nombre, sizeof(nombre), "nombre%d", i + 1);
        snprintf(apellido, sizeof(apellido), "apellido%d", i + 1);

        jugadores[i].setNombre(nombre);
        jugadores[i].setApellido(apellido);
        jugadores[i].setNumeroCamiseta(i + 1);

        // Los primeros golesExtra jugadores reciben un gol extra
        int golesJugador = golesBase + (i < golesExtra ? 1 : 0);
        jugadores[i].getStats().setGoles(golesJugador);
    }
}

// Imprime los datos principales del equipo
void Equipo::imprimir() const {
    cout << "Pais: " << pais
         << " | Confederacion: " << confederacion
         << " | Ranking FIFA: " << rankingFIFA
         << " | DT: " << directorTecnico
         << " | Goles historicos: " << statsHistoricas.getGolesFavor()
         << endl;
}
