#include "Jugador.h"
#include <cstring>
#include <iostream>
using namespace std;

// Constructor por defecto
Jugador::Jugador() {
    nombre = new char[10];
    apellido = new char[12]; // “sinApellido” = 11 chars + ‘\0’
    strncpy(nombre, "sinNombre", 10);
    strncpy(apellido, "sinApellido", 12);
    numeroCamiseta = 0;
}

// Constructor con parámetros
Jugador::Jugador(const char* nom, const char* ape, int camiseta) {
    nombre = new char[strlen(nom) + 1];
    strncpy(nombre, nom, strlen(nom) + 1);
    apellido = new char[strlen(ape) + 1];
    strncpy(apellido, ape, strlen(ape) + 1);
    numeroCamiseta = camiseta;
}

// Constructor de copia
Jugador::Jugador(const Jugador& origen) {
    nombre = new char[strlen(origen.nombre) + 1];
    strncpy(nombre, origen.nombre, strlen(origen.nombre) + 1);
    apellido = new char[strlen(origen.apellido) + 1];
    strncpy(apellido, origen.apellido, strlen(origen.apellido) + 1);
    numeroCamiseta = origen.numeroCamiseta;
    statsHistoricas = origen.statsHistoricas;
}

// Destructor
Jugador::~Jugador() {
    delete[] nombre;
    delete[] apellido;
}

// Operador de asignación
Jugador& Jugador::operator=(const Jugador& origen) {
    if (this != &origen) {
        delete[] nombre;
        delete[] apellido;
        nombre = new char[strlen(origen.nombre) + 1];
        strncpy(nombre, origen.nombre, strlen(origen.nombre) + 1);
        apellido = new char[strlen(origen.apellido) + 1];
        strncpy(apellido, origen.apellido, strlen(origen.apellido) + 1);
        numeroCamiseta = origen.numeroCamiseta;
        statsHistoricas = origen.statsHistoricas;
    }
    return *this;
}

// Compara dos jugadores por número de camiseta
bool Jugador::operator==(const Jugador& origen) const {
    return numeroCamiseta == origen.numeroCamiseta;
}

// Getters
const char* Jugador::getNombre() const { return nombre; }
const char* Jugador::getApellido() const { return apellido; }
int Jugador::getNumeroCamiseta() const { return numeroCamiseta; }
EstadisticasJugador& Jugador::getStats() { return statsHistoricas; }
const EstadisticasJugador& Jugador::getStats() const { return statsHistoricas; }

// Setters
void Jugador::setNombre(const char* nom) {
    delete[] nombre;
    nombre = new char[strlen(nom) + 1];
    strncpy(nombre, nom, strlen(nom) + 1);
}

void Jugador::setApellido(const char* ape) {
    delete[] apellido;
    apellido = new char[strlen(ape) + 1];
    strncpy(apellido, ape, strlen(ape) + 1);
}

void Jugador::setNumeroCamiseta(int num) { numeroCamiseta = num; }

// Imprime toda la info del jugador en consola
void Jugador::imprimir() const {
    cout << "Camiseta #" << numeroCamiseta
         << " | " << nombre << " "  << apellido
            << " | Goles: " << statsHistoricas.getGoles()
                    << " | Partidos: " << statsHistoricas.getPartidosJugados()
                       << endl;
}
