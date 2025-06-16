#ifndef PELICULA_H
#define PELICULA_H

/**
 * @file pelicula.h
 * @brief Declaración de la clase Pelicula.
 * @author Tu Nombre
 * @date 2025-06-15
 */

#include "video.h"
#include <string>

/**
 * @class Pelicula
 * @brief Representa una película, que es un tipo de Video.
 *
 * Hereda de la clase Video y proporciona una implementación específica
 * para mostrar los datos de una película.
 */
class Pelicula : public Video {
public:
    /**
     * @brief Constructor de la clase Pelicula.
     * @param id El identificador único de la película.
     * @param nombre El título de la película.
     * @param duracion La duración de la película en minutos.
     * @param genero El género de la película.
     */
    Pelicula(const std::string& id, const std::string& nombre, double duracion, const std::string& genero);

    /**
     * @brief Destructor por defecto.
     */
    ~Pelicula() override = default;

    /**
     * @brief Muestra los datos completos de la película en la consola.
     *
     * Sobrescribe el método virtual de la clase base Video.
     */
    void MostrarDatos() const override;
};

#endif // PELICULA_H