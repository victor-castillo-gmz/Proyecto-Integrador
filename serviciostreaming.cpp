#include "serviciostreaming.h"
#include "pelicula.h"
#include "serie.h"
#include <iostream>
#include <fstream> // Required for std::ifstream
#include <sstream>
#include <algorithm> // Required for std::transform
#include <memory>    // Required for std::make_unique

// Helper function (already defined in .h and used in .cpp)
// Convert string to lowercase for case-insensitive comparison
std::string ServicioStreaming::toLower(const std::string& str) {
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lower_str;
}

ServicioStreaming::ServicioStreaming() {
    // Constructor
}

// Destructor - handles unique_ptr memory automatically
ServicioStreaming::~ServicioStreaming() {
    // The std::unique_ptr in the vector will automatically call destructors
    // for the pointed-to objects when the vector is destroyed.
    // No manual deletion loop is needed here.
    // The previous loop 'for (Video* video : videos)' was incorrect for unique_ptr.
}

void ServicioStreaming::cargarArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo); // Corrected declaration (already assumed fstream was included)
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return;
    }

    std::string line;
    int videos_cargados = 0;
    while (std::getline(archivo, line)) {
        if (line.empty()) continue;
        parseLine(line);
        videos_cargados++;
    }
    std::cout << "Datos cargados exitosamente desde " << nombreArchivo
              << ". Total de videos: " << videos_cargados << std::endl;
    archivo.close();
}

void ServicioStreaming::parseLine(const std::string& line) {
    std::stringstream ss(line);
    std::string tipo, id, nombre, duracion_str, genero, calificaciones_str;

    if (!std::getline(ss, tipo, ',') ||
        !std::getline(ss, id, ',') ||
        !std::getline(ss, nombre, ',') ||
        !std::getline(ss, duracion_str, ',') ||
        !std::getline(ss, genero, ',')) {
        std::cerr << "Advertencia: Línea con formato incorrecto (datos base incompletos): " << line << std::endl;
        return;
    }

    double duracion;
    try {
        duracion = std::stod(duracion_str);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Advertencia: Duración inválida en la línea: " << line << std::endl;
        return;
    } catch (const std::out_of_range& e) {
        std::cerr << "Advertencia: Duración fuera de rango en la línea: " << line << std::endl;
        return;
    }

    // Read remaining part for ratings/episodes
    std::string remaining_part;
    std::getline(ss, remaining_part);

    std::vector<int> calificaciones;
    size_t delimiter_pos = remaining_part.find(';');
    if (delimiter_pos != std::string::npos) {
        calificaciones_str = remaining_part.substr(0, delimiter_pos);
        std::stringstream ss_ratings(calificaciones_str);
        std::string rating_token;
        while (std::getline(ss_ratings, rating_token, '-')) {
            try {
                int rating = std::stoi(rating_token);
                if (rating >= 1 && rating <= 5) {
                    calificaciones.push_back(rating);
                } else {
                    std::cerr << "Advertencia: Calificación inválida para " << nombre << ": '" << rating_token << "'" << std::endl;
                }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Advertencia: Calificación inválida para " << nombre << ": '" << rating_token << "'" << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Advertencia: Calificación fuera de rango para " << nombre << ": '" << rating_token << "'" << std::endl;
            }
        }
        remaining_part = remaining_part.substr(delimiter_pos + 1);
    } else {
        // If no semicolon, assume the whole remaining part is ratings for Pelicula, or nothing for Serie
        calificaciones_str = remaining_part;
        std::stringstream ss_ratings(calificaciones_str);
        std::string rating_token;
        while (std::getline(ss_ratings, rating_token, '-')) {
             if (!rating_token.empty()) { // Only try to parse if not empty
                try {
                    int rating = std::stoi(rating_token);
                    if (rating >= 1 && rating <= 5) {
                        calificaciones.push_back(rating);
                    } else {
                        std::cerr << "Advertencia: Calificación inválida para " << nombre << ": '" << rating_token << "'" << std::endl;
                    }
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Advertencia: Calificación inválida para " << nombre << ": '" << rating_token << "'" << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Advertencia: Calificación fuera de rango para " << nombre << ": '" << rating_token << "'" << std::endl;
                }
            }
        }
        remaining_part = ""; // Mark as processed for ratings
    }

    if (tipo == "Pelicula") {
        Pelicula* nuevaPelicula = new Pelicula(id, nombre, duracion, genero);
        for (int r : calificaciones) {
            nuevaPelicula->calificar(r);
        }
        videos.push_back(std::unique_ptr<Video>(nuevaPelicula)); // Corrected: Use std::unique_ptr
    } else if (tipo == "Serie") {
        Serie* nuevaSerie = new Serie(id, nombre, duracion, genero);
        for (int r : calificaciones) {
            nuevaSerie->calificar(r); // Calificar la serie como un todo
        }

        std::vector<Episodio> episodios_parsed = parseEpisodios(remaining_part);
        for (const Episodio& ep : episodios_parsed) {
            nuevaSerie->AgregarEpisodio(ep); // Corrected: Renamed call
        }
        videos.push_back(std::unique_ptr<Video>(nuevaSerie)); // Corrected: Use std::unique_ptr
    } else {
        std::cerr << "Advertencia: Tipo de video desconocido en la línea: " << line << std::endl;
    }
}


