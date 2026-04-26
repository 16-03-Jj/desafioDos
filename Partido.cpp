#include "partido.h"
#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;

// Constructor por defecto
Partido::Partido() {
    fecha = new char[12];
    hora = new char[6];
    sede = new char[30];
    strncpy(fecha, "00/00/0000", 11);
    strncpy(hora, "00:00", 6);
    strncpy(sede, "nombreSede", 11);
    for (int i = 0; i < 3; i++) {
        arbitros[i] = new char[15];
        strncpy(arbitros[i], "codArbitro", 11);
    }
    equipo1 = nullptr;
    equipo2 = nullptr;
    fueProrroga = false;
    simulado = false;
    esEliminatoria = false;
}

// Constructor de copia
Partido::Partido(const Partido& origen) {
    fecha = new char[strlen(origen.fecha) + 1];
    strncpy(fecha, origen.fecha, strlen(origen.fecha) + 1);
    hora = new char[strlen(origen.hora) + 1];
    strncpy(hora, origen.hora, strlen(origen.hora) + 1);
    sede = new char[strlen(origen.sede) + 1];
    strncpy(sede, origen.sede, strlen(origen.sede) + 1);
    for (int i = 0; i < 3; i++) {
        arbitros[i] = new char[strlen(origen.arbitros[i]) + 1];
        strncpy(arbitros[i], origen.arbitros[i],
                strlen(origen.arbitros[i]) + 1);
    }
    equipo1 = origen.equipo1;
    equipo2 = origen.equipo2;
    resEquipo1 = origen.resEquipo1;
    resEquipo2 = origen.resEquipo2;
    fueProrroga = origen.fueProrroga;
    simulado = origen.simulado;
    esEliminatoria = origen.esEliminatoria;
}

// Destructor
Partido::~Partido() {
    delete[] fecha;
    delete[] hora;
    delete[] sede;
    for (int i = 0; i < 3; i++) {
        delete[] arbitros[i];
    }
    // NO delete de equipo1 y equipo2
    // el partido no es dueño de los equipos
}

// Operador de asignación
Partido& Partido::operator=(const Partido& origen) {
    if (this != &origen) {
        delete[] fecha;
        delete[] hora;
        delete[] sede;
        for (int i = 0; i < 3; i++) {
            delete[] arbitros[i];
        }
        fecha = new char[strlen(origen.fecha) + 1];
        strncpy(fecha, origen.fecha, strlen(origen.fecha) + 1);
        hora = new char[strlen(origen.hora) + 1];
        strncpy(hora, origen.hora, strlen(origen.hora) + 1);
        sede = new char[strlen(origen.sede) + 1];
        strncpy(sede, origen.sede, strlen(origen.sede) + 1);
        for (int i = 0; i < 3; i++) {
            arbitros[i] = new char[strlen(origen.arbitros[i]) + 1];
            strncpy(arbitros[i], origen.arbitros[i],
                    strlen(origen.arbitros[i]) + 1);
        }
        equipo1 = origen.equipo1;
        equipo2 = origen.equipo2;
        resEquipo1 = origen.resEquipo1;
        resEquipo2 = origen.resEquipo2;
        fueProrroga = origen.fueProrroga;
        simulado = origen.simulado;
        esEliminatoria = origen.esEliminatoria;
    }
    return *this;
}

// Getters
const char* Partido::getFecha() const { return fecha; }
const char* Partido::getHora() const { return hora; }
const char* Partido::getSede() const { return sede; }
Equipo* Partido::getEquipo1() const { return equipo1; }
Equipo* Partido::getEquipo2() const { return equipo2; }
ResPartidoEquipo& Partido::getResEquipo1() { return resEquipo1; }
ResPartidoEquipo& Partido::getResEquipo2() { return resEquipo2; }
const ResPartidoEquipo& Partido::getResEquipo1() const { return resEquipo1; }
const ResPartidoEquipo& Partido::getResEquipo2() const { return resEquipo2; }
bool Partido::getFueProrroga() const { return fueProrroga; }
bool Partido::getSimulado() const { return simulado; }
bool Partido::getEsEliminatoria() const { return esEliminatoria; }

