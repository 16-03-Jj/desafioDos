#include "Torneo.h"
#include <iostream>
#include <cstdlib>
using namespace std;

// Imprime el menú principal
void imprimirMenu() {
    cout << "\n========================================" << endl;
    cout << "       UdeAWorldCup - FIFA 2026         " << endl;
    cout << "========================================" << endl;
    cout << " 1. Cargar datos del CSV                " << endl;
    cout << " 2. Conformar grupos                    " << endl;
    cout << " 3. Simular fase de grupos              " << endl;
    cout << " 4. Simular dieciseisavos (R16)         " << endl;
    cout << " 5. Simular octavos de final (R8)       " << endl;
    cout << " 6. Simular cuartos de final (QF)       " << endl;
    cout << " 7. Simular semifinales (SF)            " << endl;
    cout << " 8. Simular final y tercer puesto       " << endl;
    cout << " 9. Ver estadisticas finales            " << endl;
    cout << " 0. Salir                               " << endl;
    cout << "========================================" << endl;
    cout << "Seleccione una opcion: ";
}

int main() {
    Torneo torneo;


    // Flags para controlar el orden de ejecución
    bool datosCargados   = false;
    bool gruposFormados  = false;
    bool gruposSimulados = false;
    bool r16Simulado     = false;
    bool r8Simulado      = false;
    bool qfSimulado      = false;
    bool sfSimulado      = false;
    bool finalSimulado   = false;

    int opcion = -1;

    while (opcion != 0) {
        imprimirMenu();
        cin >> opcion;

        switch (opcion) {

        case 1:
            // I - Cargar datos desde CSV
           torneo.cargarDatos("C:\\Users\\JUAN JOSE\\OneDrive\\Documentos\\UdeAWorldCup\\build\\selecciones_clasificadas_mundial.csv");
            datosCargados = true;
            break;

        case 2:
            // II - Conformar grupos
            if (!datosCargados) {
                cout << "Primero debe cargar los datos (opcion 1)." << endl;
                break;
            }
            torneo.conformarGrupos();
            torneo.imprimirGrupos();
            gruposFormados = true;
            break;

        case 3:
            // III - Simular fase de grupos
            if (!gruposFormados) {
                cout << "Primero debe conformar los grupos (opcion 2)." << endl;
                break;
            }
            torneo.simularFaseGrupos();
            gruposSimulados = true;
            break;

        case 4:
            // III - Simular R16
            if (!gruposSimulados) {
                cout << "Primero debe simular la fase de grupos (opcion 3)." << endl;
                break;
            }
            torneo.simularR16();
            r16Simulado = true;
            break;

        case 5:
            // III - Simular R8
            if (!r16Simulado) {
                cout << "Primero debe simular los dieciseisavos (opcion 4)." << endl;
                break;
            }
            torneo.simularR8();
            r8Simulado = true;
            break;

        case 6:
            // III - Simular QF
            if (!r8Simulado) {
                cout << "Primero debe simular los octavos (opcion 5)." << endl;
                break;
            }
            torneo.simularQF();
            qfSimulado = true;
            break;

        case 7:
            // III - Simular SF
            if (!qfSimulado) {
                cout << "Primero debe simular los cuartos (opcion 6)." << endl;
                break;
            }
            torneo.simularSF();
            sfSimulado = true;
            break;

        case 8:
            // III - Simular Final y Tercer Puesto
            if (!sfSimulado) {
                cout << "Primero debe simular las semifinales (opcion 7)." << endl;
                break;
            }
            torneo.simularFinal();
            finalSimulado = true;
            break;

        case 9:
            // IV - Estadísticas finales
            if (!finalSimulado) {
                cout << "Primero debe completar el torneo (opciones 3 a 8)." << endl;
                break;
            }
            torneo.imprimirEstadisticas();
            break;

        case 0:
            torneo.guardarTodo();
            cout << "Saliendo de UdeAWorldCup..." << endl;
            break;
            cout << "Saliendo de UdeAWorldCup..." << endl;
            break;

        default:
            cout << "Opcion invalida. Intente de nuevo." << endl;
            break;
        }
    }

    return 0;


}
