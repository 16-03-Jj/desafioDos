#include "EstadisticasJugador.h"

// Pone todos los atributos en cero al crear el objeto
EstadisticasJugador::EstadisticasJugador() {
    goles = 0;
    asistencias = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
    faltas = 0;
    minutosJugados = 0;
    partidosJugados = 0;
}

// Copia todos los atributos del objeto origen a este objeto
EstadisticasJugador::EstadisticasJugador(const EstadisticasJugador& origen) {
    goles = origen.goles;
    asistencias = origen.asistencias;
    tarjetasAmarillas = origen.tarjetasAmarillas;
    tarjetasRojas = origen.tarjetasRojas;
    faltas = origen.faltas;
    minutosJugados = origen.minutosJugados;
    partidosJugados = origen.partidosJugados;
}

// Igual que el constructor de copia pero se usa con el signo =
EstadisticasJugador& EstadisticasJugador::operator=(const EstadisticasJugador& origen) {
    // this != &origen evita que el objeto se copie a sí mismo
    if (this != &origen) {
        goles = origen.goles;
        asistencias = origen.asistencias;
        tarjetasAmarillas = origen.tarjetasAmarillas;
        tarjetasRojas = origen.tarjetasRojas;
        faltas = origen.faltas;
        minutosJugados = origen.minutosJugados;
        partidosJugados = origen.partidosJugados;
    }
    return *this;
}

// Getters: simplemente devuelven el valor del atributo privado
int EstadisticasJugador::getGoles() const { return goles; }
int EstadisticasJugador::getAsistencias() const { return asistencias; }
int EstadisticasJugador::getTarjetasAmarillas() const { return tarjetasAmarillas; }
int EstadisticasJugador::getTarjetasRojas() const { return tarjetasRojas; }
int EstadisticasJugador::getFaltas() const { return faltas; }
int EstadisticasJugador::getMinutosJugados() const { return minutosJugados; }
int EstadisticasJugador::getPartidosJugados() const { return partidosJugados; }

// Setters: asignan un nuevo valor al atributo privado
void EstadisticasJugador::setGoles(int g) { goles = g; }
void EstadisticasJugador::setAsistencias(int a) { asistencias = a; }
void EstadisticasJugador::setTarjetasAmarillas(int ta) { tarjetasAmarillas = ta; }
void EstadisticasJugador::setTarjetasRojas(int tr) { tarjetasRojas = tr; }
void EstadisticasJugador::setFaltas(int f) { faltas = f; }
void EstadisticasJugador::setMinutosJugados(int m) { minutosJugados = m; }
void EstadisticasJugador::setPartidosJugados(int p) { partidosJugados = p; }

// Suma las stats de un partido específico al acumulado histórico
// Se llama justo después de que termina cada partido
void EstadisticasJugador::acumular(const EstadisticasJugador& statsPartido) {
    goles += statsPartido.goles;
    asistencias += statsPartido.asistencias;
    tarjetasAmarillas += statsPartido.tarjetasAmarillas;
    tarjetasRojas += statsPartido.tarjetasRojas;
    faltas += statsPartido.faltas;
    minutosJugados += statsPartido.minutosJugados;
    partidosJugados += statsPartido.partidosJugados;
}

// Reinicia todo a cero, útil para las stats por partido
void EstadisticasJugador::reiniciar() {
    goles = 0;
    asistencias = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
    faltas = 0;
    minutosJugados = 0;
    partidosJugados = 0;
}
