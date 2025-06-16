#ifndef SERIE_H
#define SERIE_H

#include "video.h"   // Inherits from Video
#include "episodio.h" // Uses Episodio objects
#include <vector>
#include <string> // Agregado: Es buena práctica incluir <string> cuando se usa std::string

class Serie : public Video {
private:
    std::vector<Episodio> episodios;

public:
    // Constructor
    // MODIFICADO: Cambiado a 'const std::string&' para que coincida con la definición en serie.cpp
    Serie(const std::string& id, const std::string& nombre, double duracion, const std::string& genero);

    // MODIFICADO: Añadida la declaración del destructor.
    // Es importante que el destructor sea 'virtual' si 'Video' tiene funciones virtuales
    // o si 'Serie' puede ser la base de otras clases derivadas.
    // Si 'Video' no tiene funciones virtuales, solo sería '~Serie();'
    // Asumiré que 'Video' es polimórfica o se espera que lo sea para una buena práctica.
    virtual ~Serie();

    // Add an episode to the series
    void agregarEpisodio(const Episodio& ep);

    // Override the virtual mostrarDatos() from Video
    void mostrarDatos() const override;

    // Display details for all episodes in the series
    void mostrarEpisodios() const;

    // Display episodes with a minimum rating
    void mostrarEpisodiosConCalificacion(double calificacionMinima) const;

    // Rate a specific episode by title
    // MODIFICADO: Cambiado 'valor' a 'calificacion' para consistencia con los tests previos.
    // Aunque esto no fue la causa del error de compilación actual, es una buena práctica.
    void calificarEpisodio(const std::string& tituloEpisodio, int calificacion);

    // Getter for episodes (to allow external modification/access for data loading)
    // MODIFICADO: Añadido 'const' a la función getter de referencia para permitir acceso const
    // y mantener la versión no-const para modificación. O, si solo se necesita leer,
    // se podría dejar solo la versión const. Para los tests, la versión const es suficiente
    // para los ASSERT_EQ en getCalificacionPromedio(). La versión no-const es útil
    // si necesitas modificar el vector 'episodios' directamente desde fuera (ej. en ServicioStreaming al cargar).
    const std::vector<Episodio>& getEpisodios() const { return episodios; }
    // Puedes mantener también: std::vector<Episodio>& getEpisodios() { return episodios; }
    // si el código que llama a esta función necesita modificar el vector.
    // Por ahora, para evitar complejidades innecesarias en el header, dejaremos la versión const.
    // Si 'ServicioStreaming' necesita modificar el vector de episodios directamente (más allá de calificar),
    // se deberá añadir la versión no-const aquí también.
};

#endif // SERIE_H