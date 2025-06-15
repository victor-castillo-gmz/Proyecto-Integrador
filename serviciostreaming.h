#ifndef SERVICIOSTREAMING_H
#define SERVICIOSTREAMING_H

#include <vector>
#include <string>
#include "Pelicula.h"
#include "Serie.h"

class ServicioStreaming {
private:
    std::vector<Pelicula> peliculas;
    std::vector<Serie> series;

public:
    void agregarPelicula(const Pelicula& pelicula);
    void agregarSerie(const Serie& serie);

    void calificarVideo(const std::string& titulo, int calificacion);
    double obtenerCalificacion(const std::string& titulo) const;
};

#endif // SERVICIOSTREAMING_H
