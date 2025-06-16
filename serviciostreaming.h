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
 */
class ServicioStreaming {
private:
    std::vector<std::unique_ptr<Video>> videos;
    // Mapas para búsqueda rápida y sensible a mayúsculas/minúsculas
    std::map<std::string, Video*> videosPorTituloLower;
    std::map<std::string, Episodio*> episodiosPorTituloLower;

    // --- Métodos de Ayuda para Parseo ---
    void ParsePeliculaLine(const std::string& line);
    void ParseSerieLine(const std::string& line);
    void ParseRatings(Video& video, const std::string& ratingsStr);
    void ParseEpisodios(Serie& serie, const std::string& episodesStr);

    // Método de utilidad
    std::string ToLower(const std::string& str) const;
    void IndexarContenido();

public:
    ServicioStreaming() = default;
    ~ServicioStreaming() = default;

    /**
     * @brief Carga y procesa un archivo de datos para poblar el catálogo.
     * @param nombreArchivo La ruta del archivo a cargar.
     */
    void CargarArchivo(const std::string& nombreArchivo);

    /**
     * @brief Permite al usuario calificar un video o un episodio por su título.
     * @param titulo El título (no sensible a mayúsculas/minúsculas) a calificar.
     * @param calificacion La calificación a asignar (1-5).
     */
    void CalificarVideo(const std::string& titulo, int calificacion);

    /**
     * @brief Muestra videos filtrados por calificación y/o género.
     * @param calificacionMinima La calificación mínima requerida.
     * @param genero El género para filtrar (no sensible a mayúsculas/minúsculas).
     */
    void MostrarVideosPorCalificacionOGenero(double calificacionMinima, const std::string& genero);

    /**
     * @brief Muestra los episodios de una serie que cumplen con una calificación mínima.
     * @param tituloSerie El título de la serie a buscar (no sensible a mayúsculas/minúsculas).
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