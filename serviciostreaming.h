#ifndef SERVICIOSTREAMING_H
#define SERVICIOSTREAMING_H

#include <vector>
#include <string>
#include <memory> // For std::unique_ptr
#include "video.h"
#include "pelicula.h"
#include "serie.h"

/**
 * @class ServicioStreaming
 * @brief Clase que gestiona una colección de videos (Películas y Series)
 * y proporciona funcionalidades de carga, calificación y visualización.
 */
class ServicioStreaming {
public:
    /**
     * @brief Constructor por defecto de ServicioStreaming.
     */
    ServicioStreaming();

    /**
     * @brief Destructor de ServicioStreaming.
     * Libera la memoria de los objetos Video gestionados.
     */
    ~ServicioStreaming();

    /**
     * @brief Carga datos de videos desde un archivo CSV.
     * @param nombreArchivo El nombre del archivo a cargar.
     */
    void cargarArchivo(const std::string& nombreArchivo);

    /**
     * @brief Califica un video o un episodio por su nombre/título.
     * @param nombreVideoEpisodio El nombre del video o el título del episodio a calificar.
     * @param calificacion La calificación a asignar (1-5).
     */
    void calificarVideo(const std::string& nombreVideoEpisodio, int calificacion);

    /**
     * @brief Muestra videos filtrados por calificación mínima y/o género.
     * @param calificacionMinima La calificación mínima para filtrar (0.0 para no filtrar por calificación).
     * @param genero El género para filtrar (cadena vacía para no filtrar por género).
     */
    void mostrarVideosPorCalificacionOGenero(double calificacionMinima, const std::string& genero) const;

    /**
     * @brief Muestra todos los episodios de una serie específica que cumplen con una calificación mínima.
     * @param nombreSerie El nombre de la serie.
     * @param calificacionMinima La calificación mínima de los episodios a mostrar.
     */
    void mostrarEpisodiosDeSerieConCalificacion(const std::string& nombreSerie, double calificacionMinima) const;

    /**
     * @brief Muestra todas las películas que cumplen con una calificación mínima.
     * @param calificacionMinima La calificación mínima de las películas a mostrar.
     */
    void mostrarPeliculasConCalificacion(double calificacionMinima) const;

    /**
     * @brief Obtiene una referencia constante al vector de punteros a Video.
     * @return Una referencia constante al vector de std::unique_ptr<Video>.
     */
    const std::vector<std::unique_ptr<Video>>& GetVideos() const; // Rename method "getVideos", Undocumented API: getVideos

private:
    std::vector<std::unique_ptr<Video>> videos; ///< Almacena punteros inteligentes a objetos Video.

    // Helper methods for parsing
    void parseLine(const std::string& line);
    std::vector<int> parseCalificaciones(const std::string& s);
    std::vector<Episodio> parseEpisodios(const std::string& s);

    // Helper for case-insensitive comparison
    static std::string toLower(const std::string& str);
};

#endif // SERVICIOSTREAMING_H