#include "torneo.h"
#include "gestorarchivos.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;

// Constructor
Torneo::Torneo() {
    // Reserva memoria para los 48 equipos y 12 grupos
    equipos = new Equipo[NUM_EQUIPOS_TORNEO];
    grupos = new Grupo[NUM_GRUPOS];
    numEquipos = 0;

    // Reserva memoria para cada etapa eliminatoria
    partidosR16 = new Partido[32];
    partidosR8  = new Partido[16];
    partidosQF  = new Partido[8];
    partidosSF  = new Partido[4];

    // Inicializa el generador de aleatorios una sola vez
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
    // Inicializar jugadores de cada equipo artificialmente
    for (int i = 0; i < numEquipos; i++) {
        equipos[i].inicializarJugadores();
    }
    cout << "Datos cargados: " << numEquipos << " equipos." << endl;
}

// Ordena los equipos por ranking FIFA usando burbuja
// Mejor ranking = número más bajo queda primero
void Torneo::ordenarPorRanking() {
    for (int i = 0; i < numEquipos - 1; i++) {
        for (int j = 0; j < numEquipos - 1 - i; j++) {
            if (equipos[j+1] < equipos[j]) {
                // Usa el operator= de Equipo para intercambiar
                Equipo temp = equipos[j];
                equipos[j] = equipos[j+1];
                equipos[j+1] = temp;
            }
        }
    }
}

// Arma los 4 bombos del sorteo
// EE.UU. va directo al bombo 1
// Los demás se ordenan por ranking y se dividen en 4 bombos de 12
void Torneo::armarBombos(int bombos[NUM_BOMBOS][EQUIPOS_POR_BOMBO],
                         int& indiceUSA) {
    // Primero ordenar por ranking
    ordenarPorRanking();

    // Encontrar a EE.UU.
    indiceUSA = -1;
    for (int i = 0; i < numEquipos; i++) {
        if (strcmp(equipos[i].getPais(), "United States") == 0 ||
            strcmp(equipos[i].getPais(), "USA") == 0) {
            indiceUSA = i;
            break;
        }
    }

    // Llenar los bombos saltando a EE.UU.
    // Bombo 1: posiciones 0-11 (sin EE.UU.)
    // Bombo 2: posiciones 12-23
    // Bombo 3: posiciones 24-35
    // Bombo 4: posiciones 36-47
    int contBombo[NUM_BOMBOS] = {0, 0, 0, 0};

    // EE.UU. va al bombo 1 primero
    if (indiceUSA != -1) {
        bombos[0][0] = indiceUSA;
        contBombo[0] = 1;
    }

    // Llenar el resto
    int bomboActual = 0;
    for (int i = 0; i < numEquipos; i++) {
        if (i == indiceUSA) continue; // saltar EE.UU.
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
        // Contar equipos de la misma confederación
        if (strcmp(eq->getConfederacion(), confNuevo) == 0) {
            countConfNuevo++;
        }
        // Contar equipos UEFA
        if (strcmp(eq->getConfederacion(), "UEFA") == 0) {
            countUEFA++;
        }
    }

    // Si es UEFA puede haber máximo 2
    if (strcmp(confNuevo, "UEFA") == 0 && countUEFA >= 2) {
        return false;
    }
    // Si no es UEFA no puede repetirse la confederación
    if (strcmp(confNuevo, "UEFA") != 0 && countConfNuevo >= 1) {
        return false;
    }
    return true;
}

// II - Conforma los 12 grupos del torneo
void Torneo::conformarGrupos() {
    // Armar los bombos
    int bombos[NUM_BOMBOS][EQUIPOS_POR_BOMBO];
    int indiceUSA = -1;
    armarBombos(bombos, indiceUSA);

    // Asignar letra a cada grupo
    char letras[NUM_GRUPOS] = {'A','B','C','D','E','F',
                               'G','H','I','J','K','L'};
    for (int i = 0; i < NUM_GRUPOS; i++) {
        grupos[i].setLetra(letras[i]);
    }

    // Sacar un equipo de cada bombo para cada grupo
    // respetando restricciones de confederación
    bool asignado[NUM_EQUIPOS_TORNEO] = {false};

    for (int bombo = 0; bombo < NUM_BOMBOS; bombo++) {
        // Para cada grupo asignar un equipo del bombo actual
        int intentos = 0;
        for (int g = 0; g < NUM_GRUPOS; g++) {
            bool encontrado = false;
            // Mezclar el bombo aleatoriamente antes de asignar
            for (int k = EQUIPOS_POR_BOMBO - 1; k > 0; k--) {
                int r = rand() % (k + 1);
                int temp = bombos[bombo][k];
                bombos[bombo][k] = bombos[bombo][r];
                bombos[bombo][r] = temp;
            }
            for (int e = 0; e < EQUIPOS_POR_BOMBO && !encontrado; e++) {
                int idx = bombos[bombo][e];
                if (!asignado[idx]) {
                    Equipo* candidato = &equipos[idx];
                    if (puedenCompartirGrupo(candidato,
                                             nullptr,
                                             grupos[g])) {
                        grupos[g].setEquipo(bombo, candidato);
                        asignado[idx] = true;
                        encontrado = true;
                    }
                }
                intentos++;
            }
        }
    }
}

