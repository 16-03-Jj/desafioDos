#include "torneo.h"
#include "gestorarchivos.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;

Torneo::Torneo() {
    equipos = new Equipo[NUM_EQUIPOS_TORNEO];
    grupos  = new Grupo[NUM_GRUPOS];
    numEquipos = 0;
    partidosR16 = new Partido[16];
    partidosR8  = new Partido[8];
    partidosQF  = new Partido[4];
    partidosSF  = new Partido[2];
    srand(time(0));
}

Torneo::~Torneo() {
    delete[] equipos;
    delete[] grupos;
    delete[] partidosR16;
    delete[] partidosR8;
    delete[] partidosQF;
    delete[] partidosSF;
}

void Torneo::cargarDatos(const char* rutaCSV) {
    medidor.reset();
    GestorArchivos gestor;
    numEquipos = gestor.leerCSV(rutaCSV, equipos, NUM_EQUIPOS_TORNEO);
    for (int i = 0; i < numEquipos; i++) {
        medidor.contarIteracion();
        equipos[i].inicializarJugadores();
    }
    gestor.cargarJugadores(
        "C:/Users/JUAN JOSE/OneDrive/Documentos/UdeAWorldCup/jugadores.csv",
        equipos, numEquipos);
    cout << "Datos cargados: " << numEquipos << " equipos." << endl;
    medidor.imprimirReporte(numEquipos, NUM_GRUPOS);
}

void Torneo::ordenarPorRanking() {
    for (int i = 0; i < numEquipos - 1; i++) {
        medidor.contarIteracion();
        for (int j = 0; j < numEquipos - 1 - i; j++) {
            medidor.contarIteracion();
            if (equipos[j+1] < equipos[j]) {
                Equipo temp = equipos[j];
                equipos[j] = equipos[j+1];
                equipos[j+1] = temp;
            }
        }
    }
}

void Torneo::armarBombos(int bombos[NUM_BOMBOS][EQUIPOS_POR_BOMBO], int& indiceUSA) {
    ordenarPorRanking();
    for (int i = 0; i < NUM_BOMBOS; i++) {
        for (int j = 0; j < EQUIPOS_POR_BOMBO; j++) {
            medidor.contarIteracion();
            bombos[i][j] = -1;
        }
    }
    indiceUSA = -1;
    for (int i = 0; i < numEquipos; i++) {
        medidor.contarIteracion();
        if (strcmp(equipos[i].getPais(), "United States") == 0 ||
            strcmp(equipos[i].getPais(), "USA") == 0) {
            indiceUSA = i;
            break;
        }
    }
    int contBombo[NUM_BOMBOS] = {0, 0, 0, 0};
    if (indiceUSA != -1) {
        bombos[0][0] = indiceUSA;
        contBombo[0] = 1;
    }
    int bomboActual = 0;
    for (int i = 0; i < numEquipos; i++) {
        medidor.contarIteracion();
        if (i == indiceUSA) continue;
        if (contBombo[bomboActual] >= EQUIPOS_POR_BOMBO) bomboActual++;
        if (bomboActual >= NUM_BOMBOS) break;
        bombos[bomboActual][contBombo[bomboActual]] = i;
        contBombo[bomboActual]++;
    }
}

bool Torneo::puedenCompartirGrupo(Equipo* nuevo, Equipo* eq2, Grupo& grupo) {
    int countUEFA = 0;
    int countConfNuevo = 0;
    const char* confNuevo = nuevo->getConfederacion();
    for (int i = 0; i < NUM_EQUIPOS_GRUPO; i++) {
        medidor.contarIteracion();
        Equipo* eq = grupo.getEquipo(i);
        if (eq == nullptr) continue;
        if (strcmp(eq->getConfederacion(), confNuevo) == 0) countConfNuevo++;
        if (strcmp(eq->getConfederacion(), "UEFA") == 0) countUEFA++;
    }
    if (strcmp(confNuevo, "UEFA") == 0 && countUEFA >= 2) return false;
    if (strcmp(confNuevo, "UEFA") != 0 && countConfNuevo >= 1) return false;
    return true;
}

