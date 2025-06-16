/**
 * @file pelicula.cpp
 * @brief Implementación de la clase Pelicula.
 * @author Tu Nombre
 * @date 2025-06-15
 */

#include "pelicula.h"
#include <iostream>

Pelicula::Pelicula(const std::string& id, const std::string& nombre, double duracion, const std::string& genero)
    : Video(id, nombre, duracion, genero) {}

void Pelicula::MostrarDatos() const {
    std::cout << "Tipo: Pelicula" << std::endl;
    ImprimirInfoBase();
}