// III - Simula la fase de grupos completa
void Torneo::simularFaseGrupos() {
    for (int i = 0; i < NUM_GRUPOS; i++) {
        grupos[i].configurarPartidos(20); // inicia el 20 de junio
        grupos[i].simularPartidos();
        grupos[i].calcularPuntos();
        grupos[i].clasificar();
    }
    cout << "\n=== FASE DE GRUPOS COMPLETADA ===" << endl;
    imprimirGrupos();
    configurarR16();
}

// Configura los partidos de dieciseisavos según el enunciado
void Torneo::configurarR16() {
    // Clasifican: 12 primeros, 12 segundos, 8 mejores terceros
    // Primeros de grupo vs terceros clasificados
    // Segundos peores vs cabezas de grupo restantes
    // El resto de segundos entre sí

    // Por simplicidad asignamos fecha fija como dice el enunciado
    const char* fechaR16 = "10/07/2026";

    // Primeros de cada grupo
    Equipo* primeros[NUM_GRUPOS];
    Equipo* segundos[NUM_GRUPOS];
    Equipo* terceros[NUM_GRUPOS];

    for (int i = 0; i < NUM_GRUPOS; i++) {
        primeros[i] = grupos[i].getEquipo(
            grupos[i].getOrdenClasificacion(0));
        segundos[i] = grupos[i].getEquipo(
            grupos[i].getOrdenClasificacion(1));
        terceros[i] = grupos[i].getEquipo(
            grupos[i].getOrdenClasificacion(2));
    }

    // Configurar los 32 partidos de R16
    // Primeros (0-11) vs terceros (0-7 clasificados)
    for (int i = 0; i < 12; i++) {
        partidosR16[i].setEquipo1(primeros[i]);
        partidosR16[i].setEquipo2(terceros[i]);
        partidosR16[i].setFecha(fechaR16);
        partidosR16[i].setSede("nombreSede");
        partidosR16[i].setHora("00:00");
        partidosR16[i].setArbitro(0, "codArbitro1");
        partidosR16[i].setArbitro(1, "codArbitro2");
        partidosR16[i].setArbitro(2, "codArbitro3");
        partidosR16[i].setEsEliminatoria(true);
    }
    // Segundos entre sí (12-23)
    for (int i = 0; i < 12; i++) {
        partidosR16[12 + i].setEquipo1(segundos[i]);
        partidosR16[12 + i].setEquipo2(segundos[(i+1) % NUM_GRUPOS]);
        partidosR16[12 + i].setFecha(fechaR16);
        partidosR16[12 + i].setSede("nombreSede");
        partidosR16[12 + i].setHora("00:00");
        partidosR16[12 + i].setArbitro(0, "codArbitro1");
        partidosR16[12 + i].setArbitro(1, "codArbitro2");
        partidosR16[12 + i].setArbitro(2, "codArbitro3");
        partidosR16[12 + i].setEsEliminatoria(true);
    }

    cout << "\n=== PARTIDOS R16 CONFIGURADOS ===" << endl;
    for (int i = 0; i < 32; i++) {
        cout << partidosR16[i].getEquipo1()->getPais()
        << " vs "
        << partidosR16[i].getEquipo2()->getPais()
        << endl;
    }
}

