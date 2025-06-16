#ifndef VIDEO_H
#define VIDEO_H

/**
 * @file video.h
 * @brief Declaración de la clase base abstracta Video.
 * @author Tu Nombre
 * @date 2025-06-15
 */

#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include <iomanip>

/**
 * @class Video
 * @brief Clase base para representar contenido de video en el servicio de streaming.
 *
 * Esta clase es abstracta y proporciona la interfaz común para diferentes tipos
 * de videos, como películas y series.
 */
class Video {
protected:
    std::string id;
    std::string nombre;
    double duracion;
    std::string genero;
    std::vector<int> calificaciones;

    /**
     * @brief Imprime la información base común a todos los videos.
     *
     * Este método es utilizado por las clases derivadas para mostrar datos
     * como ID, nombre, duración, género y calificación promedio.
     */
    void ImprimirInfoBase() const;

public:
    /**
     * @brief Constructor de la clase Video.
     * @param id El identificador único del video.
     * @param nombre El nombre o título del video.
     * @param duracion La duración del video en minutos.
     * @param genero El género del video.
     */
    Video(const std::string& id, const std::string& nombre, double duracion, const std::string& genero);

    /**
     * @brief Destructor virtual por defecto.
     *
     * Asegura la limpieza correcta de los recursos en clases derivadas.
     */
    virtual ~Video() = default;

    // --- Getters ---

    /** @brief Obtiene el ID del video. @return El ID. */
    std::string GetId() const;
    /** @brief Obtiene el nombre del video. @return El nombre. */
    std::string GetNombre() const;
    /** @brief Obtiene la duración del video. @return La duración en minutos. */
    double GetDuracion() const;
    /** @brief Obtiene el género del video. @return El género. */
    std::string GetGenero() const;
    /** @brief Calcula y obtiene la calificación promedio del video. @return La calificación promedio (0.0 si no hay calificaciones). */
    double GetCalificacionPromedio() const;

    /**
     * @brief Agrega una nueva calificación al video.
     * @param calificacion Un entero entre 1 y 5. Calificaciones fuera de este rango son ignoradas.
     */
    void Calificar(int calificacion);

    /**
     * @brief Muestra los datos completos del video.
     *
     * Esta es una función virtual pura que debe ser implementada por las clases derivadas.
     */
    virtual void MostrarDatos() const = 0;
};

#endif // VIDEO_H