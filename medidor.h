#ifndef MEDIDOR_H
#define MEDIDOR_H

class Medidor {
private:
    long long iteraciones;

public:
    Medidor();
    void reset();
    void contarIteracion(int cantidad = 1);
    long long getIteraciones() const;

    // Calcula memoria de estructuras principales
    long long calcularMemoria(int numEquipos,
                              int numGrupos,
                              int numPartidosR16,
                              int numPartidosR8,
                              int numPartidosQF,
                              int numPartidosSF) const;

    void imprimirReporte(int numEquipos,
                         int numGrupos) const;
};

#endif // MEDIDOR_H