void Torneo::conformarGrupos() {
    medidor.reset();
    int bombos[NUM_BOMBOS][EQUIPOS_POR_BOMBO];
    int indiceUSA = -1;
    armarBombos(bombos, indiceUSA);
    char letras[NUM_GRUPOS] = {'A','B','C','D','E','F','G','H','I','J','K','L'};
    for (int i = 0; i < NUM_GRUPOS; i++) {
        medidor.contarIteracion();
        grupos[i].setLetra(letras[i]);
    }
    bool asignado[NUM_EQUIPOS_TORNEO] = {false};
    for (int bombo = 0; bombo < NUM_BOMBOS; bombo++) {
        medidor.contarIteracion();
        for (int g = 0; g < NUM_GRUPOS; g++) {
            medidor.contarIteracion();
            bool encontrado = false;
            for (int k = EQUIPOS_POR_BOMBO - 1; k > 0; k--) {
                medidor.contarIteracion();
                int r = rand() % (k + 1);
                int temp = bombos[bombo][k];
                bombos[bombo][k] = bombos[bombo][r];
                bombos[bombo][r] = temp;
            }
            for (int e = 0; e < EQUIPOS_POR_BOMBO && !encontrado; e++) {
                medidor.contarIteracion();
                int idx = bombos[bombo][e];
                if (idx == -1) continue;
                if (!asignado[idx]) {
                    Equipo* candidato = &equipos[idx];
                    if (puedenCompartirGrupo(candidato, nullptr, grupos[g])) {
                        grupos[g].setEquipo(bombo, candidato);
                        asignado[idx] = true;
                        encontrado = true;
                    }
                }
            }
            if (!encontrado) {
                for (int e = 0; e < EQUIPOS_POR_BOMBO && !encontrado; e++) {
                    medidor.contarIteracion();
                    int idx = bombos[bombo][e];
                    if (idx == -1) continue;
                    if (!asignado[idx]) {
                        grupos[g].setEquipo(bombo, &equipos[idx]);
                        asignado[idx] = true;
                        encontrado = true;
                    }
                }
            }
        }
    }
    medidor.imprimirReporte(numEquipos, NUM_GRUPOS);
}

void Torneo::simularFaseGrupos() {
    medidor.reset();
    for (int i = 0; i < NUM_GRUPOS; i++) {
        medidor.contarIteracion();
        grupos[i].configurarPartidos(20);
        grupos[i].simularPartidos();
        grupos[i].calcularPuntos();
        grupos[i].clasificar();
    }
    cout << "\n=== FASE DE GRUPOS COMPLETADA ===" << endl;
    imprimirGrupos();
    configurarR16();
    medidor.imprimirReporte(numEquipos, NUM_GRUPOS);
}

