#ifndef JUGADOR_H
#define JUGADOR_H

#include "EstadisticasJugador.h"

class Jugador {
private:
    char* nombre;
    char* apellido;
    int numeroCamiseta;
    EstadisticasJugador statsHistoricas;

public:
    // Constructores
    Jugador();
    Jugador(const char* nom, const char* ape, int camiseta);
    Jugador(const Jugador& origen);

    // Destructor - libera la memoria de nombre y apellido
    ~Jugador();

    // Operador de asignación
    Jugador& operator=(const Jugador& origen);

    // Operador de comparación por número de camiseta
    bool operator==(const Jugador& origen) const;

    // Getters
    const char* getNombre() const;
    const char* getApellido() const;
    int getNumeroCamiseta() const;
    EstadisticasJugador& getStats();
    const EstadisticasJugador& getStats() const;

    // Setters
    void setNombre(const char* nom);
    void setApellido(const char* ape);
    void setNumeroCamiseta(int num);

    // Imprime los datos del jugador en consola
    void imprimir() const;
};

#endif // JUGADOR_H
