#include "Grupo.h"
#include <iostream>
#include <cstring>
using namespace std;

Grupo::Grupo() {
    letra = '?';
    for (int i = 0; i < NUM_EQUIPOS_GRUPO; i++) {
        equipos[i] = nullptr;
        puntos[i] = 0;
        ordenClasificacion[i] = i;
    }
}

Grupo::Grupo(const Grupo& origen) {
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

Equipo* Grupo::operator[](int indice) const { return equipos[indice]; }
char Grupo::getLetra() const { return letra; }
Equipo* Grupo::getEquipo(int indice) const { return equipos[indice]; }
Partido& Grupo::getPartido(int indice) { return partidos[indice]; }
int Grupo::getPuntos(int indice) const { return puntos[indice]; }
int Grupo::getOrdenClasificacion(int indice) const { return ordenClasificacion[indice]; }
void Grupo::setLetra(char l) { letra = l; }
void Grupo::setEquipo(int indice, Equipo* equipo) { equipos[indice] = equipo; }

void Grupo::configurarPartidos(int diaInicio) {
    int eq1[6] = {0, 2, 0, 1, 0, 1};
    int eq2[6] = {1, 3, 2, 3, 3, 2};
    int dias[6] = {0, 0, 4, 4, 8, 8};
    for (int i = 0; i < NUM_PARTIDOS_GRUPO; i++) {
        if (equipos[eq1[i]] == nullptr || equipos[eq2[i]] == nullptr) {
            cout << "Grupo " << letra
                 << ": equipo nulo en partido " << i << endl;
            continue;
        }
        partidos[i].setEquipo1(equipos[eq1[i]]);
        partidos[i].setEquipo2(equipos[eq2[i]]);
        partidos[i].setSede("nombreSede");
        partidos[i].setHora("00:00");
        partidos[i].setArbitro(0, "codArbitro1");
        partidos[i].setArbitro(1, "codArbitro2");
        partidos[i].setArbitro(2, "codArbitro3");
        partidos[i].setEsEliminatoria(false);
        char fecha[12];
        int dia = diaInicio + dias[i];
        snprintf(fecha, sizeof(fecha), "%02d/06/2026", dia);
        partidos[i].setFecha(fecha);
    }
}

void Grupo::simularPartidos() {
    for (int i = 0; i < NUM_PARTIDOS_GRUPO; i++) {
        partidos[i].simular();
    }
}

void Grupo::calcularPuntos() {
    for (int i = 0; i < NUM_EQUIPOS_GRUPO; i++) puntos[i] = 0;
    int eq1[6] = {0, 2, 0, 1, 0, 1};
    int eq2[6] = {1, 3, 2, 3, 3, 2};
    for (int i = 0; i < NUM_PARTIDOS_GRUPO; i++) {
        int g1 = partidos[i].getResEquipo1().getGolesFavor();
        int g2 = partidos[i].getResEquipo2().getGolesFavor();
        if (g1 > g2)      { puntos[eq1[i]] += 3; }
        else if (g2 > g1) { puntos[eq2[i]] += 3; }
        else              { puntos[eq1[i]] += 1; puntos[eq2[i]] += 1; }
    }
}

void Grupo::clasificar() {
    for (int i = 0; i < NUM_EQUIPOS_GRUPO; i++) ordenClasificacion[i] = i;
    int eq1idx[6] = {0, 2, 0, 1, 0, 1};
    int eq2idx[6] = {1, 3, 2, 3, 3, 2};
    int gfGrupo[NUM_EQUIPOS_GRUPO] = {0};
    int gcGrupo[NUM_EQUIPOS_GRUPO] = {0};
    for (int i = 0; i < NUM_PARTIDOS_GRUPO; i++) {
        int g1 = partidos[i].getResEquipo1().getGolesFavor();
        int g2 = partidos[i].getResEquipo2().getGolesFavor();
        gfGrupo[eq1idx[i]] += g1; gcGrupo[eq1idx[i]] += g2;
        gfGrupo[eq2idx[i]] += g2; gcGrupo[eq2idx[i]] += g1;
    }
    for (int i = 0; i < NUM_EQUIPOS_GRUPO - 1; i++) {
        for (int j = 0; j < NUM_EQUIPOS_GRUPO - 1 - i; j++) {
            int a = ordenClasificacion[j];
            int b = ordenClasificacion[j + 1];
            bool intercambiar = false;
            if (puntos[a] < puntos[b]) {
                intercambiar = true;
            } else if (puntos[a] == puntos[b]) {
                int difA = gfGrupo[a] - gcGrupo[a];
                int difB = gfGrupo[b] - gcGrupo[b];
                if (difA < difB) {
                    intercambiar = true;
                } else if (difA == difB) {
                    if (gfGrupo[a] < gfGrupo[b]) intercambiar = true;
                    else if (gfGrupo[a] == gfGrupo[b])
                        if (rand() % 2 == 0) intercambiar = true;
                }
            }
            if (intercambiar) {
                int temp = ordenClasificacion[j];
                ordenClasificacion[j] = ordenClasificacion[j + 1];
                ordenClasificacion[j + 1] = temp;
            }
        }
    }
}

void Grupo::imprimirTabla() const {
    int eq1idx[6] = {0, 2, 0, 1, 0, 1};
    int eq2idx[6] = {1, 3, 2, 3, 3, 2};
    int gfGrupo[NUM_EQUIPOS_GRUPO] = {0};
    int gcGrupo[NUM_EQUIPOS_GRUPO] = {0};
    for (int i = 0; i < NUM_PARTIDOS_GRUPO; i++) {
        int g1 = partidos[i].getResEquipo1().getGolesFavor();
        int g2 = partidos[i].getResEquipo2().getGolesFavor();
        gfGrupo[eq1idx[i]] += g1; gcGrupo[eq1idx[i]] += g2;
        gfGrupo[eq2idx[i]] += g2; gcGrupo[eq2idx[i]] += g1;
    }
    cout << "\n=== GRUPO " << letra << " ===" << endl;
    cout << "Pos | Pais            | Pts | GF | GC | DIF" << endl;
    cout << "----+-----------------+-----+----+----+----" << endl;
    for (int i = 0; i < NUM_EQUIPOS_GRUPO; i++) {
        int idx = ordenClasificacion[i];
        Equipo* eq = equipos[idx];
        if (eq == nullptr) {
            cout << "  " << (i+1) << " | SIN ASIGNAR     | 0   | 0  | 0  | 0" << endl;
            continue;
        }
        cout << "  " << (i+1) << " | " << eq->getPais();
        int largoPais = strlen(eq->getPais());
        int espacios = (largoPais < 16) ? 16 - largoPais : 1;
        for (int s = 0; s < espacios; s++) cout << " ";
        cout << "| " << puntos[idx]
             << "   | " << gfGrupo[idx]
             << "  | " << gcGrupo[idx]
             << "  | " << (gfGrupo[idx] - gcGrupo[idx])
             << endl;
    }
}
