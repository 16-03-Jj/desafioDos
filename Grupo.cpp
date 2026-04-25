#include "Grupo.h"
#include <iostream>
#include <cstring>
using namespace std;

// Constructor por defecto
Grupo::Grupo() {
    letra = '?';
    for (int i = 0; i < NUM_EQUIPOS_GRUPO; i++) {
        equipos[i] = nullptr;
        puntos[i] = 0;
        ordenClasificacion[i] = i;
    }
}

// Constructor de copia
Grupo::Grupo(const Grupo& origen) {
    letra = origen.letra;
    for (int i = 0; i < NUM_EQUIPOS_GRUPO; i++) {
        // Solo copiamos el puntero, no el equipo
        equipos[i] = origen.equipos[i];
        puntos[i] = origen.puntos[i];
        ordenClasificacion[i] = origen.ordenClasificacion[i];
    }
    for (int i = 0; i < NUM_PARTIDOS_GRUPO; i++) {
        partidos[i] = origen.partidos[i];
    }
}

// Operador de asignación
Grupo& Grupo::operator=(const Grupo& origen) {
    if (this != &origen) {
        letra = origen.letra;
        for (int i = 0; i < NUM_EQUIPOS_GRUPO; i++) {
            equipos[i] = origen.equipos[i];
            puntos[i] = origen.puntos[i];
            ordenClasificacion[i] = origen.ordenClasificacion[i];
        }
        for (int i = 0; i < NUM_PARTIDOS_GRUPO; i++) {
            partidos[i] = origen.partidos[i];
        }
    }
    return *this;
}

// Permite acceder a un equipo con grupo[i]
Equipo* Grupo::operator[](int indice) const {
    return equipos[indice];
}

// Getters
char Grupo::getLetra() const { return letra; }
Equipo* Grupo::getEquipo(int indice) const { return equipos[indice]; }
Partido& Grupo::getPartido(int indice) { return partidos[indice]; }
int Grupo::getPuntos(int indice) const { return puntos[indice]; }
int Grupo::getOrdenClasificacion(int indice) const {
    return ordenClasificacion[indice];
}

// Setters
void Grupo::setLetra(char l) { letra = l; }
void Grupo::setEquipo(int indice, Equipo* equipo) {
    equipos[indice] = equipo;
}

// Configura los 6 partidos del grupo con fechas válidas
// Restricciones del enunciado:
// - Máximo 4 partidos por día en todo el torneo
// - Ningún equipo juega con menos de 3 días de diferencia
void Grupo::configurarPartidos(int fechaInicio) {
    // Los 6 partidos se arman con todas las combinaciones posibles
    // de los 4 equipos: (0,1),(0,2),(0,3),(1,2),(1,3),(2,3)
    int eq1[6] = {0, 0, 0, 1, 1, 2};
    int eq2[6] = {1, 2, 3, 2, 3, 3};

    // Asignamos fechas respetando que cada equipo
    // descanse al menos 3 días entre partidos
    // Día relativo desde fechaInicio para cada partido
    int diasPartido[6] = {0, 1, 2, 3, 4, 5};

    for (int i = 0; i < NUM_PARTIDOS_GRUPO; i++) {
        partidos[i].setEquipo1(equipos[eq1[i]]);
        partidos[i].setEquipo2(equipos[eq2[i]]);
        partidos[i].setSede("nombreSede");
        partidos[i].setHora("00:00");
        partidos[i].setArbitro(0, "codArbitro1");
        partidos[i].setArbitro(1, "codArbitro2");
        partidos[i].setArbitro(2, "codArbitro3");
        partidos[i].setEsEliminatoria(false);

        // Calcular fecha: fechaInicio + diasPartido[i]
        // Formato simple: "20/06/2026", "21/06/2026", etc.
        char fecha[12];
        int dia = fechaInicio + diasPartido[i];
        snprintf(fecha, sizeof(fecha), "%02d/06/2026", dia);
        partidos[i].setFecha(fecha);
    }
}