// Simula los partidos de R16
void Torneo::simularR16() {
    cout << "\n=== DIECISEISAVOS DE FINAL ===" << endl;
    for (int i = 0; i < 32; i++) {
        partidosR16[i].simular();
        partidosR16[i].imprimir();
    }
    // Configurar R8 con los ganadores
    for (int i = 0; i < 16; i++) {
        int g1 = partidosR16[i*2].getResEquipo1().getGolesFavor();
        int g2 = partidosR16[i*2].getResEquipo2().getGolesFavor();
        Equipo* ganador1 = g1 >= g2 ?
                               partidosR16[i*2].getEquipo1() :
                               partidosR16[i*2].getEquipo2();

        g1 = partidosR16[i*2+1].getResEquipo1().getGolesFavor();
        g2 = partidosR16[i*2+1].getResEquipo2().getGolesFavor();
        Equipo* ganador2 = g1 >= g2 ?
                               partidosR16[i*2+1].getEquipo1() :
                               partidosR16[i*2+1].getEquipo2();

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

// Simula octavos de final
void Torneo::simularR8() {
    cout << "\n=== OCTAVOS DE FINAL ===" << endl;
    for (int i = 0; i < 16; i++) {
        partidosR8[i].simular();
        partidosR8[i].imprimir();
    }
    // Configurar QF con ganadores
    for (int i = 0; i < 8; i++) {
        int g1 = partidosR8[i*2].getResEquipo1().getGolesFavor();
        int g2 = partidosR8[i*2].getResEquipo2().getGolesFavor();
        Equipo* ganador1 = g1 >= g2 ?
                               partidosR8[i*2].getEquipo1() :
                               partidosR8[i*2].getEquipo2();

        g1 = partidosR8[i*2+1].getResEquipo1().getGolesFavor();
        g2 = partidosR8[i*2+1].getResEquipo2().getGolesFavor();
        Equipo* ganador2 = g1 >= g2 ?
                               partidosR8[i*2+1].getEquipo1() :
                               partidosR8[i*2+1].getEquipo2();

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

// Simula cuartos de final
void Torneo::simularQF() {
    cout << "\n=== CUARTOS DE FINAL ===" << endl;
    for (int i = 0; i < 8; i++) {
        partidosQF[i].simular();
        partidosQF[i].imprimir();
    }
    // Configurar SF con ganadores
    for (int i = 0; i < 4; i++) {
        int g1 = partidosQF[i*2].getResEquipo1().getGolesFavor();
        int g2 = partidosQF[i*2].getResEquipo2().getGolesFavor();
        Equipo* ganador1 = g1 >= g2 ?
                               partidosQF[i*2].getEquipo1() :
                               partidosQF[i*2].getEquipo2();

        g1 = partidosQF[i*2+1].getResEquipo1().getGolesFavor();
        g2 = partidosQF[i*2+1].getResEquipo2().getGolesFavor();
        Equipo* ganador2 = g1 >= g2 ?
                               partidosQF[i*2+1].getEquipo1() :
                               partidosQF[i*2+1].getEquipo2();

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

// Simula semifinales y configura final y tercer puesto
void Torneo::simularSF() {
    cout << "\n=== SEMIFINALES ===" << endl;
    for (int i = 0; i < 4; i++) {
        partidosSF[i].simular();
        partidosSF[i].imprimir();
    }
    // Ganadores van a la final
    // Perdedores van al partido del tercer puesto
    Equipo* ganadores[2];
    Equipo* perdedores[2];
    for (int i = 0; i < 2; i++) {
        int g1 = partidosSF[i].getResEquipo1().getGolesFavor();
        int g2 = partidosSF[i].getResEquipo2().getGolesFavor();
        if (g1 >= g2) {
            ganadores[i] = partidosSF[i].getEquipo1();
            perdedores[i] = partidosSF[i].getEquipo2();
        } else {
            ganadores[i] = partidosSF[i].getEquipo2();
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
    cout << "\n=== ESTADÍSTICAS FINALES ===" << endl;

    // Campeón
    int g1 = partidoFinal.getResEquipo1().getGolesFavor();//todavía falta hacerla
    int g2 = partidoFinal.getResEquipo2().getGolesFavor();// todavía falta hacerla
    Equipo* campeon = g1 >= g2 ?
                          partidoFinal.getEquipo1() :
                          partidoFinal.getEquipo2();
    cout << "CAMPEON: " << campeon->getPais() << endl;

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
    cout << "Max goleador del campeon: "
         << jugs[idxMaxGoleador].getNombre() << " "
         << jugs[idxMaxGoleador].getApellido()
         << " (" << maxGoles << " goles)" << endl;

    // Top 3 goleadores del torneo
    cout << "\nTop 3 goleadores del torneo:" << endl;
    int topGoles[3] = {0, 0, 0};
    int topEquipo[3] = {-1, -1, -1};
    int topJugador[3] = {-1, -1, -1};

    for (int e = 0; e < numEquipos; e++) {
        Jugador* js = equipos[e].getJugadores();
        for (int j = 0; j < NUM_JUGADORES; j++) {
            int g = js[j].getStats().getGoles();
            if (g > topGoles[0]) {
                topGoles[2] = topGoles[1];
                topEquipo[2] = topEquipo[1];
                topJugador[2] = topJugador[1];
                topGoles[1] = topGoles[0];
                topEquipo[1] = topEquipo[0];
                topJugador[1] = topJugador[0];
                topGoles[0] = g;
                topEquipo[0] = e;
                topJugador[0] = j;
            } else if (g > topGoles[1]) {
                topGoles[2] = topGoles[1];
                topEquipo[2] = topEquipo[1];
                topJugador[2] = topJugador[1];
                topGoles[1] = g;
                topEquipo[1] = e;
                topJugador[1] = j;
            } else if (g > topGoles[2]) {
                topGoles[2] = g;
                topEquipo[2] = e;
                topJugador[2] = j;
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        if (topEquipo[i] != -1) {
            Jugador& jug = equipos[topEquipo[i]]
                               .getJugadores()[topJugador[i]];
            cout << (i+1) << ". " << jug.getNombre()
                 << " " << jug.getApellido()
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
}
