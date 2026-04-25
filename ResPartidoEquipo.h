#ifndef RESPARTIDOEQUIPO_H
#define RESPARTIDOEQUIPO_H

#include "EstadisticasJugador.h"

#define NUM_CONVOCADOS 11

class ResPartidoEquipo {
private:
    int golesFavor;
    int golesContra;
    float posesion;
    // Índices de los 11 jugadores convocados
    // dentro del arreglo de jugadores del equipo
    int* indicesJugadores;
    // Stats de cada jugador durante este partido
    EstadisticasJugador* statsJugadores;

public:
    // Constructores
    ResPartidoEquipo();
    ResPartidoEquipo(const ResPartidoEquipo& origen);

    // Destructor
    ~ResPartidoEquipo();

    // Operador de asignación
    ResPartidoEquipo& operator=(const ResPartidoEquipo& origen);

    // Getters
    int getGolesFavor() const;
    int getGolesContra() const;
    float getPosesion() const;
    int getIndiceJugador(int pos) const;
    EstadisticasJugador& getStatsJugador(int pos);
    const EstadisticasJugador& getStatsJugador(int pos) const;

    // Setters
    void setGolesFavor(int g);
    void setGolesContra(int g);
    void setPosesion(float p);
    void setIndiceJugador(int pos, int indice);

    // Reinicia todo a cero para un nuevo partido
    void reiniciar();

    // Imprime el resultado del equipo en este partido
    void imprimir() const;
};

#endif // RESPARTIDOEQUIPO_H
