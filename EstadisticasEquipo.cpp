#include "EstadisticasEquipo.h"

// Inicializa todo en cero
EstadisticasEquipo::EstadisticasEquipo() {
    golesFavor = 0;
    golesContra = 0;
    ganados = 0;
    empatados = 0;
    perdidos = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
    faltas = 0;
}

// Copia todos los atributos del objeto origen
EstadisticasEquipo::EstadisticasEquipo(const EstadisticasEquipo& origen) {
    golesFavor = origen.golesFavor;
    golesContra = origen.golesContra;
    ganados = origen.ganados;
    empatados = origen.empatados;
    perdidos = origen.perdidos;
    tarjetasAmarillas = origen.tarjetasAmarillas;
    tarjetasRojas = origen.tarjetasRojas;
    faltas = origen.faltas;
}

// Se usa cuando haces estadisticas1 = estadisticas2
EstadisticasEquipo& EstadisticasEquipo::operator=(const EstadisticasEquipo& origen) {
    if (this != &origen) {
        golesFavor = origen.golesFavor;
        golesContra = origen.golesContra;
        ganados = origen.ganados;
        empatados = origen.empatados;
        perdidos = origen.perdidos;
        tarjetasAmarillas = origen.tarjetasAmarillas;
        tarjetasRojas = origen.tarjetasRojas;
        faltas = origen.faltas;
    }
    return *this;
}

// Getters
int EstadisticasEquipo::getGolesFavor() const { return golesFavor; }
int EstadisticasEquipo::getGolesContra() const { return golesContra; }
int EstadisticasEquipo::getGanados() const { return ganados; }
int EstadisticasEquipo::getEmpatados() const { return empatados; }
int EstadisticasEquipo::getPerdidos() const { return perdidos; }
int EstadisticasEquipo::getTarjetasAmarillas() const { return tarjetasAmarillas; }
int EstadisticasEquipo::getTarjetasRojas() const { return tarjetasRojas; }
int EstadisticasEquipo::getFaltas() const { return faltas; }

// Setters
void EstadisticasEquipo::setGolesFavor(int g) { golesFavor = g; }
void EstadisticasEquipo::setGolesContra(int g) { golesContra = g; }
void EstadisticasEquipo::setGanados(int g) { ganados = g; }
void EstadisticasEquipo::setEmpatados(int e) { empatados = e; }
void EstadisticasEquipo::setPerdidos(int p) { perdidos = p; }
void EstadisticasEquipo::setTarjetasAmarillas(int ta) { tarjetasAmarillas = ta; }
void EstadisticasEquipo::setTarjetasRojas(int tr) { tarjetasRojas = tr; }
void EstadisticasEquipo::setFaltas(int f) { faltas = f; }

// Suma las stats de un partido al acumulado histórico del equipo
void EstadisticasEquipo::acumular(const EstadisticasEquipo& statsPartido) {
    golesFavor += statsPartido.golesFavor;
    golesContra += statsPartido.golesContra;
    ganados += statsPartido.ganados;
    empatados += statsPartido.empatados;
    perdidos += statsPartido.perdidos;
    tarjetasAmarillas += statsPartido.tarjetasAmarillas;
    tarjetasRojas += statsPartido.tarjetasRojas;
    faltas += statsPartido.faltas;
}

// Calcula el promedio de goles a favor por partido
// Se usa en la ecuación: lambda1 * promGFA + lambda2 * promGCB
float EstadisticasEquipo::promGolesFavor() const {
    int partidos = ganados + empatados + perdidos;
    if (partidos == 0) return 0.0f;
    return (float)golesFavor / partidos;
}

// Calcula el promedio de goles en contra por partido
float EstadisticasEquipo::promGolesContra() const {
    int partidos = ganados + empatados + perdidos;
    if (partidos == 0) return 0.0f;
    return (float)golesContra / partidos;
}
