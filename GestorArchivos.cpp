#include "GestorArchivos.h"
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

GestorArchivos::GestorArchivos() {}

// Lee el CSV línea por línea y llena el arreglo de equipos
int GestorArchivos::leerCSV(const char* ruta,
                            Equipo* equipos,
                            int maxEquipos) {
    // Abrimos el archivo en modo lectura
    FILE* archivo = fopen(ruta, "r");
    if (archivo == nullptr) {
        cout << "Error: no se pudo abrir el archivo " << ruta << endl;
        return 0;
    }

    char linea[256];
    int count = 0;

    // Saltar la primera línea si es encabezado
    fgets(linea, sizeof(linea), archivo);

    // Leer cada línea del CSV
    while (fgets(linea, sizeof(linea), archivo) && count < maxEquipos) {
        // Eliminar el salto de línea al final
        int len = strlen(linea);
        if (linea[len - 1] == '\n') linea[len - 1] = '\0';

        // Separar los campos por coma
        char pais[50], conf[20], dt[50];
        int ranking, gf, gc, gan, emp, per;

        // Tokenizar la línea por comas
        char* token = strtok(linea, ",");
        if (token == nullptr) continue;
        strncpy(pais, token, sizeof(pais));

        token = strtok(nullptr, ",");
        if (token == nullptr) continue;
        strncpy(conf, token, sizeof(conf));

        token = strtok(nullptr, ",");
        if (token == nullptr) continue;
        strncpy(dt, token, sizeof(dt));

        token = strtok(nullptr, ",");
        if (token == nullptr) continue;
        ranking = atoi(token);

        token = strtok(nullptr, ",");
        if (token == nullptr) continue;
        gf = atoi(token);

        token = strtok(nullptr, ",");
        if (token == nullptr) continue;
        gc = atoi(token);

        token = strtok(nullptr, ",");
        if (token == nullptr) continue;
        gan = atoi(token);

        token = strtok(nullptr, ",");
        if (token == nullptr) continue;
        emp = atoi(token);

        token = strtok(nullptr, ",");
        if (token == nullptr) continue;
        per = atoi(token);

        // Llenar el equipo con los datos leídos
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

// Guarda las estadísticas actualizadas de todos los equipos
// Se llama solo al finalizar el torneo
void GestorArchivos::guardarDatos(const char* ruta,
                                  Equipo* equipos,
                                  int numEquipos) {
    FILE* archivo = fopen(ruta, "w");
    if (archivo == nullptr) {
        cout << "Error: no se pudo guardar en " << ruta << endl;
        return;
    }

    // Escribir encabezado
    fprintf(archivo, "pais,confederacion,dt,ranking,"
                     "golesFavor,golesContra,ganados,empatados,perdidos\n");

    // Escribir cada equipo
    for (int i = 0; i < numEquipos; i++) {
        fprintf(archivo, "%s,%s,%s,%d,%d,%d,%d,%d,%d\n",
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
