#ifndef SERIE_H
#define SERIE_H

#include <vector>
#include <string>
#include "video.h"
#include "episodio.h"

/**
 * @class Serie
 * @brief Representa una serie de videos, que es un tipo de Video que contiene múltiples episodios.
 */
class Serie : public Video {
public:
    /**
     * @brief Constructor de la clase Serie.
     * @param id El ID único de la serie.
     * @param nombre El nombre de la serie.
     * @param duracion La duración promedio de los episodios de la serie en minutos.
     * @param genero El género de la serie.
     */
    Serie(const std::string& id, const std::string& nombre, double duracion, const std::string& genero);

    /**
     * @brief Destructor de la clase Serie.
     */
    ~Serie(); // Undocumented API: ~

    /**
     * @brief Agrega un episodio a la serie.
     * @param episodio El objeto Episodio a agregar.
     */
    void AgregarEpisodio(const Episodio& episodio); // Rename method "agregarEpisodio"

    /**
     * @brief Califica un episodio específico de la serie.
     * @param tituloEpisodio El título del episodio a calificar.
     * @param calificacion La calificación a asignar (1-5).
     */
    void CalificarEpisodio(const std::string& tituloEpisodio, int calificacion); // Undocumented API: calificarEpisodio, Rename method "calificarEpisodio"

    /**
     * @brief Muestra los datos de la serie, incluyendo detalles de sus episodios.
     */
    void mostrarDatos() const override;

    /**
     * @brief Muestra una lista de todos los episodios de la serie.
     */
    void MostrarEpisodios() const;

    /**
     * @brief Muestra los episodios de la serie que tienen una calificación igual o superior a la mínima especificada.
     * @param calificacionMinima La calificación mínima para filtrar los episodios.
     */
    void MostrarEpisodiosConCalificacion(double calificacionMinima) const;

    /**
     * @brief Obtiene una referencia constante al vector de episodios de la serie.
     * @return Una referencia constante al vector de Episodios.
     */
    const std::vector<Episodio>& GetEpisodios() const; // Rename method "getEpisodios", Undocumented API: getEpisodios

private:
    std::vector<Episodio> episodios; ///< Vector que almacena los episodios de la serie.
};

#endif // SERIE_H