// Setters
void Partido::setFecha(const char* f) {
    delete[] fecha;
    fecha = new char[strlen(f) + 1];
    strncpy(fecha, f, strlen(f) + 1);
}

void Partido::setHora(const char* h) {
    delete[] hora;
    hora = new char[strlen(h) + 1];
    strncpy(hora, h, strlen(h) + 1);
}

void Partido::setSede(const char* s) {
    delete[] sede;
    sede = new char[strlen(s) + 1];
    strncpy(sede, s, strlen(s) + 1);
}

void Partido::setArbitro(int pos, const char* cod) {
    delete[] arbitros[pos];
    arbitros[pos] = new char[strlen(cod) + 1];
    strncpy(arbitros[pos], cod, strlen(cod) + 1);
}

void Partido::setEquipo1(Equipo* e) { equipo1 = e; }
void Partido::setEquipo2(Equipo* e) { equipo2 = e; }
void Partido::setEsEliminatoria(bool e) { esEliminatoria = e; }

// Ecuación del enunciado:
// λA = λ1 * GFA + λ2 * GCB * α
// λ1=0.4, λ2=0.6, α=1.35
int Partido::calcularGolesEsperados(Equipo* atacante, Equipo* defensor) {
    float lambda1 = 0.4f;
    float lambda2 = 0.6f;
    float alpha   = 1.35f;
    float GFA = atacante->getStats().promGolesFavor();
    float GCB = defensor->getStats().promGolesContra();
    float golesEsperados = lambda1 * GFA + lambda2 * GCB * alpha;
    int resultado = (int)(golesEsperados + 0.5f);
    if (resultado < 0) resultado = 0;
    return resultado;
}

// Elige 11 jugadores aleatorios del equipo
void Partido::elegirConvocados(Equipo* equipo, ResPartidoEquipo& res) {
    bool elegido[NUM_JUGADORES] = {false};
    int convocados = 0;
    while (convocados < NUM_CONVOCADOS) {
        int indice = rand() % NUM_JUGADORES;
        if (!elegido[indice]) {
            elegido[indice] = true;
            res.setIndiceJugador(convocados, indice);
            convocados++;
        }
    }
}

void Partido::simularStatsJugadores(ResPartidoEquipo& res,
                                    int golesEquipo,
                                    int minutos) {
    int golesRestantes = golesEquipo;
    for (int i = 0; i < NUM_CONVOCADOS; i++) {
        EstadisticasJugador& stats = res.getStatsJugador(i);
        stats.setMinutosJugados(minutos);
        stats.setPartidosJugados(1);

        // Si quedan goles por repartir y es el último jugador
        // asignarle los goles restantes para garantizar que se repartan
        if (i == NUM_CONVOCADOS - 1 && golesRestantes > 0) {
            stats.setGoles(golesRestantes);
            golesRestantes = 0;
        } else if (golesRestantes > 0 && (rand() % 100) < 4) {
            stats.setGoles(1);
            golesRestantes--;
        } else {
            stats.setGoles(0);
        }

        // Amarillas
        int amarillas = 0;
        if ((rand() % 100) < 6) {
            amarillas = 1;
            if ((rand() % 1000) < 12) amarillas = 2;
        }
        stats.setTarjetasAmarillas(amarillas);
        stats.setTarjetasRojas(amarillas == 2 ? 1 : 0);

        // Faltas
        int faltas = 0;
        if ((rand() % 100) < 13) {
            faltas = 1;
            if ((rand() % 1000) < 28) {
                faltas = 2;
                if ((rand() % 1000) < 7) faltas = 3;
            }
        }
        stats.setFaltas(faltas);
    }
}

