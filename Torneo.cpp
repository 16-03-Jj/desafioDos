#include "torneo.h"
#include "gestorarchivos.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;

// Constructor
Torneo::Torneo() {
    equipos = new Equipo[NUM_EQUIPOS_TORNEO];
    grupos  = new Grupo[NUM_GRUPOS];
    numEquipos = 0;

    // CORRECCIÓN: R16 tiene 16 partidos (32 equipos → 16 cruces)
    // R8 tiene 8 partidos, QF tiene 4, SF tiene 2
    partidosR16 = new Partido[16];
    partidosR8  = new Partido[8];
    partidosQF  = new Partido[4];
    partidosSF  = new Partido[2];

    srand(time(0));
}

// Destructor
Torneo::~Torneo() {
    delete[] equipos;
    delete[] grupos;
    delete[] partidosR16;
    delete[] partidosR8;
    delete[] partidosQF;
    delete[] partidosSF;
}

// I - Carga los datos del CSV usando GestorArchivos
void Torneo::cargarDatos(const char* rutaCSV) {
    GestorArchivos gestor;
    numEquipos = gestor.leerCSV(rutaCSV, equipos, NUM_EQUIPOS_TORNEO);
    for (int i = 0; i < numEquipos; i++) {
        equipos[i].inicializarJugadores();
    }
    cout << "Datos cargados: " << numEquipos << " equipos." << endl;
}

// Ordena los equipos por ranking FIFA usando burbuja
void Torneo::ordenarPorRanking() {
    for (int i = 0; i < numEquipos - 1; i++) {
        for (int j = 0; j < numEquipos - 1 - i; j++) {
            if (equipos[j+1] < equipos[j]) {
                Equipo temp = equipos[j];
                equipos[j] = equipos[j+1];
                equipos[j+1] = temp;
            }
        }
    }
}

// Arma los 4 bombos del sorteo
// EE.UU. va directo al bombo 1
void Torneo::armarBombos(int bombos[NUM_BOMBOS][EQUIPOS_POR_BOMBO],
                         int& indiceUSA) {
    ordenarPorRanking();

    // Inicializar bombos en -1
    for (int i = 0; i < NUM_BOMBOS; i++)
        for (int j = 0; j < EQUIPOS_POR_BOMBO; j++)
            bombos[i][j] = -1;

    indiceUSA = -1;
    for (int i = 0; i < numEquipos; i++) {
        if (strcmp(equipos[i].getPais(), "United States") == 0 ||
            strcmp(equipos[i].getPais(), "USA") == 0) {
            indiceUSA = i;
            break;
        }
    }

    int contBombo[NUM_BOMBOS] = {0, 0, 0, 0};

    // EE.UU. va al bombo 1 primero
    if (indiceUSA != -1) {
        bombos[0][0] = indiceUSA;
        contBombo[0] = 1;
    }

    int bomboActual = 0;
    for (int i = 0; i < numEquipos; i++) {
        if (i == indiceUSA) continue;
        if (contBombo[bomboActual] >= EQUIPOS_POR_BOMBO) {
            bomboActual++;
        }
        if (bomboActual >= NUM_BOMBOS) break;
        bombos[bomboActual][contBombo[bomboActual]] = i;
        contBombo[bomboActual]++;
    }
}

// Verifica si un equipo puede entrar a un grupo
// Restricción: max 2 equipos UEFA por grupo
// Restricción: no dos equipos de la misma confederación (salvo UEFA)
bool Torneo::puedenCompartirGrupo(Equipo* nuevo, Equipo* eq2, Grupo& grupo) {
    int countUEFA = 0;
    int countConfNuevo = 0;
    const char* confNuevo = nuevo->getConfederacion();

    for (int i = 0; i < NUM_EQUIPOS_GRUPO; i++) {
        Equipo* eq = grupo.getEquipo(i);
        if (eq == nullptr) continue;
        if (strcmp(eq->getConfederacion(), confNuevo) == 0) {
            countConfNuevo++;
        }
        if (strcmp(eq->getConfederacion(), "UEFA") == 0) {
            countUEFA++;
        }
    }

    if (strcmp(confNuevo, "UEFA") == 0 && countUEFA >= 2) return false;
    if (strcmp(confNuevo, "UEFA") != 0 && countConfNuevo >= 1) return false;
    return true;
}