void Torneo::configurarR16() {
    medidor.reset();
    const char* fechaR16 = "10/07/2026";
    Equipo* primeros[NUM_GRUPOS];
    Equipo* segundos[NUM_GRUPOS];
    Equipo* terceros[NUM_GRUPOS];
    int puntosTerceros[NUM_GRUPOS];
    int puntosSegundos[NUM_GRUPOS];

    for (int i = 0; i < NUM_GRUPOS; i++) {
        medidor.contarIteracion();
        primeros[i] = nullptr;
        segundos[i] = nullptr;
        terceros[i] = nullptr;
        puntosTerceros[i] = 0;
        puntosSegundos[i] = 0;
    }
    for (int i = 0; i < NUM_GRUPOS; i++) {
        medidor.contarIteracion();
        int idx0 = grupos[i].getOrdenClasificacion(0);
        int idx1 = grupos[i].getOrdenClasificacion(1);
        int idx2 = grupos[i].getOrdenClasificacion(2);
        primeros[i] = grupos[i].getEquipo(idx0);
        segundos[i] = grupos[i].getEquipo(idx1);
        terceros[i] = grupos[i].getEquipo(idx2);
        puntosTerceros[i] = grupos[i].getPuntos(idx2);
        puntosSegundos[i] = grupos[i].getPuntos(idx1);
    }

    int indicesTerceros[NUM_GRUPOS];
    for (int i = 0; i < NUM_GRUPOS; i++) indicesTerceros[i] = i;
    for (int i = 0; i < NUM_GRUPOS - 1; i++) {
        medidor.contarIteracion();
        for (int j = 0; j < NUM_GRUPOS - 1 - i; j++) {
            medidor.contarIteracion();
            int a = indicesTerceros[j];
            int b = indicesTerceros[j+1];
            if (puntosTerceros[a] < puntosTerceros[b]) {
                int temp = indicesTerceros[j];
                indicesTerceros[j] = indicesTerceros[j+1];
                indicesTerceros[j+1] = temp;
            }
        }
    }
    Equipo* mejoresTerceros[8];
    for (int i = 0; i < 8; i++) {
        medidor.contarIteracion();
        mejoresTerceros[i] = terceros[indicesTerceros[i]];
    }

    int indicesSegundos[NUM_GRUPOS];
    for (int i = 0; i < NUM_GRUPOS; i++) indicesSegundos[i] = i;
    for (int i = 0; i < NUM_GRUPOS - 1; i++) {
        medidor.contarIteracion();
        for (int j = 0; j < NUM_GRUPOS - 1 - i; j++) {
            medidor.contarIteracion();
            int a = indicesSegundos[j];
            int b = indicesSegundos[j+1];
            if (puntosSegundos[a] > puntosSegundos[b]) {
                int temp = indicesSegundos[j];
                indicesSegundos[j] = indicesSegundos[j+1];
                indicesSegundos[j+1] = temp;
            }
        }
    }
    Equipo* peoresSegundos[4];
    for (int i = 0; i < 4; i++) {
        medidor.contarIteracion();
        peoresSegundos[i] = segundos[indicesSegundos[i]];
    }
    Equipo* mejoresSegundos[8];
    for (int i = 0; i < 8; i++) {
        medidor.contarIteracion();
        mejoresSegundos[i] = segundos[indicesSegundos[i+4]];
    }

    bool terceroUsado[8] = {false};
    for (int i = 0; i < 8; i++) {
        medidor.contarIteracion();
        if (primeros[i] == nullptr) continue;
        bool asignado = false;
        for (int k = 0; k < 8 && !asignado; k++) {
            medidor.contarIteracion();
            if (terceroUsado[k]) continue;
            if (mejoresTerceros[k] == nullptr) continue;
            bool mismoGrupo = false;
            for (int g = 0; g < NUM_GRUPOS && !mismoGrupo; g++) {
                medidor.contarIteracion();
                bool p1 = false, p2 = false;
                for (int e = 0; e < NUM_EQUIPOS_GRUPO; e++) {
                    medidor.contarIteracion();
                    if (grupos[g].getEquipo(e) == primeros[i]) p1 = true;
                    if (grupos[g].getEquipo(e) == mejoresTerceros[k]) p2 = true;
                }
                if (p1 && p2) mismoGrupo = true;
            }
            if (!mismoGrupo) {
                partidosR16[i].setEquipo1(primeros[i]);
                partidosR16[i].setEquipo2(mejoresTerceros[k]);
                partidosR16[i].setFecha(fechaR16);
                partidosR16[i].setSede("nombreSede");
                partidosR16[i].setHora("00:00");
                partidosR16[i].setArbitro(0, "codArbitro1");
                partidosR16[i].setArbitro(1, "codArbitro2");
                partidosR16[i].setArbitro(2, "codArbitro3");
                partidosR16[i].setEsEliminatoria(true);
                terceroUsado[k] = true;
                asignado = true;
            }
        }
        if (!asignado) {
            for (int k = 0; k < 8 && !asignado; k++) {
                medidor.contarIteracion();
                if (terceroUsado[k]) continue;
                if (mejoresTerceros[k] == nullptr) continue;
                partidosR16[i].setEquipo1(primeros[i]);
                partidosR16[i].setEquipo2(mejoresTerceros[k]);
                partidosR16[i].setFecha(fechaR16);
                partidosR16[i].setSede("nombreSede");
                partidosR16[i].setHora("00:00");
                partidosR16[i].setArbitro(0, "codArbitro1");
                partidosR16[i].setArbitro(1, "codArbitro2");
                partidosR16[i].setArbitro(2, "codArbitro3");
                partidosR16[i].setEsEliminatoria(true);
                terceroUsado[k] = true;
                asignado = true;
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        medidor.contarIteracion();
        if (primeros[i+8] == nullptr) continue;
        if (peoresSegundos[i] == nullptr) continue;
        partidosR16[i+8].setEquipo1(primeros[i+8]);
        partidosR16[i+8].setEquipo2(peoresSegundos[i]);
        partidosR16[i+8].setFecha(fechaR16);
        partidosR16[i+8].setSede("nombreSede");
        partidosR16[i+8].setHora("00:00");
        partidosR16[i+8].setArbitro(0, "codArbitro1");
        partidosR16[i+8].setArbitro(1, "codArbitro2");
        partidosR16[i+8].setArbitro(2, "codArbitro3");
        partidosR16[i+8].setEsEliminatoria(true);
    }
    for (int i = 0; i < 4; i++) {
        medidor.contarIteracion();
        if (mejoresSegundos[i*2] == nullptr) continue;
        if (mejoresSegundos[i*2+1] == nullptr) continue;
        partidosR16[i+12].setEquipo1(mejoresSegundos[i*2]);
        partidosR16[i+12].setEquipo2(mejoresSegundos[i*2+1]);
        partidosR16[i+12].setFecha(fechaR16);
        partidosR16[i+12].setSede("nombreSede");
        partidosR16[i+12].setHora("00:00");
        partidosR16[i+12].setArbitro(0, "codArbitro1");
        partidosR16[i+12].setArbitro(1, "codArbitro2");
        partidosR16[i+12].setArbitro(2, "codArbitro3");
        partidosR16[i+12].setEsEliminatoria(true);
    }

    cout << "\n=== PARTIDOS R16 CONFIGURADOS ===" << endl;
    for (int i = 0; i < 16; i++) {
        medidor.contarIteracion();
        if (partidosR16[i].getEquipo1() == nullptr ||
            partidosR16[i].getEquipo2() == nullptr) {
            cout << (i+1) << ". PARTIDO SIN ASIGNAR" << endl;
            continue;
        }
        cout << (i+1) << ". "
             << partidosR16[i].getEquipo1()->getPais()
             << " vs "
             << partidosR16[i].getEquipo2()->getPais()
             << endl;
    }
}

void Torneo::simularR16() {
    medidor.reset();
    cout << "\n=== DIECISEISAVOS DE FINAL ===" << endl;
    for (int i = 0; i < 16; i++) {
        medidor.contarIteracion();
        if (partidosR16[i].getEquipo1() == nullptr ||
            partidosR16[i].getEquipo2() == nullptr) {
            cout << "Partido " << (i+1) << " sin equipos asignados." << endl;
            continue;
        }
        partidosR16[i].simular();
        partidosR16[i].imprimir();
    }
    for (int i = 0; i < 8; i++) {
        medidor.contarIteracion();
        int g1 = partidosR16[i*2].getResEquipo1().getGolesFavor();
        int g2 = partidosR16[i*2].getResEquipo2().getGolesFavor();
        Equipo* ganador1 = (g1 >= g2) ?
                               partidosR16[i*2].getEquipo1() : partidosR16[i*2].getEquipo2();
        g1 = partidosR16[i*2+1].getResEquipo1().getGolesFavor();
        g2 = partidosR16[i*2+1].getResEquipo2().getGolesFavor();
        Equipo* ganador2 = (g1 >= g2) ?
                               partidosR16[i*2+1].getEquipo1() : partidosR16[i*2+1].getEquipo2();
        partidosR8[i].setEquipo1(ganador1);
        partidosR8[i].setEquipo2(ganador2);
        partidosR8[i].setFecha("10/07/2026");
        partidosR8[i].setSede("nombreSede");
        partidosR8[i].setHora("00:00");
        partidosR8[i].setArbitro(0, "codArbitro1");
        partidosR8[i].setArbitro(1, "codArbitro2");
        partidosR8[i].setArbitro(2, "codArbitro3");
        partidosR8[i].setEsEliminatoria(true);
    }
    medidor.imprimirReporte(numEquipos, NUM_GRUPOS);
}

void Torneo::simularR8() {
    medidor.reset();
    cout << "\n=== OCTAVOS DE FINAL ===" << endl;
    for (int i = 0; i < 8; i++) {
        medidor.contarIteracion();
        partidosR8[i].simular();
        partidosR8[i].imprimir();
    }
    for (int i = 0; i < 4; i++) {
        medidor.contarIteracion();
        int g1 = partidosR8[i*2].getResEquipo1().getGolesFavor();
        int g2 = partidosR8[i*2].getResEquipo2().getGolesFavor();
        Equipo* ganador1 = (g1 >= g2) ?
                               partidosR8[i*2].getEquipo1() : partidosR8[i*2].getEquipo2();
        g1 = partidosR8[i*2+1].getResEquipo1().getGolesFavor();
        g2 = partidosR8[i*2+1].getResEquipo2().getGolesFavor();
        Equipo* ganador2 = (g1 >= g2) ?
                               partidosR8[i*2+1].getEquipo1() : partidosR8[i*2+1].getEquipo2();
        partidosQF[i].setEquipo1(ganador1);
        partidosQF[i].setEquipo2(ganador2);
        partidosQF[i].setFecha("10/07/2026");
        partidosQF[i].setSede("nombreSede");
        partidosQF[i].setHora("00:00");
        partidosQF[i].setArbitro(0, "codArbitro1");
        partidosQF[i].setArbitro(1, "codArbitro2");
        partidosQF[i].setArbitro(2, "codArbitro3");
        partidosQF[i].setEsEliminatoria(true);
    }
    medidor.imprimirReporte(numEquipos, NUM_GRUPOS);
}

void Torneo::simularQF() {
    medidor.reset();
    cout << "\n=== CUARTOS DE FINAL ===" << endl;
    for (int i = 0; i < 4; i++) {
        medidor.contarIteracion();
        partidosQF[i].simular();
        partidosQF[i].imprimir();
    }
    for (int i = 0; i < 2; i++) {
        medidor.contarIteracion();
        int g1 = partidosQF[i*2].getResEquipo1().getGolesFavor();
        int g2 = partidosQF[i*2].getResEquipo2().getGolesFavor();
        Equipo* ganador1 = (g1 >= g2) ?
                               partidosQF[i*2].getEquipo1() : partidosQF[i*2].getEquipo2();
        g1 = partidosQF[i*2+1].getResEquipo1().getGolesFavor();
        g2 = partidosQF[i*2+1].getResEquipo2().getGolesFavor();
        Equipo* ganador2 = (g1 >= g2) ?
                               partidosQF[i*2+1].getEquipo1() : partidosQF[i*2+1].getEquipo2();
        partidosSF[i].setEquipo1(ganador1);
        partidosSF[i].setEquipo2(ganador2);
        partidosSF[i].setFecha("10/07/2026");
        partidosSF[i].setSede("nombreSede");
        partidosSF[i].setHora("00:00");
        partidosSF[i].setArbitro(0, "codArbitro1");
        partidosSF[i].setArbitro(1, "codArbitro2");
        partidosSF[i].setArbitro(2, "codArbitro3");
        partidosSF[i].setEsEliminatoria(true);
    }
    medidor.imprimirReporte(numEquipos, NUM_GRUPOS);
}

void Torneo::simularSF() {
    medidor.reset();
    cout << "\n=== SEMIFINALES ===" << endl;
    for (int i = 0; i < 2; i++) {
        medidor.contarIteracion();
        partidosSF[i].simular();
        partidosSF[i].imprimir();
    }
    Equipo* ganadores[2];
    Equipo* perdedores[2];
    for (int i = 0; i < 2; i++) {
        medidor.contarIteracion();
        int g1 = partidosSF[i].getResEquipo1().getGolesFavor();
        int g2 = partidosSF[i].getResEquipo2().getGolesFavor();
        if (g1 >= g2) {
            ganadores[i]  = partidosSF[i].getEquipo1();
            perdedores[i] = partidosSF[i].getEquipo2();
        } else {
            ganadores[i]  = partidosSF[i].getEquipo2();
            perdedores[i] = partidosSF[i].getEquipo1();
        }
    }
    partidoFinal.setEquipo1(ganadores[0]);
    partidoFinal.setEquipo2(ganadores[1]);
    partidoFinal.setFecha("10/07/2026");
    partidoFinal.setSede("nombreSede");
    partidoFinal.setHora("00:00");
    partidoFinal.setArbitro(0, "codArbitro1");
    partidoFinal.setArbitro(1, "codArbitro2");
    partidoFinal.setArbitro(2, "codArbitro3");
    partidoFinal.setEsEliminatoria(true);
    partidoTercero.setEquipo1(perdedores[0]);
    partidoTercero.setEquipo2(perdedores[1]);
    partidoTercero.setFecha("10/07/2026");
    partidoTercero.setSede("nombreSede");
    partidoTercero.setHora("00:00");
    partidoTercero.setArbitro(0, "codArbitro1");
    partidoTercero.setArbitro(1, "codArbitro2");
    partidoTercero.setArbitro(2, "codArbitro3");
    partidoTercero.setEsEliminatoria(true);
    medidor.imprimirReporte(numEquipos, NUM_GRUPOS);
}

void Torneo::simularFinal() {
    medidor.reset();
    cout << "\n=== TERCER PUESTO ===" << endl;
    partidoTercero.simular();
    partidoTercero.imprimir();
    cout << "\n=== FINAL ===" << endl;
    partidoFinal.simular();
    partidoFinal.imprimir();
    medidor.imprimirReporte(numEquipos, NUM_GRUPOS);
}

void Torneo::imprimirGrupos() const {
    for (int i = 0; i < NUM_GRUPOS; i++) {
        medidor.contarIteracion();
        grupos[i].imprimirTabla();
    }
}

void Torneo::imprimirEstadisticas() const {
    medidor.reset();
    cout << "\n=== ESTADISTICAS FINALES ===" << endl;

    int g1 = partidoFinal.getResEquipo1().getGolesFavor();
    int g2 = partidoFinal.getResEquipo2().getGolesFavor();
    Equipo* campeon = (g1 >= g2) ?
                          partidoFinal.getEquipo1() : partidoFinal.getEquipo2();
    cout << "1er puesto (CAMPEON): " << campeon->getPais() << endl;

    Equipo* subcampeon = (g1 >= g2) ?
                             partidoFinal.getEquipo2() : partidoFinal.getEquipo1();
    cout << "2do puesto: " << subcampeon->getPais() << endl;

    int g3 = partidoTercero.getResEquipo1().getGolesFavor();
    int g4 = partidoTercero.getResEquipo2().getGolesFavor();
    Equipo* tercerPuesto = (g3 >= g4) ?
                               partidoTercero.getEquipo1() : partidoTercero.getEquipo2();
    Equipo* cuartoPuesto = (g3 >= g4) ?
                               partidoTercero.getEquipo2() : partidoTercero.getEquipo1();
    cout << "3er puesto: " << tercerPuesto->getPais() << endl;
    cout << "4to puesto: " << cuartoPuesto->getPais() << endl;

    Jugador* jugs = campeon->getJugadores();
    int maxGoles = 0;
    int idxMaxGoleador = 0;
    for (int i = 0; i < NUM_JUGADORES; i++) {
        medidor.contarIteracion();
        if (jugs[i].getStats().getGoles() > maxGoles) {
            maxGoles = jugs[i].getStats().getGoles();
            idxMaxGoleador = i;
        }
    }
    cout << "\nMax goleador del campeon: "
         << jugs[idxMaxGoleador].getNombre() << " "
         << jugs[idxMaxGoleador].getApellido()
         << " (" << maxGoles << " goles)" << endl;

    cout << "\nTop 3 goleadores del torneo:" << endl;
    int topGoles[3]   = {0, 0, 0};
    int topEquipo[3]  = {-1, -1, -1};
    int topJugador[3] = {-1, -1, -1};

    for (int e = 0; e < numEquipos; e++) {
        medidor.contarIteracion();
        Jugador* js = equipos[e].getJugadores();
        for (int j = 0; j < NUM_JUGADORES; j++) {
            medidor.contarIteracion();
            int g = js[j].getStats().getGoles();
            if (g > topGoles[0]) {
                topGoles[2]=topGoles[1]; topEquipo[2]=topEquipo[1]; topJugador[2]=topJugador[1];
                topGoles[1]=topGoles[0]; topEquipo[1]=topEquipo[0]; topJugador[1]=topJugador[0];
                topGoles[0]=g; topEquipo[0]=e; topJugador[0]=j;
            } else if (g > topGoles[1]) {
                topGoles[2]=topGoles[1]; topEquipo[2]=topEquipo[1]; topJugador[2]=topJugador[1];
                topGoles[1]=g; topEquipo[1]=e; topJugador[1]=j;
            } else if (g > topGoles[2]) {
                topGoles[2]=g; topEquipo[2]=e; topJugador[2]=j;
            }
        }
    }
    for (int i = 0; i < 3; i++) {
        medidor.contarIteracion();
        if (topEquipo[i] != -1) {
            Jugador& jug = equipos[topEquipo[i]].getJugadores()[topJugador[i]];
            cout << (i+1) << ". " << jug.getNombre() << " " << jug.getApellido()
                 << " (" << equipos[topEquipo[i]].getPais() << ")"
                 << " - " << topGoles[i] << " goles" << endl;
        }
    }

    int maxGolesHist = 0;
    int idxMaxEquipo = 0;
    for (int i = 0; i < numEquipos; i++) {
        medidor.contarIteracion();
        if (equipos[i].getStats().getGolesFavor() > maxGolesHist) {
            maxGolesHist = equipos[i].getStats().getGolesFavor();
            idxMaxEquipo = i;
        }
    }
    cout << "\nEquipo con mas goles historicos: "
         << equipos[idxMaxEquipo].getPais()
         << " (" << maxGolesHist << " goles)" << endl;

    cout << "\nConfederacion con mayor presencia por etapa:" << endl;
    const char* confNombres[6] = {"UEFA","CONMEBOL","Concacaf","CAF","AFC","OFC"};

    int confCountR16[6] = {0};
    for (int i = 0; i < 16; i++) {
        medidor.contarIteracion();
        const char* c1 = partidosR16[i].getEquipo1()->getConfederacion();
        const char* c2 = partidosR16[i].getEquipo2()->getConfederacion();
        for (int k = 0; k < 6; k++) {
            medidor.contarIteracion();
            if (strcmp(c1, confNombres[k]) == 0) confCountR16[k]++;
            if (strcmp(c2, confNombres[k]) == 0) confCountR16[k]++;
        }
    }
    int maxR16 = 0;
    for (int k = 0; k < 6; k++) {
        medidor.contarIteracion();
        if (confCountR16[k] > maxR16) maxR16 = confCountR16[k];
    }
    cout << "R16: ";
    for (int k = 0; k < 6; k++) {
        if (confCountR16[k] == maxR16)
            cout << confNombres[k] << " (" << maxR16 << " equipos) ";
    }
    cout << endl;

    int confCountR8[6] = {0};
    for (int i = 0; i < 8; i++) {
        medidor.contarIteracion();
        const char* c1 = partidosR8[i].getEquipo1()->getConfederacion();
        const char* c2 = partidosR8[i].getEquipo2()->getConfederacion();
        for (int k = 0; k < 6; k++) {
            medidor.contarIteracion();
            if (strcmp(c1, confNombres[k]) == 0) confCountR8[k]++;
            if (strcmp(c2, confNombres[k]) == 0) confCountR8[k]++;
        }
    }
    int maxR8 = 0;
    for (int k = 0; k < 6; k++) {
        medidor.contarIteracion();
        if (confCountR8[k] > maxR8) maxR8 = confCountR8[k];
    }
    cout << "R8:  ";
    for (int k = 0; k < 6; k++) {
        if (confCountR8[k] == maxR8)
            cout << confNombres[k] << " (" << maxR8 << " equipos) ";
    }
    cout << endl;

    int confCountQF[6] = {0};
    for (int i = 0; i < 4; i++) {
        medidor.contarIteracion();
        const char* c1 = partidosQF[i].getEquipo1()->getConfederacion();
        const char* c2 = partidosQF[i].getEquipo2()->getConfederacion();
        for (int k = 0; k < 6; k++) {
            medidor.contarIteracion();
            if (strcmp(c1, confNombres[k]) == 0) confCountQF[k]++;
            if (strcmp(c2, confNombres[k]) == 0) confCountQF[k]++;
        }
    }
    int maxQF = 0;
    for (int k = 0; k < 6; k++) {
        medidor.contarIteracion();
        if (confCountQF[k] > maxQF) maxQF = confCountQF[k];
    }
    cout << "R4:  ";
    for (int k = 0; k < 6; k++) {
        if (confCountQF[k] == maxQF)
            cout << confNombres[k] << " (" << maxQF << " equipos) ";
    }
    cout << endl;

    medidor.imprimirReporte(numEquipos, NUM_GRUPOS);
}

void Torneo::guardarTodo() {
    GestorArchivos gestor;
    gestor.guardarDatos(
        "C:/Users/JUAN JOSE/OneDrive/Documentos/UdeAWorldCup/selecciones_clasificadas_mundial.csv",
        equipos, numEquipos);
    gestor.guardarJugadores(
        "C:/Users/JUAN JOSE/OneDrive/Documentos/UdeAWorldCup/jugadores.csv",
        equipos, numEquipos);
}
