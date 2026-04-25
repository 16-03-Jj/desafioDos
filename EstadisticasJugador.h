#ifndef ESTADISTICASJUGADOR_H
#define ESTADISTICASJUGADOR_H

class EstadisticasJugador {
private:
    int goles;
    int asistencias;
    int tarjetasAmarillas;
    int tarjetasRojas;
    int faltas;
    int minutosJugados;
    int partidosJugados;

public:
    EstadisticasJugador();
    EstadisticasJugador(const EstadisticasJugador& origen);
    EstadisticasJugador& operator=(const EstadisticasJugador& origen);

    // Getters
    int getGoles() const;
    int getAsistencias() const;
    int getTarjetasAmarillas() const;
    int getTarjetasRojas() const;
    int getFaltas() const;
    int getMinutosJugados() const;
    int getPartidosJugados() const;

    // Setters
    void setGoles(int g);
    void setAsistencias(int a);
    void setTarjetasAmarillas(int ta);
    void setTarjetasRojas(int tr);
    void setFaltas(int f);
    void setMinutosJugados(int m);
    void setPartidosJugados(int p);

    // Suma las stats de un partido al histórico
    void acumular(const EstadisticasJugador& statsPartido);

    // Reinicia todo a cero
    void reiniciar();
};

#endif // ESTADISTICASJUGADOR_H
