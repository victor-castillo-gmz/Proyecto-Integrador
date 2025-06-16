#ifndef EPISODIO_H
#define EPISODIO_H

/**
 * @file episodio.h
 * @brief Declaración de la clase Episodio.
 * @author Tu Nombre
 * @date 2025-06-15
 */

#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include <iomanip>

/**
 * @class Episodio
 * @brief Representa un único episodio de una serie.
 *
 * Almacena información sobre el título, temporada y calificaciones de un episodio.
 * No hereda de Video porque un episodio no es un video independiente en este modelo.
 */
class Episodio {
private:
    std::string titulo;
    int temporada;
    std::vector<int> calificaciones;

public:
    /**
     * @brief Constructor de la clase Episodio.
     * @param titulo El título del episodio.
     * @param temporada El número de temporada a la que pertenece el episodio.
     */
    Episodio(const std::string& titulo, int temporada);

    // --- Getters ---

    /** @brief Obtiene el título del episodio. @return El título. */
    std::string GetTitulo() const;
    /** @brief Obtiene el número de temporada. @return El número de temporada. */
    int GetTemporada() const;
    /** @brief Calcula y obtiene la calificación promedio del episodio. @return La calificación promedio. */
    double GetCalificacionPromedio() const;

    /**
     * @brief Agrega una nueva calificación al episodio.
     * @param calificacion Un entero entre 1 y 5.
     */
    void Calificar(int calificacion);

    /**
     * @brief Muestra los datos del episodio en la consola.
     */
    void MostrarDatos() const;
};

#endif // EPISODIO_H
