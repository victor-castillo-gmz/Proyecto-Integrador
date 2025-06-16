#ifndef SERVICIOSTREAMING_H
#define SERVICIOSTREAMING_H

#include "video.h"
#include "pelicula.h"
#include "serie.h"
#include "episodio.h" // Ensure Episode is known
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm> // For std::transform

class ServicioStreaming {
private:
    std::vector<Video*> videos;

    // Helper to find a video by title (case-insensitive)
    Video* buscarVideoPorTitulo(const std::string& titulo) const;

public:
    ServicioStreaming() = default; // Default constructor

    // Destructor to free dynamically allocated memory
    ~ServicioStreaming();

    // Load data from file
    void cargarArchivo(const std::string& nombreArchivo);

    // Menu options implementations
    void mostrarVideosPorCalificacionOGenero(double calificacionMinima, const std::string& generoFiltro);
    void mostrarEpisodiosDeSerieConCalificacion(const std::string& nombreSerie, double calificacionMinima);
    void mostrarPeliculasConCalificacion(double calificacionMinima);
    void calificarVideo(const std::string& tituloVideo, int calificacion);
};

#endif // SERVICIOSTREAMING_H