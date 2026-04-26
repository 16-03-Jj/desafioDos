#include "GestorArchivos.h"
#include "equipo.h"
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

GestorArchivos::GestorArchivos() {}

int GestorArchivos::leerCSV(const char* ruta,
                            Equipo* equipos,
                            int maxEquipos) {
    FILE* archivo = fopen(ruta, "r");
    if (archivo == nullptr) {
        cout << "Error: no se pudo abrir el archivo " << ruta << endl;
        return 0;
    }

    char linea[256];
    int count = 0;

    // NO saltamos ninguna línea porque ya borramos el encabezado del archivo
    while (fgets(linea, sizeof(linea), archivo) && count < maxEquipos) {
        // Eliminar \n y \r
        int len = strlen(linea);
        if (len > 0 && linea[len - 1] == '\n') linea[--len] = '\0';
        if (len > 0 && linea[len - 1] == '\r') linea[--len] = '\0';

        // Saltar líneas vacías
        if (len == 0) continue;

        char pais[50], dt[60], federacion[80], conf[20];
        int ranking, gf, gc, gan, emp, per;

        char* token = strtok(linea, ";");
        if (!token) continue;
        ranking = atoi(token);

        token = strtok(nullptr, ";");
        if (!token) continue;
        strncpy(pais, token, sizeof(pais) - 1);
        pais[sizeof(pais) - 1] = '\0';

        token = strtok(nullptr, ";");
        if (!token) continue;
        strncpy(dt, token, sizeof(dt) - 1);
        dt[sizeof(dt) - 1] = '\0';

        token = strtok(nullptr, ";");
        if (!token) continue;
        strncpy(federacion, token, sizeof(federacion) - 1);

        token = strtok(nullptr, ";");
        if (!token) continue;
        strncpy(conf, token, sizeof(conf) - 1);
        conf[sizeof(conf) - 1] = '\0';

        token = strtok(nullptr, ";");
        if (!token) continue;
        gf = atoi(token);

        token = strtok(nullptr, ";");
        if (!token) continue;
        gc = atoi(token);

        token = strtok(nullptr, ";");  // <- corregido
        if (!token) continue;
        gan = atoi(token);

        token = strtok(nullptr, ";");
        if (!token) continue;
        emp = atoi(token);

        token = strtok(nullptr, ";");
        if (!token) continue;
        per = atoi(token);

        equipos[count].setPais(pais);
        equipos[count].setConfederacion(conf);
        equipos[count].setDT(dt);
        equipos[count].setRankingFIFA(ranking);
        equipos[count].getStats().setGolesFavor(gf);
        equipos[count].getStats().setGolesContra(gc);
        equipos[count].getStats().setGanados(gan);
        equipos[count].getStats().setEmpatados(emp);
        equipos[count].getStats().setPerdidos(per);

        count++;
    }

    fclose(archivo);
    cout << "CSV leido: " << count << " equipos cargados." << endl;
    return count;
}
void GestorArchivos::guardarJugadores(const char* ruta,
                                      Equipo* equipos,
                                      int numEquipos) {
    FILE* archivo = fopen(ruta, "w");
    if (archivo == nullptr) {
        cout << "Error: no se pudo guardar jugadores en " << ruta << endl;
        return;
    }

    for (int e = 0; e < numEquipos; e++) {
        Jugador* jugs = equipos[e].getJugadores();
        for (int j = 0; j < NUM_JUGADORES; j++) {
            EstadisticasJugador& stats = jugs[j].getStats();
            fprintf(archivo, "%s;%d;%s;%s;%d;%d;%d;%d;%d;%d;%d\n",
                    equipos[e].getPais(),
                    jugs[j].getNumeroCamiseta(),
                    jugs[j].getNombre(),
                    jugs[j].getApellido(),
                    stats.getGoles(),
                    stats.getAsistencias(),
                    stats.getTarjetasAmarillas(),
                    stats.getTarjetasRojas(),
                    stats.getFaltas(),
                    stats.getMinutosJugados(),
                    stats.getPartidosJugados());
        }
    }

    fclose(archivo);
    cout << "Jugadores guardados correctamente." << endl;
}

