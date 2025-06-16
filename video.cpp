#include "video.h"
#include <numeric> // For std::accumulate
#include <iostream>
#include <iomanip> // For std::fixed, std::setprecision

Video::Video(const std::string& id, const std::string& nombre, double duracion, const std::string& genero)
    : id(id), nombre(nombre), duracion(duracion), genero(genero) {}

std::string Video::getId() const {
    return id;
}

std::string Video::getNombre() const {
    return nombre;
}

double Video::getDuracion() const {
    return duracion;
}

std::string Video::getGenero() const {
    return genero;
}

double Video::getCalificacionPromedio() const {
    if (calificaciones.empty()) {
        return 0.0;
    }
    double sum = std::accumulate(calificaciones.begin(), calificaciones.end(), 0.0);
    return sum / calificaciones.size();
}

void Video::calificar(int calificacion) {
    if (calificacion >= 1 && calificacion <= 5) {
        calificaciones.push_back(calificacion);
    } else {
        // Optionally, print a warning or throw an exception for invalid ratings
        // std::cerr << "Advertencia: Calificación inválida: " << calificacion << std::endl;
    }
}

void Video::imprimirInfoBase() const {
    std::cout << "ID: " << id << std::endl;
    std::cout << "Nombre: " << nombre << std::endl;
    std::cout << "Duración: " << duracion << " mins" << std::endl;
    std::cout << "Género: " << genero << std::endl;
    // Ensure consistent floating-point output for tests
    std::cout << "Calificación promedio: " << std::fixed << std::setprecision(1) << getCalificacionPromedio() << std::endl;
}

void Video::mostrarDatos() const {
    imprimirInfoBase();
    // Specific implementation for base Video, if needed, otherwise overridden by derived classes
}