// Rompe el empate en eliminatorias usando ranking FIFA
// Mejor ranking = número más bajo = más probabilidad de ganar
// CORRECCIÓN: protección contra rankingFIFA == 0
void Partido::romperEmpate() {
    int r1 = equipo1->getRankingFIFA();
    int r2 = equipo2->getRankingFIFA();


    // Protección contra ranking 0 o negativo
    if (r1 <= 0) r1 = 1;
    if (r2 <= 0) r2 = 1;

    // total = r1 + r2
    // si rand() % total < r2 → gana equipo1
    // (r2 grande significa equipo2 tiene peor ranking → equipo1 gana más veces)
    int total = r1 + r2;
    if ((rand() % total) < r2) {
        resEquipo1.setGolesFavor(resEquipo1.getGolesFavor() + 1);
    } else {
        resEquipo2.setGolesFavor(resEquipo2.getGolesFavor() + 1);
    }
    fueProrroga = true;


}

// Actualiza históricos de jugadores y equipos al terminar el partido
void Partido::actualizarHistoricos() {
    // Actualizar jugadores equipo 1
    Jugador* jugs1 = equipo1->getJugadores();
    for (int i = 0; i < NUM_CONVOCADOS; i++) {
        int idx = resEquipo1.getIndiceJugador(i);
        jugs1[idx].getStats().acumular(resEquipo1.getStatsJugador(i));
    }


    // Actualizar jugadores equipo 2
    Jugador* jugs2 = equipo2->getJugadores();
    for (int i = 0; i < NUM_CONVOCADOS; i++) {
        int idx = resEquipo2.getIndiceJugador(i);
        jugs2[idx].getStats().acumular(resEquipo2.getStatsJugador(i));
    }

    // Determinar resultado y actualizar stats del equipo
    int g1 = resEquipo1.getGolesFavor();
    int g2 = resEquipo2.getGolesFavor();

    EstadisticasEquipo statsEq1, statsEq2;
    statsEq1.setGolesFavor(g1);
    statsEq1.setGolesContra(g2);
    statsEq2.setGolesFavor(g2);
    statsEq2.setGolesContra(g1);

    if (g1 > g2) {
        statsEq1.setGanados(1);
        statsEq2.setPerdidos(1);
    } else if (g2 > g1) {
        statsEq2.setGanados(1);
        statsEq1.setPerdidos(1);
    } else {
        statsEq1.setEmpatados(1);
        statsEq2.setEmpatados(1);
    }

    // Actualizar tarjetas y faltas del equipo sumando las de los jugadores
    int ta1 = 0, tr1 = 0, f1 = 0;
    int ta2 = 0, tr2 = 0, f2 = 0;
    for (int i = 0; i < NUM_CONVOCADOS; i++) {
        ta1 += resEquipo1.getStatsJugador(i).getTarjetasAmarillas();
        tr1 += resEquipo1.getStatsJugador(i).getTarjetasRojas();
        f1  += resEquipo1.getStatsJugador(i).getFaltas();
        ta2 += resEquipo2.getStatsJugador(i).getTarjetasAmarillas();
        tr2 += resEquipo2.getStatsJugador(i).getTarjetasRojas();
        f2  += resEquipo2.getStatsJugador(i).getFaltas();
    }
    statsEq1.setTarjetasAmarillas(ta1);
    statsEq1.setTarjetasRojas(tr1);
    statsEq1.setFaltas(f1);
    statsEq2.setTarjetasAmarillas(ta2);
    statsEq2.setTarjetasRojas(tr2);
    statsEq2.setFaltas(f2);

    equipo1->getStats().acumular(statsEq1);
    equipo2->getStats().acumular(statsEq2);


}