void GestorArchivos::cargarJugadores(const char* ruta,
                                     Equipo* equipos,
                                     int numEquipos) {
    FILE* archivo = fopen(ruta, "r");
    if (archivo == nullptr) {
        // Si no existe el archivo simplemente no carga
        // Los jugadores se inicializan artificialmente
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        int len = strlen(linea);
        if (len > 0 && linea[len-1] == '\n') linea[--len] = '\0';
        if (len > 0 && linea[len-1] == '\r') linea[--len] = '\0';
        if (len == 0) continue;

        char pais[50], nombre[30], apellido[30];
        int camiseta, goles, asistencias, tarjAm, tarjRoj;
        int faltas, minutos, partidos;

        char* token = strtok(linea, ";");
        if (!token) continue;
        strncpy(pais, token, sizeof(pais)-1);

        token = strtok(nullptr, ";");
        if (!token) continue;
        camiseta = atoi(token);

        token = strtok(nullptr, ";");
        if (!token) continue;
        strncpy(nombre, token, sizeof(nombre)-1);

        token = strtok(nullptr, ";");
        if (!token) continue;
        strncpy(apellido, token, sizeof(apellido)-1);

        token = strtok(nullptr, ";");
        if (!token) continue;
        goles = atoi(token);

        token = strtok(nullptr, ";");
        if (!token) continue;
        asistencias = atoi(token);

        token = strtok(nullptr, ";");
        if (!token) continue;
        tarjAm = atoi(token);

        token = strtok(nullptr, ";");
        if (!token) continue;
        tarjRoj = atoi(token);

        token = strtok(nullptr, ";");
        if (!token) continue;
        faltas = atoi(token);

        token = strtok(nullptr, ";");
        if (!token) continue;
        minutos = atoi(token);

        token = strtok(nullptr, ";");
        if (!token) continue;
        partidos = atoi(token);

        // Buscar el equipo correspondiente
        for (int e = 0; e < numEquipos; e++) {
            if (strcmp(equipos[e].getPais(), pais) == 0) {
                // Buscar el jugador por camiseta
                Jugador* jugs = equipos[e].getJugadores();
                for (int j = 0; j < NUM_JUGADORES; j++) {
                    if (jugs[j].getNumeroCamiseta() == camiseta) {
                        jugs[j].setNombre(nombre);
                        jugs[j].setApellido(apellido);
                        jugs[j].getStats().setGoles(goles);
                        jugs[j].getStats().setAsistencias(asistencias);
                        jugs[j].getStats().setTarjetasAmarillas(tarjAm);
                        jugs[j].getStats().setTarjetasRojas(tarjRoj);
                        jugs[j].getStats().setFaltas(faltas);
                        jugs[j].getStats().setMinutosJugados(minutos);
                        jugs[j].getStats().setPartidosJugados(partidos);
                        break;
                    }
                }
                break;
            }
        }
    }

    fclose(archivo);
    cout << "Jugadores cargados desde archivo." << endl;
}
void GestorArchivos::guardarDatos(const char* ruta,
                                  Equipo* equipos,
                                  int numEquipos) {
    FILE* archivo = fopen(ruta, "w");
    if (archivo == nullptr) {
        cout << "Error: no se pudo guardar en " << ruta << endl;
        return;
    }

    for (int i = 0; i < numEquipos; i++) {
        fprintf(archivo, "%s;%s;%s;%d;%d;%d;%d;%d;%d\n",
                equipos[i].getPais(),
                equipos[i].getConfederacion(),
                equipos[i].getDT(),
                equipos[i].getRankingFIFA(),
                equipos[i].getStats().getGolesFavor(),
                equipos[i].getStats().getGolesContra(),
                equipos[i].getStats().getGanados(),
                equipos[i].getStats().getEmpatados(),
                equipos[i].getStats().getPerdidos());
    }

    fclose(archivo);
    cout << "Datos guardados correctamente." << endl;
}

