#ifndef PELICULA_H
#define PELICULA_H

#include "Video.h" // Inherits from Video

class Pelicula : public Video {
private:
    // Removed yearEstreno, director, estudio to align with original problem statement's simplicity
    // and your provided sample code which also didn't use these specific fields for filtering/display.
    // If needed, they can be easily re-added here.

public:
    // Constructor
    Pelicula(std::string id, std::string nombre, double duracion, std::string genero);

    // Override the virtual mostrarDatos() from Video
    void mostrarDatos() const override;
};

#endif // PELICULA_H