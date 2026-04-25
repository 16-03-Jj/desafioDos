#ifndef EQUIPO_H
#define EQUIPO_H

#include "Jugador.h"
#include "EstadisticasEquipo.h"

#define NUM_JUGADORES 26

class Equipo {
private:
    char* pais;
    char* confederacion;
    char* directorTecnico;
    int rankingFIFA;
    Jugador* jugadores;
    EstadisticasEquipo statsHistoricas;

public:
    // Constructores
    Equipo();
    Equipo(const char* pais, const char* conf,
           const char* dt, int ranking);
    Equipo(const Equipo& origen);

    // Destructor
    ~Equipo();

    // Operador de asignación
    Equipo& operator=(const Equipo& origen);

    // Compara dos equipos por ranking FIFA
    // Se usa para ordenar y armar los bombos del sorteo
    bool operator<(const Equipo& origen) const;

    // Getters
    const char* getPais() const;
    const char* getConfederacion() const;
    const char* getDT() const;
    int getRankingFIFA() const;
    Jugador* getJugadores() const;
    EstadisticasEquipo& getStats();
    const EstadisticasEquipo& getStats() const;

    // Setters
    void setPais(const char* p);
    void setConfederacion(const char* c);
    void setDT(const char* dt);
    void setRankingFIFA(int r);

    // Crea los 26 jugadores artificialmente como pide el enunciado
    void inicializarJugadores();

    // Imprime los datos del equipo en consola
    void imprimir() const;
};

#endif // EQUIPO_H
