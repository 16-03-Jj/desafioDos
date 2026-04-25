#include "jugador.h"
#include <cstring>
#include <iostream>
using namespace std;

// Constructor por defecto
// Reserva memoria para nombre y apellido con valores vacíos
Jugador::Jugador() {
    nombre = new char[10];
    apellido = new char[10];
    strncpy(nombre, "sinNombre", 10);
    strncpy(apellido, "sinApellido", 10);
    numeroCamiseta = 0;
    // statsHistoricas se inicializa sola con su propio constructor
}

// Constructor con parámetros
// strlen(nom) + 1: el +1 es para el caracter nulo '\0' que termina todo texto en C++
Jugador::Jugador(const char* nom, const char* ape, int camiseta) {
    nombre = new char[strlen(nom) + 1];
    strncpy(nombre, nom, strlen(nom) + 1);
    apellido = new char[strlen(ape) + 1];
    strncpy(apellido, ape, strlen(ape) + 1);
    numeroCamiseta = camiseta;
}

// Constructor de copia
// Se llama cuando haces: Jugador jugador2(jugador1)
Jugador::Jugador(const Jugador& origen) {
    nombre = new char[strlen(origen.nombre) + 1];
    strncpy(nombre, origen.nombre, strlen(origen.nombre) + 1);
    apellido = new char[strlen(origen.apellido) + 1];
    strncpy(apellido, origen.apellido, strlen(origen.apellido) + 1);
    numeroCamiseta = origen.numeroCamiseta;
    statsHistoricas = origen.statsHistoricas;
}

// Destructor
// Se llama automáticamente cuando el objeto se destruye
// Libera la memoria que pedimos con new
Jugador::~Jugador() {
    delete[] nombre;
    delete[] apellido;
}

// Operador de asignación
// Se llama cuando haces: jugador2 = jugador1
Jugador& Jugador::operator=(const Jugador& origen) {
    // Verificamos que no se esté copiando a sí mismo
    if (this != &origen) {
        // Primero liberamos la memoria antigua
        delete[] nombre;
        delete[] apellido;
        // Luego reservamos nueva memoria y copiamos
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
// Se usa para buscar jugadores: if(jugador1 == jugador2)
bool Jugador::operator==(const Jugador& origen) const {
    return numeroCamiseta == origen.numeroCamiseta;
}

// Getters
const char* Jugador::getNombre() const { return nombre; }
const char* Jugador::getApellido() const { return apellido; }
int Jugador::getNumeroCamiseta() const { return numeroCamiseta; }

// Dos versiones de getStats:
// La primera se usa cuando el jugador NO es constante y necesitas modificar sus stats
// La segunda se usa cuando el jugador ES constante y solo vas a leer sus stats
EstadisticasJugador& Jugador::getStats() { return statsHistoricas; }
const EstadisticasJugador& Jugador::getStats() const { return statsHistoricas; }

// Setters
// Cada setter libera la memoria anterior y reserva nueva
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
         << " | " << nombre << " " << apellido
         << " | Goles: " << statsHistoricas.getGoles()
         << " | Partidos: " << statsHistoricas.getPartidosJugados()
         << endl;
}
