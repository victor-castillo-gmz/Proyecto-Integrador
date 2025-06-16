#include "pelicula.h"
#include <iostream>
#include <iomanip> // Already included via video.h, but good to be explicit if needed

Pelicula::Pelicula(const std::string& id, const std::string& nombre, double duracion, const std::string& genero)
    : Video(id, nombre, duracion, genero) {}

void Pelicula::mostrarDatos() const {
    std::cout << "Tipo: Película" << std::endl;
    imprimirInfoBase(); // Reuses the base class's info printing
}