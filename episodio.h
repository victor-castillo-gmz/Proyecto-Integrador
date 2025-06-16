#ifndef EPISODIO_H
#define EPISODIO_H

#include <string>
#include <vector>
#include <numeric> // For std::accumulate
#include <iostream> // For std::cout
#include <iomanip>  // For std::fixed, std::setprecision

class Episodio {
private:
    std::string titulo;
    int temporada;
    std::vector<int> calificaciones;

public:
    Episodio(const std::string& titulo, int temporada);

    // Getters
    std::string getTitulo() const;
    int getTemporada() const;
    double getCalificacionPromedio() const;

    void calificar(int calificacion);

    void mostrarDatos() const;
};

#endif // EPISODIO_H