std::vector<int> ServicioStreaming::parseCalificaciones(const std::string& s) {
    std::vector<int> calificaciones;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, '-')) {
        try {
            int cal = std::stoi(token);
            if (cal >= 1 && cal <= 5) { // Ensure ratings are valid
                calificaciones.push_back(cal);
            }
        } catch (const std::invalid_argument& e) {
            // Ignore malformed ratings, or log a warning if desired
        } catch (const std::out_of_range& e) {
            // Ignore out of range ratings, or log a warning
        }
    }
    return calificaciones;
}

std::vector<Episodio> ServicioStreaming::parseEpisodios(const std::string& s) {
    std::vector<Episodio> episodios_list;
    if (s.empty()) return episodios_list;

    std::stringstream ss(s);
    std::string episodio_segment;
    while (std::getline(ss, episodio_segment, '|')) {
        std::stringstream ss_ep(episodio_segment);
        std::string titulo, temporada_str, calificacion_ep_str;

        if (std::getline(ss_ep, titulo, ':') &&
            std::getline(ss_ep, temporada_str, ':') &&
            std::getline(ss_ep, calificacion_ep_str)) {
            try {
                int temporada = std::stoi(temporada_str);
                if (temporada <= 0) { // Basic validation
                    std::cerr << "Advertencia: Temporada inválida para episodio '" << titulo << "': '" << temporada_str << "'" << std::endl;
                    continue;
                }
                Episodio newEp(titulo, temporada);

                // Parse episode ratings
                std::stringstream ss_ep_ratings(calificacion_ep_str);
                std::string rating_token;
                while (std::getline(ss_ep_ratings, rating_token, '-')) {
                    try {
                        int rating = std::stoi(rating_token);
                        newEp.calificar(rating);
                    } catch (const std::invalid_argument& e) {
                        std::cerr << "Advertencia: Calificación inválida para episodio '" << titulo << "': '" << rating_token << "'" << std::endl;
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Advertencia: Calificación fuera de rango para episodio '" << titulo << "': '" << rating_token << "'" << std::endl;
                    }
                }
                episodios_list.push_back(newEp);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Advertencia: Error al parsear episodio: '" << episodio_segment << "'" << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Advertencia: Valor fuera de rango al parsear episodio: '" << episodio_segment << "'" << std::endl;
            }
        } else {
            std::cerr << "Advertencia: Segmento de episodio con formato incorrecto: '" << episodio_segment << "'" << std::endl;
        }
    }
    return episodios_list;
}