// Simula el partido completo
void Partido::simular() {
    if (simulado) return;

    // Protección contra punteros nulos
    if (equipo1 == nullptr || equipo2 == nullptr) {
        cout << "Error: partido sin equipos asignados" << endl;
        return;
    }


    // Paso 1: elegir convocados
    elegirConvocados(equipo1, resEquipo1);
    elegirConvocados(equipo2, resEquipo2);

    // Paso 2: calcular goles esperados con la ecuación del enunciado
    int golesEq1 = calcularGolesEsperados(equipo1, equipo2);
    int golesEq2 = calcularGolesEsperados(equipo2, equipo1);
    resEquipo1.setGolesFavor(golesEq1);
    resEquipo1.setGolesContra(golesEq2);
    resEquipo2.setGolesFavor(golesEq2);
    resEquipo2.setGolesContra(golesEq1);

    // Paso 3: calcular posesión proporcional al ranking FIFA inverso
    // CORRECCIÓN: protección contra rankingFIFA == 0
    int r1 = equipo1->getRankingFIFA();
    int r2 = equipo2->getRankingFIFA();
    if (r1 <= 0) r1 = 1;
    if (r2 <= 0) r2 = 1;
    float inv1 = 1.0f / r1;
    float inv2 = 1.0f / r2;
    float pos1 = (inv1 / (inv1 + inv2)) * 100.0f;
    resEquipo1.setPosesion(pos1);
    resEquipo2.setPosesion(100.0f - pos1);

    // Paso 4: si es eliminatoria y hay empate → romper con ranking FIFA
    int minutos = 90;
    if (esEliminatoria && golesEq1 == golesEq2) {
        romperEmpate();
        minutos = 120;
    }

    // Paso 5: simular stats de jugadores
    simularStatsJugadores(resEquipo1, resEquipo1.getGolesFavor(), minutos);
    simularStatsJugadores(resEquipo2, resEquipo2.getGolesFavor(), minutos);

    // Paso 6: actualizar históricos
    actualizarHistoricos();

    simulado = true;


}

// Imprime toda la información del partido
void Partido::imprimir() const {
    cout << "Fecha: " << fecha
         << " | Hora: " << hora
                   << " | Sede: "<< sede << endl;
    cout << equipo1->getPais()
         << " vs "
         << equipo2->getPais() << endl;
    cout << "Resultado: "
         << resEquipo1.getGolesFavor()
         << " - "
         << resEquipo2.getGolesFavor();
    if (fueProrroga) cout << "(Prorroga)";
    cout << endl;
    cout << "Posesion: "
         << equipo1->getPais() << " "
         << resEquipo1.getPosesion() << " % | "
              << equipo2->getPais() << " "
              << resEquipo2.getPosesion() << " %"
              << endl;
    // Mostrar goleadores equipo 1
    cout << "Goleadores " << equipo1->getPais() << ": ";
    bool hayGoles1 = false;
    for (int i = 0; i < NUM_CONVOCADOS; i++) {
        int idx = resEquipo1.getIndiceJugador(i);
        if (idx < 0 || idx >= NUM_JUGADORES) continue; // protección
        if (resEquipo1.getStatsJugador(i).getGoles() > 0) {
            cout << "#" << equipo1->getJugadores()[idx].getNumeroCamiseta() << " ";
            hayGoles1 = true;
        }
    }
    if (!hayGoles1) cout << "ninguno";
    cout << endl;

    // Mostrar goleadores equipo 2
    cout << "Goleadores " << equipo2->getPais() << ": ";
    bool hayGoles2 = false;
    for (int i = 0; i < NUM_CONVOCADOS; i++) {
        int idx = resEquipo2.getIndiceJugador(i);
        if (idx < 0 || idx >= NUM_JUGADORES) continue; // protección
        if (resEquipo2.getStatsJugador(i).getGoles() > 0) {
            cout << "#" << equipo2->getJugadores()[idx].getNumeroCamiseta() << " ";
            hayGoles2 = true;
        }
    }
    if (!hayGoles2) cout << "ninguno";
    cout << endl;
}