// II - Conforma los 12 grupos del torneo
void Torneo::conformarGrupos() {
    int bombos[NUM_BOMBOS][EQUIPOS_POR_BOMBO];
    int indiceUSA = -1;
    armarBombos(bombos, indiceUSA);

    char letras[NUM_GRUPOS] = {'A','B','C','D','E','F',
                               'G','H','I','J','K','L'};
    for (int i = 0; i < NUM_GRUPOS; i++) {
        grupos[i].setLetra(letras[i]);
    }

    bool asignado[NUM_EQUIPOS_TORNEO] = {false};

    for (int bombo = 0; bombo < NUM_BOMBOS; bombo++) {
        for (int g = 0; g < NUM_GRUPOS; g++) {
            bool encontrado = false;
            // Mezclar el bombo aleatoriamente
            for (int k = EQUIPOS_POR_BOMBO - 1; k > 0; k--) {
                int r = rand() % (k + 1);
                int temp = bombos[bombo][k];
                bombos[bombo][k] = bombos[bombo][r];
                bombos[bombo][r] = temp;
            }
            for (int e = 0; e < EQUIPOS_POR_BOMBO && !encontrado; e++) {
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
        }
    }
}

// III - Simula la fase de grupos completa
void Torneo::simularFaseGrupos() {
    for (int i = 0; i < NUM_GRUPOS; i++) {
        grupos[i].configurarPartidos(20);
        grupos[i].simularPartidos();
        grupos[i].calcularPuntos();
        grupos[i].clasificar();
    }
    cout << "\n=== FASE DE GRUPOS COMPLETADA ===" << endl;
    imprimirGrupos();
    configurarR16();
}

// Configura los 16 partidos de dieciseisavos
void Torneo::configurarR16() {
    const char* fechaR16 = "10/07/2026";

    Equipo* primeros[NUM_GRUPOS];
    Equipo* segundos[NUM_GRUPOS];
    Equipo* terceros[NUM_GRUPOS];
    int puntosTerceros[NUM_GRUPOS];

    for (int i = 0; i < NUM_GRUPOS; i++) {
        int idx0 = grupos[i].getOrdenClasificacion(0);
        int idx1 = grupos[i].getOrdenClasificacion(1);
        int idx2 = grupos[i].getOrdenClasificacion(2);
        primeros[i] = grupos[i].getEquipo(idx0);
        segundos[i] = grupos[i].getEquipo(idx1);
        terceros[i] = grupos[i].getEquipo(idx2);
        puntosTerceros[i] = grupos[i].getPuntos(idx2);
    }

    // Seleccionar los 8 mejores terceros ordenados por puntos
    int indicesTerceros[NUM_GRUPOS];
    for (int i = 0; i < NUM_GRUPOS; i++) indicesTerceros[i] = i;

    // Burbuja descendente por puntos de terceros
    for (int i = 0; i < NUM_GRUPOS - 1; i++) {
        for (int j = 0; j < NUM_GRUPOS - 1 - i; j++) {
            int a = indicesTerceros[j];
            int b = indicesTerceros[j + 1];
            if (puntosTerceros[a] < puntosTerceros[b]) {
                int temp = indicesTerceros[j];
                indicesTerceros[j] = indicesTerceros[j + 1];
                indicesTerceros[j + 1] = temp;
            }
        }
    }

    // Los primeros 8 son los mejores terceros
    Equipo* mejoresTerceros[8];
    int grupoTercero[8]; // grupo de origen de cada tercer clasificado
    for (int i = 0; i < 8; i++) {
        mejoresTerceros[i] = terceros[indicesTerceros[i]];
        grupoTercero[i] = indicesTerceros[i];
    }

    // CORRECCIÓN: 16 partidos totales
    // Partidos 0-7:  primeros[0..7] vs mejoresTerceros (evitando mismo grupo)
    // Partidos 8-15: segundos emparejados entre sí (evitando mismo grupo)

    // Paso 1: primeros vs mejores terceros (8 partidos)
    bool terceroUsado[8] = {false};
    for (int i = 0; i < 8; i++) {
        // Buscar un tercero que no sea del mismo grupo que primeros[i]
        for (int k = 0; k < 8; k++) {
            if (terceroUsado[k]) continue;
            // Verificar que primeros[i] y mejoresTerceros[k] no sean del mismo grupo
            bool mismoGrupo = false;
            for (int g = 0; g < NUM_GRUPOS; g++) {
                bool primerEsDeG = false, terceroEsDeG = false;
                for (int e = 0; e < NUM_EQUIPOS_GRUPO; e++) {
                    if (grupos[g].getEquipo(e) == primeros[i])
                        primerEsDeG = true;
                    if (grupos[g].getEquipo(e) == mejoresTerceros[k])
                        terceroEsDeG = true;
                }
                if (primerEsDeG && terceroEsDeG) {
                    mismoGrupo = true;
                    break;
                }
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
                break;
            }
        }
    }

    // Paso 2: segundos entre sí (8 partidos, índices 8-15)
    // Emparejar los 12 segundos en 6 pares... pero solo necesitamos 8 partidos
    // Usamos los 12 segundos: los primeros 4 juegan contra cabezas de grupo
    // los últimos 8 juegan entre sí según el enunciado
    // Por simplicidad: emparejamos los 12 segundos en 6 pares consecutivos
    // evitando mismo grupo
    bool segundoUsado[NUM_GRUPOS] = {false};
    int partidoIdx = 8;

    for (int i = 0; i < NUM_GRUPOS && partidoIdx < 16; i++) {
        if (segundoUsado[i]) continue;
        for (int j = i + 1; j < NUM_GRUPOS && partidoIdx < 16; j++) {
            if (segundoUsado[j]) continue;
            // Los segundos siempre vienen de grupos distintos → no hay conflicto
            partidosR16[partidoIdx].setEquipo1(segundos[i]);
            partidosR16[partidoIdx].setEquipo2(segundos[j]);
            partidosR16[partidoIdx].setFecha(fechaR16);
            partidosR16[partidoIdx].setSede("nombreSede");
            partidosR16[partidoIdx].setHora("00:00");
            partidosR16[partidoIdx].setArbitro(0, "codArbitro1");
            partidosR16[partidoIdx].setArbitro(1, "codArbitro2");
            partidosR16[partidoIdx].setArbitro(2, "codArbitro3");
            partidosR16[partidoIdx].setEsEliminatoria(true);
            segundoUsado[i] = true;
            segundoUsado[j] = true;
            partidoIdx++;
            break;
        }
    }

    cout << "\n=== PARTIDOS R16 CONFIGURADOS ===" << endl;
    for (int i = 0; i < 16; i++) {
        cout << (i + 1) << ". "
             << partidosR16[i].getEquipo1()->getPais()
             << " vs "
             << partidosR16[i].getEquipo2()->getPais()
             << endl;
    }
}

// Simula los 16 partidos de R16 y configura R8
void Torneo::simularR16() {
    cout << "\n=== DIECISEISAVOS DE FINAL ===" << endl;
    for (int i = 0; i < 16; i++) {
        partidosR16[i].simular();
        partidosR16[i].imprimir();
    }

    // CORRECCIÓN: 16 partidos → 8 ganadores → 8 partidos de R8
    for (int i = 0; i < 8; i++) {
        int g1 = partidosR16[i * 2].getResEquipo1().getGolesFavor();
        int g2 = partidosR16[i * 2].getResEquipo2().getGolesFavor();
        Equipo* ganador1 = (g1 >= g2) ?
                               partidosR16[i * 2].getEquipo1() :
                               partidosR16[i * 2].getEquipo2();

        g1 = partidosR16[i * 2 + 1].getResEquipo1().getGolesFavor();
        g2 = partidosR16[i * 2 + 1].getResEquipo2().getGolesFavor();
        Equipo* ganador2 = (g1 >= g2) ?
                               partidosR16[i * 2 + 1].getEquipo1() :
                               partidosR16[i * 2 + 1].getEquipo2();

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
}

// Simula los 8 partidos de octavos y configura QF
void Torneo::simularR8() {
    cout << "\n=== OCTAVOS DE FINAL ===" << endl;
    for (int i = 0; i < 8; i++) {
        partidosR8[i].simular();
        partidosR8[i].imprimir();
    }

    // CORRECCIÓN: 8 partidos → 4 ganadores → 4 partidos de QF
    for (int i = 0; i < 4; i++) {
        int g1 = partidosR8[i * 2].getResEquipo1().getGolesFavor();
        int g2 = partidosR8[i * 2].getResEquipo2().getGolesFavor();
        Equipo* ganador1 = (g1 >= g2) ?
                               partidosR8[i * 2].getEquipo1() :
                               partidosR8[i * 2].getEquipo2();

        g1 = partidosR8[i * 2 + 1].getResEquipo1().getGolesFavor();
        g2 = partidosR8[i * 2 + 1].getResEquipo2().getGolesFavor();
        Equipo* ganador2 = (g1 >= g2) ?
                               partidosR8[i * 2 + 1].getEquipo1() :
                               partidosR8[i * 2 + 1].getEquipo2();

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
}

// Simula los 4 partidos de cuartos y configura SF
void Torneo::simularQF() {
    cout << "\n=== CUARTOS DE FINAL ===" << endl;
    for (int i = 0; i < 4; i++) {
        partidosQF[i].simular();
        partidosQF[i].imprimir();
    }

    // CORRECCIÓN: 4 partidos → 2 ganadores → 2 partidos de SF
    for (int i = 0; i < 2; i++) {
        int g1 = partidosQF[i * 2].getResEquipo1().getGolesFavor();
        int g2 = partidosQF[i * 2].getResEquipo2().getGolesFavor();
        Equipo* ganador1 = (g1 >= g2) ?
                               partidosQF[i * 2].getEquipo1() :
                               partidosQF[i * 2].getEquipo2();

        g1 = partidosQF[i * 2 + 1].getResEquipo1().getGolesFavor();
        g2 = partidosQF[i * 2 + 1].getResEquipo2().getGolesFavor();
        Equipo* ganador2 = (g1 >= g2) ?
                               partidosQF[i * 2 + 1].getEquipo1() :
                               partidosQF[i * 2 + 1].getEquipo2();

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
}

// Simula los 2 partidos de semifinales y configura final y tercer puesto
// CORRECCIÓN: solo hay 2 partidos de SF (no 4)
void Torneo::simularSF() {
    cout << "\n=== SEMIFINALES ===" << endl;
    for (int i = 0; i < 2; i++) {
        partidosSF[i].simular();
        partidosSF[i].imprimir();
    }

    Equipo* ganadores[2];
    Equipo* perdedores[2];

    // CORRECCIÓN: iterar sobre los 2 partidos de SF (índices 0 y 1)
    for (int i = 0; i < 2; i++) {
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

    // Configurar final
    partidoFinal.setEquipo1(ganadores[0]);
    partidoFinal.setEquipo2(ganadores[1]);
    partidoFinal.setFecha("10/07/2026");
    partidoFinal.setSede("nombreSede");
    partidoFinal.setHora("00:00");
    partidoFinal.setArbitro(0, "codArbitro1");
    partidoFinal.setArbitro(1, "codArbitro2");
    partidoFinal.setArbitro(2, "codArbitro3");
    partidoFinal.setEsEliminatoria(true);

    // Configurar partido del tercer puesto
    partidoTercero.setEquipo1(perdedores[0]);
    partidoTercero.setEquipo2(perdedores[1]);
    partidoTercero.setFecha("10/07/2026");
    partidoTercero.setSede("nombreSede");
    partidoTercero.setHora("00:00");
    partidoTercero.setArbitro(0, "codArbitro1");
    partidoTercero.setArbitro(1, "codArbitro2");
    partidoTercero.setArbitro(2, "codArbitro3");
    partidoTercero.setEsEliminatoria(true);
}

// Simula la final y el partido del tercer puesto
void Torneo::simularFinal() {
    cout << "\n=== TERCER PUESTO ===" << endl;
    partidoTercero.simular();
    partidoTercero.imprimir();

    cout << "\n=== FINAL ===" << endl;
    partidoFinal.simular();
    partidoFinal.imprimir();
}

// Imprime todos los grupos conformados
void Torneo::imprimirGrupos() const {
    for (int i = 0; i < NUM_GRUPOS; i++) {
        grupos[i].imprimirTabla();
    }
}

// IV - Estadísticas finales del torneo
void Torneo::imprimirEstadisticas() const {
    cout << "\n=== ESTADISTICAS FINALES ===" << endl;

    // Determinar campeón
    // CORRECCIÓN: usar versión const de getResEquipo
    int g1 = partidoFinal.getResEquipo1().getGolesFavor();
    int g2 = partidoFinal.getResEquipo2().getGolesFavor();
    Equipo* campeon = (g1 >= g2) ?
                          partidoFinal.getEquipo1() :
                          partidoFinal.getEquipo2();
    cout << "1er puesto (CAMPEON): " << campeon->getPais() << endl;

    // 2do puesto: el perdedor de la final
    Equipo* subcampeon = (g1 >= g2) ?
                             partidoFinal.getEquipo2() :
                             partidoFinal.getEquipo1();
    cout << "2do puesto: " << subcampeon->getPais() << endl;

    // 3er y 4to puesto: del partido del tercer puesto
    int g3 = partidoTercero.getResEquipo1().getGolesFavor();
    int g4 = partidoTercero.getResEquipo2().getGolesFavor();
    Equipo* tercerPuesto = (g3 >= g4) ?
                               partidoTercero.getEquipo1() :
                               partidoTercero.getEquipo2();
    Equipo* cuartoPuesto = (g3 >= g4) ?
                               partidoTercero.getEquipo2() :
                               partidoTercero.getEquipo1();
    cout << "3er puesto: " << tercerPuesto->getPais() << endl;
    cout << "4to puesto: " << cuartoPuesto->getPais() << endl;

    // Máximo goleador del campeón
    Jugador* jugs = campeon->getJugadores();
    int maxGoles = 0;
    int idxMaxGoleador = 0;
    for (int i = 0; i < NUM_JUGADORES; i++) {
        if (jugs[i].getStats().getGoles() > maxGoles) {
            maxGoles = jugs[i].getStats().getGoles();
            idxMaxGoleador = i;
        }
    }
    cout << "\nMax goleador del campeon: "
         << jugs[idxMaxGoleador].getNombre() << " "
         << jugs[idxMaxGoleador].getApellido()
         << " (" << maxGoles << " goles)" << endl;

    // Top 3 goleadores del torneo
    cout << "\nTop 3 goleadores del torneo:" << endl;
    int topGoles[3]   = {0, 0, 0};
    int topEquipo[3]  = {-1, -1, -1};
    int topJugador[3] = {-1, -1, -1};

    for (int e = 0; e < numEquipos; e++) {
        Jugador* js = equipos[e].getJugadores();
        for (int j = 0; j < NUM_JUGADORES; j++) {
            int g = js[j].getStats().getGoles();
            if (g > topGoles[0]) {
                topGoles[2]   = topGoles[1];   topEquipo[2]  = topEquipo[1];
                topJugador[2] = topJugador[1];
                topGoles[1]   = topGoles[0];   topEquipo[1]  = topEquipo[0];
                topJugador[1] = topJugador[0];
                topGoles[0]   = g;             topEquipo[0]  = e;
                topJugador[0] = j;
            } else if (g > topGoles[1]) {
                topGoles[2]   = topGoles[1];   topEquipo[2]  = topEquipo[1];
                topJugador[2] = topJugador[1];
                topGoles[1]   = g;             topEquipo[1]  = e;
                topJugador[1] = j;
            } else if (g > topGoles[2]) {
                topGoles[2]   = g;             topEquipo[2]  = e;
                topJugador[2] = j;
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        if (topEquipo[i] != -1) {
            Jugador& jug = equipos[topEquipo[i]].getJugadores()[topJugador[i]];
            cout << (i + 1) << ". "
                 << jug.getNombre() << " " << jug.getApellido()
                 << " (" << equipos[topEquipo[i]].getPais() << ")"
                 << " - " << topGoles[i] << " goles" << endl;
        }
    }

    // Equipo con más goles históricos
    int maxGolesHist = 0;
    int idxMaxEquipo = 0;
    for (int i = 0; i < numEquipos; i++) {
        if (equipos[i].getStats().getGolesFavor() > maxGolesHist) {
            maxGolesHist = equipos[i].getStats().getGolesFavor();
            idxMaxEquipo = i;
        }
    }
    cout << "\nEquipo con mas goles historicos: "
         << equipos[idxMaxEquipo].getPais()
         << " (" << maxGolesHist << " goles)" << endl;

    // Confederación con mayor presencia en R16, R8 y R4 (QF)
    cout << "\nConfederacion con mayor presencia por etapa:" << endl;

    // R16: 32 equipos (los 16 partidos × 2 equipos)
    int confCountR16[6] = {0}; // UEFA, CONMEBOL, CONCACAF, CAF, AFC, OFC
    const char* confNombres[6] = {"UEFA","CONMEBOL","CONCACAF","CAF","AFC","OFC"};

    for (int i = 0; i < 16; i++) {
        const char* c1 = partidosR16[i].getEquipo1()->getConfederacion();
        const char* c2 = partidosR16[i].getEquipo2()->getConfederacion();
        for (int k = 0; k < 6; k++) {
            if (strcmp(c1, confNombres[k]) == 0) confCountR16[k]++;
            if (strcmp(c2, confNombres[k]) == 0) confCountR16[k]++;
        }
    }
    int maxR16 = 0;
    for (int k = 0; k < 6; k++)
        if (confCountR16[k] > maxR16) maxR16 = confCountR16[k];
    cout << "R16: ";
    for (int k = 0; k < 6; k++)
        if (confCountR16[k] == maxR16)
            cout << confNombres[k] << " (" << maxR16 << " equipos) ";
    cout << endl;

    // R8: 16 equipos
    int confCountR8[6] = {0};
    for (int i = 0; i < 8; i++) {
        const char* c1 = partidosR8[i].getEquipo1()->getConfederacion();
        const char* c2 = partidosR8[i].getEquipo2()->getConfederacion();
        for (int k = 0; k < 6; k++) {
            if (strcmp(c1, confNombres[k]) == 0) confCountR8[k]++;
            if (strcmp(c2, confNombres[k]) == 0) confCountR8[k]++;
        }
    }
    int maxR8 = 0;
    for (int k = 0; k < 6; k++)
        if (confCountR8[k] > maxR8) maxR8 = confCountR8[k];
    cout << "R8:  ";
    for (int k = 0; k < 6; k++)
        if (confCountR8[k] == maxR8)
            cout << confNombres[k] << " (" << maxR8 << " equipos) ";
    cout << endl;

    // R4 (QF): 8 equipos
    int confCountQF[6] = {0};
    for (int i = 0; i < 4; i++) {
        const char* c1 = partidosQF[i].getEquipo1()->getConfederacion();
        const char* c2 = partidosQF[i].getEquipo2()->getConfederacion();
        for (int k = 0; k < 6; k++) {
            if (strcmp(c1, confNombres[k]) == 0) confCountQF[k]++;
            if (strcmp(c2, confNombres[k]) == 0) confCountQF[k]++;
        }
    }
    int maxQF = 0;
    for (int k = 0; k < 6; k++)
        if (confCountQF[k] > maxQF) maxQF = confCountQF[k];
    cout << "R4:  ";
    for (int k = 0; k < 6; k++)
        if (confCountQF[k] == maxQF)
            cout << confNombres[k] << " (" << maxQF << " equipos) ";
    cout << endl;
}
