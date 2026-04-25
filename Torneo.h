#ifndef TORNEO_H
#define TORNEO_H

#include "Grupo.h"

#define NUM_EQUIPOS_TORNEO 48
#define NUM_GRUPOS 12
#define NUM_BOMBOS 4
#define EQUIPOS_POR_BOMBO 12

class Torneo {
private:
    Equipo* equipos;
    Grupo* grupos;
    int numEquipos;

    // Partidos de cada etapa eliminatoria
    // R16=32, R8=16, QF=8, SF=4, 3er=1, Final=1
    Partido* partidosR16;
    Partido* partidosR8;
    Partido* partidosQF;
    Partido* partidosSF;
    Partido partidoTercero;
    Partido partidoFinal;

public:
    // Constructor y destructor
    Torneo();
    ~Torneo();

    // I - Carga de datos desde CSV
    void cargarDatos(const char* rutaCSV);

    // II - Conformación de grupos
    void conformarGrupos();

    // III - Simulación de etapas
    void simularFaseGrupos();
    void simularR16();
    void simularR8();
    void simularQF();
    void simularSF();
    void simularFinal();

    // IV - Estadísticas finales
    void imprimirEstadisticas() const;

    // Imprime todos los grupos conformados
    void imprimirGrupos() const;

private:
    // Ordena los equipos por ranking FIFA (burbuja)
    void ordenarPorRanking();

    // Arma los 4 bombos del sorteo
    void armarBombos(int bombos[NUM_BOMBOS][EQUIPOS_POR_BOMBO],
                     int& indiceUSA);

    // Verifica si dos equipos pueden estar en el mismo grupo
    bool puedenCompartirGrupo(Equipo* eq1, Equipo* eq2, Grupo& grupo);

    // Configura los partidos de R16 según tabla de clasificación
    void configurarR16();

    // Configura partidos de etapas siguientes según ganadores
    void configurarSiguienteEtapa(Partido* etapaActual, int numPartidos,
                                  Partido* etapaSiguiente);
};

#endif // TORNEO_H
