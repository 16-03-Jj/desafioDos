#ifndef GRUPO_H
#define GRUPO_H

#include "partido.h"

#define NUM_EQUIPOS_GRUPO 4
#define NUM_PARTIDOS_GRUPO 6

class Grupo {
private:
    char letra;
    Equipo* equipos[NUM_EQUIPOS_GRUPO];
    Partido partidos[NUM_PARTIDOS_GRUPO];
    int puntos[NUM_EQUIPOS_GRUPO];
    int ordenClasificacion[NUM_EQUIPOS_GRUPO];

public:
    // Constructores
    Grupo();
    Grupo(const Grupo& origen);
    Grupo& operator=(const Grupo& origen);

    // Operador []
    Equipo* operator[](int indice) const;

    // Getters
    char getLetra() const;
    Equipo* getEquipo(int indice) const;
    Partido& getPartido(int indice);
    int getPuntos(int indice) const;
    int getOrdenClasificacion(int indice) const;

    // Setters
    void setLetra(char l);
    void setEquipo(int indice, Equipo* equipo);

    // Métodos principales
    void configurarPartidos(int fechaInicio);
    void simularPartidos();
    void calcularPuntos();
    void clasificar();
    void imprimirTabla() const;
};

#endif // GRUPO_H
