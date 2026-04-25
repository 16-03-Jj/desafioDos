#ifndef PARTIDO_H
#define PARTIDO_H

#include "equipo.h"
#include "respartidoequipo.h"

class Partido {
private:
    char* fecha;
    char* hora;
    char* sede;
    char* arbitros[3];
    Equipo* equipo1;
    Equipo* equipo2;
    ResPartidoEquipo resEquipo1;
    ResPartidoEquipo resEquipo2;
    bool fueProrroga;
    bool simulado;
    bool esEliminatoria;

    // Métodos privados
    int calcularGolesEsperados(Equipo* atacante, Equipo* defensor);
    void elegirConvocados(Equipo* equipo, ResPartidoEquipo& res);
    void simularStatsJugadores(ResPartidoEquipo& res,
                               int golesEquipo,
                               int minutos);
    void romperEmpate();
    void actualizarHistoricos();

public:
    // Constructores
    Partido();
    Partido(const Partido& origen);

    // Destructor
    ~Partido();

    // Operador de asignación
    Partido& operator=(const Partido& origen);

    // Getters
    const char* getFecha() const;
    const char* getHora() const;
    const char* getSede() const;
    Equipo* getEquipo1() const;
    Equipo* getEquipo2() const;
    ResPartidoEquipo& getResEquipo1();
    ResPartidoEquipo& getResEquipo2();
    const ResPartidoEquipo& getResEquipo1() const;
    const ResPartidoEquipo& getResEquipo2() const;
    bool getFueProrroga() const;
    bool getSimulado() const;
    bool getEsEliminatoria() const;

    // Setters
    void setFecha(const char* f);
    void setHora(const char* h);
    void setSede(const char* s);
    void setArbitro(int pos, const char* cod);
    void setEquipo1(Equipo* e);
    void setEquipo2(Equipo* e);
    void setEsEliminatoria(bool e);

    // Simula el partido completo
    void simular();

    // Imprime la información del partido
    void imprimir() const;
};

#endif // PARTIDO_H
