/**
 * @file episodio.cpp
 * @brief Implementación de la clase Episodio.
 * @author Tu Nombre
 * @date 2025-06-15
 */

#include "episodio.h"
#include <numeric>
#include <iostream>
#include <iomanip>

Episodio::Episodio(const std::string& titulo, int temporada)
    : titulo(titulo), temporada(temporada) {}

std::string Episodio::GetTitulo() const {
    return titulo;
}

int Episodio::GetTemporada() const {
    return temporada;
}

double Episodio::GetCalificacionPromedio() const {
    if (calificaciones.empty()) {
        return 0.0;
    }
    double sum = std::accumulate(calificaciones.begin(), calificaciones.end(), 0.0);
    return sum / calificaciones.size();
}

void Episodio::Calificar(int calificacion) {
    if (calificacion >= 1 && calificacion <= 5) {
        calificaciones.push_back(calificacion);
    }
}

void Episodio::MostrarDatos() const {
    std::cout << "    - Titulo Episodio: " << titulo << std::endl;
    std::cout << "      Temporada: " << temporada << std::endl;
    std::cout << "      Calificacion promedio: " << std::fixed << std::setprecision(1) << GetCalificacionPromedio() << std::endl;
}
