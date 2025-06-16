#ifndef SERVICIOSTREAMING_H
#define SERVICIOSTREAMING_H

/**
 * @file serviciostreaming.h
 * @brief Declaración de la clase ServicioStreaming.
 * @author Tu Nombre
 * @date 2025-06-15
 */

#include "video.h"
#include "serie.h"
#include <vector>
#include <memory>
#include <string>
#include <map>

/**
 * @class ServicioStreaming
 * @brief Gestiona el catálogo de videos y las interacciones del usuario.
 *
 * Esta clase se encarga de cargar datos desde archivos, almacenar videos
 * (películas y series), y proporcionar métodos para buscar, filtrar y calificar
 * el contenido.
 */
class ServicioStreaming {
private:
    std::vector<std::unique_ptr<Video>> videos;
    std::map<std::string, Video*> videosPorId;
    std::map<std::string, Episodio*> episodiosPorTitulo;

    // Métodos privados para refactorizar la carga de archivos
    void ParseLine(const std::string& line);
    void ParseMovie(std::stringstream& ss);
    void ParseSeries(std::stringstream& ss);
    void ParseRatings(std::stringstream& ss);
    void ParseEpisodes(std::stringstream& ss);
    void IndexVideos();
    std::string ToLower(std::string s) const;

public:
    /**
     * @brief Constructor de ServicioStreaming.
     */
    ServicioStreaming() = default;

    /**
     * @brief Destructor. Limpia la memoria de los videos.
     */
    ~ServicioStreaming() = default;

    /**
     * @brief Carga y procesa un archivo de datos para poblar el catálogo.
     * @param nombreArchivo La ruta del archivo a cargar.
     */
    void CargarArchivo(const std::string& nombreArchivo);

    /**
     * @brief Permite al usuario calificar un video o un episodio por su título.
     * @param titulo El título del video o episodio a calificar.
     * @param calificacion La calificación a asignar (1-5).
     */
    void CalificarVideo(const std::string& titulo, int calificacion);

    /**
     * @brief Muestra videos filtrados por calificación y/o género.
     * @param calificacionMinima La calificación mínima requerida.
     * @param genero El género para filtrar (dejar vacío para no filtrar por género).
     */
    void MostrarVideosPorCalificacionOGenero(double calificacionMinima, const std::string& genero);

    /**
     * @brief Muestra los episodios de una serie que cumplen con una calificación mínima.
     * @param tituloSerie El título de la serie a buscar.
     * @param calificacionMinima La calificación mínima para los episodios.
     */
    void MostrarEpisodiosDeSerieConCalificacion(const std::string& tituloSerie, double calificacionMinima);

    /**
     * @brief Muestra todas las películas que cumplen con una calificación mínima.
     * @param calificacionMinima La calificación mínima requerida.
     */
    void MostrarPeliculasConCalificacion(double calificacionMinima);
};

#endif // SERVICIOSTREAMING_H