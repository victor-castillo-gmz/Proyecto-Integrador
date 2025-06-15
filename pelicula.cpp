#include "Pelicula.h"

// Constructor
Pelicula::Pelicula(std::string id, std::string nombre, double duracion, std::string genero)
    : Video(id, nombre, duracion, genero) {} // Call base class constructor

// Override the virtual mostrarDatos() from Video
void Pelicula::mostrarDatos() const {
    std::cout << "--- Película ---\n";
    imprimirInfoBase(); // Use the helper from Video
    std::cout << "----------------\n";
}