// Simula los 6 partidos del grupo uno por uno
void Grupo::simularPartidos() {
    for (int i = 0; i < NUM_PARTIDOS_GRUPO; i++) {
        partidos[i].simular();
    }
}

// Calcula los puntos de cada equipo según los resultados
// 3 puntos por victoria, 1 por empate, 0 por derrota
void Grupo::calcularPuntos() {
    // Reiniciar puntos
    for (int i = 0; i < NUM_EQUIPOS_GRUPO; i++) {
        puntos[i] = 0;
    }
    // Los 6 partidos con sus combinaciones de equipos
    int eq1[6] = {0, 0, 0, 1, 1, 2};
    int eq2[6] = {1, 2, 3, 2, 3, 3};

    for (int i = 0; i < NUM_PARTIDOS_GRUPO; i++) {
        int g1 = partidos[i].getResEquipo1().getGolesFavor();
        int g2 = partidos[i].getResEquipo2().getGolesFavor();
        if (g1 > g2) {
            // Gana equipo1
            puntos[eq1[i]] += 3;
        } else if (g2 > g1) {
            // Gana equipo2
            puntos[eq2[i]] += 3;
        } else {
            // Empate
            puntos[eq1[i]] += 1;
            puntos[eq2[i]] += 1;
        }
    }
}

// Ordena los equipos por puntos aplicando criterios de desempate:
// 1. Puntos
// 2. Diferencia de goles
// 3. Goles a favor
// 4. Sorteo (no implementado, queda pendiente)
void Grupo::clasificar() {
    // Inicializar orden
    for (int i = 0; i < NUM_EQUIPOS_GRUPO; i++) {
        ordenClasificacion[i] = i;
    }
    // Ordenamiento burbuja por puntos y criterios de desempate
    for (int i = 0; i < NUM_EQUIPOS_GRUPO - 1; i++) {
        for (int j = 0; j < NUM_EQUIPOS_GRUPO - 1 - i; j++) {
            int a = ordenClasificacion[j];
            int b = ordenClasificacion[j + 1];

            // Criterio 1: puntos
            bool intercambiar = false;
            if (puntos[a] < puntos[b]) {
                intercambiar = true;
            } else if (puntos[a] == puntos[b]) {
                // Criterio 2: diferencia de goles
                int difA = equipos[a]->getStats().getGolesFavor()
                           - equipos[a]->getStats().getGolesContra();
                int difB = equipos[b]->getStats().getGolesFavor()
                           - equipos[b]->getStats().getGolesContra();
                if (difA < difB) {
                    intercambiar = true;
                } else if (difA == difB) {
                    // Criterio 3: goles a favor
                    if (equipos[a]->getStats().getGolesFavor()
                        < equipos[b]->getStats().getGolesFavor()) {
                        intercambiar = true;
                    }
                }
            }
            // Intercambiar posiciones
            if (intercambiar) {
                int temp = ordenClasificacion[j];
                ordenClasificacion[j] = ordenClasificacion[j + 1];
                ordenClasificacion[j + 1] = temp;
            }
        }
    }
}

// Imprime la tabla de posiciones del grupo
void Grupo::imprimirTabla() const {
    cout << "\n=== GRUPO " << letra << " ===" << endl;
    cout << "Pos | Pais            | Pts | GF | GC | DIF" << endl;
    cout << "----+-----------------+-----+----+----+----" << endl;
    for (int i = 0; i < NUM_EQUIPOS_GRUPO; i++) {
        int idx = ordenClasificacion[i];
        Equipo* eq = equipos[idx];
        int gf = eq->getStats().getGolesFavor();
        int gc = eq->getStats().getGolesContra();
        cout << "  " << (i + 1) << " | "
             << eq->getPais();
        // Espacios para alinear la tabla
        int espacios = 16 - strlen(eq->getPais());
        for (int s = 0; s < espacios; s++) cout << " ";
        cout << "| " << puntos[idx]
             << "   | " << gf
             << "  | " << gc
             << "  | " << (gf - gc)
             << endl;
    }
}
