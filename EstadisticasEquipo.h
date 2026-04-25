#ifndef ESTADISTICASEQUIPO_H
#define ESTADISTICASEQUIPO_H

class EstadisticasEquipo {
private:
    int golesFavor;
    int golesContra;
    int ganados;
    int empatados;
    int perdidos;
    int tarjetasAmarillas;
    int tarjetasRojas;
    int faltas;

public:
    EstadisticasEquipo();
    EstadisticasEquipo(const EstadisticasEquipo& origen);
    EstadisticasEquipo& operator=(const EstadisticasEquipo& origen);

    // Getters
    int getGolesFavor() const;
    int getGolesContra() const;
    int getGanados() const;
    int getEmpatados() const;
    int getPerdidos() const;
    int getTarjetasAmarillas() const;
    int getTarjetasRojas() const;
    int getFaltas() const;

    // Setters
    void setGolesFavor(int g);
    void setGolesContra(int g);
    void setGanados(int g);
    void setEmpatados(int e);
    void setPerdidos(int p);
    void setTarjetasAmarillas(int ta);
    void setTarjetasRojas(int tr);
    void setFaltas(int f);

    // Suma las stats de un partido al histórico del equipo
    void acumular(const EstadisticasEquipo& statsPartido);

    // Promedio de goles a favor por partido (para la ecuación de simulación)
    float promGolesFavor() const;

    // Promedio de goles en contra por partido (para la ecuación de simulación)
    float promGolesContra() const;
};

#endif // ESTADISTICASEQUIPO_H
