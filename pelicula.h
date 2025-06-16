#ifndef PELICULA_H
#define PELICULA_H

#include "video.h"
#include <string>

class Pelicula : public Video {
public:
    Pelicula(const std::string& id, const std::string& nombre, double duracion, const std::string& genero);
    ~Pelicula() override = default; // Use override and default for clarity

    // Override the virtual function from Video
    void mostrarDatos() const override;
};

#endif // PELICULA_H