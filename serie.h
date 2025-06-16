#ifndef SERIE_H
#define SERIE_H

/**
 * @file serie.h
 * @brief Declaración de la clase Serie.
 * @author Tu Nombre
 * @date 2025-06-15
 */

#include "video.h"
#include "episodio.h"
#include <vector>
#include <string>

/**
 * @class Serie
 * @brief Representa una serie de TV, que es un tipo de Video y contiene episodios.
 *
 * Hereda de Video y gestiona una colección de objetos Episodio.
 */
class Serie : public Video {
private:
    std::vector<Episodio> episodios;

public:
    /**
     * @brief Constructor de la clase Serie.
     * @param id El identificador único de la serie.
     * @param nombre El título de la serie.
     * @param duracion Duración promedio por episodio (o no utilizado).
     * @param genero El género de la serie.
     */
    Serie(const std::string& id, const std::string& nombre, double duracion, const std::string& genero);
    
    /**
     * @brief Destructor por defecto.
     */
    ~Serie() override = default;

    /**
     * @brief Agrega un episodio a la serie.
     * @param episodio El objeto Episodio a agregar.
     */
    void AgregarEpisodio(const Episodio& episodio);

    /**
     * @brief Obtiene una referencia constante al vector de episodios.
     * @return Una referencia al vector de episodios.
     */
    const std::vector<Episodio>& GetEpisodios() const;
    
    /**
     * @brief Obtiene una referencia mutable al vector de episodios.
     * @return Una referencia mutable al vector de episodios.
     */
    std::vector<Episodio>& GetEpisodiosMutables();

    /**
     * @brief Muestra los datos completos de la serie, incluyendo sus episodios.
     *
     * Sobrescribe el método virtual de la clase base Video.
     */
    void MostrarDatos() const override;

    /**
     * @brief Muestra todos los episodios de la serie.
     */
    void MostrarEpisodios() const;

    /**
     * @brief Muestra solo los episodios que cumplen con una calificación mínima.
     * @param calificacionMinima La calificación mínima requerida.
     */
    void MostrarEpisodiosConCalificacion(double calificacionMinima) const;
};

#endif // SERIE_H