void ServicioStreaming::calificarVideo(const std::string& nombreVideoEpisodio, int calificacion) {
    bool found = false;
    std::string lower_nombre_busqueda = toLower(nombreVideoEpisodio);

    // Try to find a direct video match (Pelicula or Serie itself)
    for (const auto& video_ptr : videos) { // Corrected: iterate unique_ptr
        if (toLower(video_ptr->getNombre()) == lower_nombre_busqueda) {
            video_ptr->calificar(calificacion);
            std::cout << "Video '" << video_ptr->getNombre() << "' calificado. Nueva calificación promedio: "
                      << std::fixed << std::setprecision(1) << video_ptr->getCalificacionPromedio() << std::endl;
            found = true;
            break;
        }
    }

    if (!found) {
        // If not found as a video, check if it's an episode of any series
        for (const auto& video_ptr : videos) { // Corrected: iterate unique_ptr
            Serie* serie_ptr = dynamic_cast<Serie*>(video_ptr.get()); // Use .get() to get raw pointer
            if (serie_ptr) { // If it's a Serie
                // Find episode by title (case-insensitive)
                bool episode_found_in_series = false;
                for (Episodio& ep : const_cast<std::vector<Episodio>&>(serie_ptr->GetEpisodios())) { // Corrected: Renamed GetEpisodios & cast to modify
                    if (toLower(ep.getTitulo()) == lower_nombre_busqueda) {
                        ep.calificar(calificacion);
                        std::cout << "Episodio '" << ep.getTitulo() << "' de la serie '" << serie_ptr->getNombre()
                                  << "' calificado. Nueva calificación promedio: "
                                  << std::fixed << std::setprecision(1) << ep.getCalificacionPromedio() << std::endl;
                        found = true;
                        episode_found_in_series = true;
                        break; // Episode found, break inner loop
                    }
                }
                if (episode_found_in_series) {
                    break; // Episode found in a series, break outer loop
                }
            }
        }
    }

    if (!found) {
        std::cout << "Video o episodio '" << nombreVideoEpisodio << "' no encontrado." << std::endl;
    }
}


void ServicioStreaming::mostrarVideosPorCalificacionOGenero(double calificacionMinima, const std::string& generoFiltro) const { // Added const
    bool found_videos = false;
    std::string lower_genero_filtro = toLower(generoFiltro);

    for (const auto& video_ptr : videos) { // Corrected: iterate unique_ptr
        bool matches_calificacion = (calificacionMinima == 0.0 || video_ptr->getCalificacionPromedio() >= calificacionMinima);
        bool matches_genero = (generoFiltro.empty() || toLower(video_ptr->getGenero()) == lower_genero_filtro);

        if (matches_calificacion && matches_genero) {
            video_ptr->mostrarDatos();
            std::cout << "--------------------" << std::endl;
            found_videos = true;
        }
    }

    if (!found_videos) {
        std::cout << "No se encontraron videos con los criterios especificados." << std::endl;
    }
}

void ServicioStreaming::mostrarEpisodiosDeSerieConCalificacion(const std::string& nombreSerie, double calificacionMinima) const { // Added const
    std::string lower_nombre_serie = toLower(nombreSerie);
    bool serie_found = false;

    for (const auto& video_ptr : videos) { // Corrected: iterate unique_ptr
        Serie* serie_ptr = dynamic_cast<Serie*>(video_ptr.get()); // Use .get()
        if (serie_ptr && toLower(serie_ptr->getNombre()) == lower_nombre_serie) {
            serie_ptr->MostrarEpisodiosConCalificacion(calificacionMinima); // Corrected: Renamed call
            serie_found = true;
            break;
        }
    }

    if (!serie_found) {
        std::cout << "Serie '" << nombreSerie << "' no encontrada." << std::endl;
    }
}

void ServicioStreaming::mostrarPeliculasConCalificacion(double calificacionMinima) const { // Added const
    bool found_movies = false;
    std::cout << "Películas con calificación >= " << std::fixed << std::setprecision(1) << calificacionMinima << ":" << std::endl;

    for (const auto& video_ptr : videos) { // Corrected: iterate unique_ptr
        Pelicula* pelicula_ptr = dynamic_cast<Pelicula*>(video_ptr.get()); // Use .get()
        if (pelicula_ptr && pelicula_ptr->getCalificacionPromedio() >= calificacionMinima) {
            pelicula_ptr->mostrarDatos();
            std::cout << "--------------------" << std::endl;
            found_movies = true;
        }
    }

    if (!found_movies) {
        std::cout << "No se encontraron películas con calificación >= "
                  << std::fixed << std::setprecision(1) << calificacionMinima << "." << std::endl;
    }
}

const std::vector<std::unique_ptr<Video>>& ServicioStreaming::GetVideos() const { // Renamed & Added const
    return videos;
}