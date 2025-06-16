#ifndef SERVICIOSTREAMING_H
#define SERVICIOSTREAMING_H

#include <vector>
#include <string>
#include <memory> // For std::unique_ptr
#include "video.h"
#include "episodio.h" // Include Episodio for parseEpisodios

class ServicioStreaming {
public:
    ServicioStreaming();
    ~ServicioStreaming();

    void cargarArchivo(const std::string& nombreArchivo);
    void calificarVideo(const std::string& nombreVideoEpisodio, int calificacion);

    // Methods for displaying information, now const-correct
    void mostrarVideosPorCalificacionOGenero(double calificacionMinima, const std::string& genero) const;
    void mostrarEpisodiosDeSerieConCalificacion(const std::string& nombreSerie, double calificacionMinima) const;
    void mostrarPeliculasConCalificacion(double calificacionMinima) const;

    // Getter for testing purposes
    const std::vector<std::unique_ptr<Video>>& GetVideos() const;

private:
    std::vector<std::unique_ptr<Video>> videos; ///< Almacena punteros inteligentes a objetos Video.

    // Helper methods for parsing
    void parseLine(const std::string& line);
    std::vector<int> parseCalificaciones(const std::string& s);
    // MODIFIED: Added series_name parameter
    std::vector<Episodio> parseEpisodios(const std::string& s, const std::string& series_name);

    // Helper for case-insensitive comparison
    static std::string toLower(const std::string& str);
};

#endif // SERVICIOSTREAMING_H