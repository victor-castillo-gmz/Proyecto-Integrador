#ifndef EPISODIO_H
#define EPISODIO_H

#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <iomanip>

class Episodio {
private:
    std::string titulo;
    int temporada;
    std::vector<int> calificaciones;
    double calificacionPromedio;

public:
    // Constructor
    Episodio(std::string titulo, int temporada);

    // Method to add a rating to the episode
    void calificar(int valor);

    // Calculate and update the average rating
    void calcularPromedio();

    // Display episode details
    void mostrarDatos() const;

    // Getters
    std::string getTitulo() const { return titulo; }
    int getTemporada() const { return temporada; }
    double getCalificacionPromedio() const { return calificacionPromedio; }
    const std::vector<int>& getCalificaciones() const { return calificaciones; } // For saving or accessing
};

#endif // EPISODIO_H
