/**
 * @file video.cpp
 * @brief Implementación de la clase base Video.
 * @author Tu Nombre
 * @date 2025-06-15
 */

#include "video.h"
#include <numeric>
#include <iostream>
#include <iomanip>

Video::Video(const std::string& id, const std::string& nombre, double duracion, const std::string& genero)
    : id(id), nombre(nombre), duracion(duracion), genero(genero) {}

std::string Video::GetId() const {
    return id;
}

std::string Video::GetNombre() const {
    return nombre;
}

double Video::GetDuracion() const {
    return duracion;
}

std::string Video::GetGenero() const {
    return genero;
}

double Video::GetCalificacionPromedio() const {
    if (calificaciones.empty()) {
        return 0.0;
    }
    double sum = std::accumulate(calificaciones.begin(), calificaciones.end(), 0.0);
    return sum / calificaciones.size();
}

void Video::Calificar(int calificacion) {
    if (calificacion >= 1 && calificacion <= 5) {
        calificaciones.push_back(calificacion);
    }
}

void Video::ImprimirInfoBase() const {
    std::cout << "ID: " << id << std::endl;
    std::cout << "Nombre: " << nombre << std::endl;
    std::cout << "Duracion: " << duracion << " mins" << std::endl;
    std::cout << "Genero: " << genero << std::endl;
    std::cout << "Calificacion promedio: " << std::fixed << std::setprecision(1) << GetCalificacionPromedio() << std::endl;
}