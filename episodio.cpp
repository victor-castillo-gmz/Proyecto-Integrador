#include "episodio.h"
#include <numeric> // For std::accumulate
#include <iostream>
#include <iomanip> // For std::fixed, std::setprecision

Episodio::Episodio(const std::string& titulo, int temporada)
    : titulo(titulo), temporada(temporada) {}

std::string Episodio::getTitulo() const {
    return titulo;
}

int Episodio::getTemporada() const {
    return temporada;
}

double Episodio::getCalificacionPromedio() const {
    if (calificaciones.empty()) {
        return 0.0;
    }
    double sum = std::accumulate(calificaciones.begin(), calificaciones.end(), 0.0);
    return sum / calificaciones.size();
}

void Episodio::calificar(int calificacion) {
    if (calificacion >= 1 && calificacion <= 5) {
        calificaciones.push_back(calificacion);
    } else {
        // No error output here, as ServicioStreaming::parseEpisodios handles the warnings
    }
}

void Episodio::mostrarDatos() const {
    std::cout << "    - Título Episodio: " << titulo << std::endl;
    std::cout << "      Temporada: " << temporada << std::endl;
    // Ensure consistent floating-point output for tests
    std::cout << "      Calificación promedio: " << std::fixed << std::setprecision(1) << getCalificacionPromedio() << std::endl;
}