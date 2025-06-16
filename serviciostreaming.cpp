#include "serviciostreaming.h"
#include "pelicula.h"
#include "serie.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <iomanip> // For std::fixed and std::setprecision
#include <cctype>  // For std::tolower

// Helper function
std::string ServicioStreaming::toLower(const std::string& str) {
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lower_str;
}

ServicioStreaming::ServicioStreaming() {
    // Constructor
}

ServicioStreaming::~ServicioStreaming() {
    // The std::unique_ptr in the vector will automatically call destructors
    // for the pointed-to objects when the vector is destroyed.
    // No manual deletion loop is needed here.
}

void ServicioStreaming::cargarArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return;
    }

    std::string line;
    int videos_cargados = 0;
    while (std::getline(archivo, line)) {
        if (line.empty()) continue; // Skip empty lines
        parseLine(line);
        videos_cargados++;
    }
    std::cout << "Datos cargados exitosamente desde " << nombreArchivo
              << ". Total de videos: " << videos_cargados << std::endl;
    archivo.close();
}

void ServicioStreaming::parseLine(const std::string& line) {
    std::stringstream ss(line);
    std::string tipo, id, nombre, duracion_str, genero;

    // Read initial 5 fields
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

    // Read the rest of the line, which can contain video ratings and/or episode data
    std::string remaining_part_full;
    std::getline(ss, remaining_part_full);

    std::string video_ratings_str;
    std::string episode_data_str;

    size_t semicolon_pos = remaining_part_full.find(';');

    if (semicolon_pos != std::string::npos) {
        // If semicolon exists, part before is video ratings, part after is episode data
        video_ratings_str = remaining_part_full.substr(0, semicolon_pos);
        episode_data_str = remaining_part_full.substr(semicolon_pos + 1);
    } else {
        // If no semicolon, assume it's all video ratings (e.g., for a Pelicula)
        video_ratings_str = remaining_part_full;
        episode_data_str = ""; // No episode data
    }

    std::vector<int> calificaciones;
    std::stringstream ss_video_ratings(video_ratings_str);
    std::string rating_token;
    while (std::getline(ss_video_ratings, rating_token, '-')) {
        if (!rating_token.empty()) {
            try {
                int rating = std::stoi(rating_token);
                if (rating >= 1 && rating <= 5) {
                    calificaciones.push_back(rating);
                } else {
                    // Changed message to match test expectation for out-of-range valid numbers (e.g., "0" or "6")
                    std::cerr << "Advertencia: Calificación inválida para " << tipo << " '" << nombre << "': '" << rating << "'" << std::endl;
                }
            } catch (const std::invalid_argument& e) {
                // Changed message to match test expectation for non-numeric ratings (e.g., "invalid")
                std::cerr << "Advertencia: Calificación inválida para " << tipo << " '" << nombre << "': '" << rating_token << "'" << std::endl;
            } catch (const std::out_of_range& e) {
                // For extremely large/small numbers that fit in string but not int
                std::cerr << "Advertencia: Calificación fuera de rango para " << tipo << " '" << nombre << "': '" << rating_token << "'" << std::endl;
            }
        }
    }

    if (tipo == "Pelicula") {
        Pelicula* nuevaPelicula = new Pelicula(id, nombre, duracion, genero);
        for (int r : calificaciones) {
            nuevaPelicula->calificar(r);
        }
        videos.push_back(std::unique_ptr<Video>(nuevaPelicula));
    } else if (tipo == "Serie") {
        Serie* nuevaSerie = new Serie(id, nombre, duracion, genero);
        for (int r : calificaciones) {
            nuevaSerie->calificar(r); // Calificar la serie como un todo
        }

        std::vector<Episodio> episodios_parsed = parseEpisodios(episode_data_str, nombre); // Pass series name
        for (const Episodio& ep : episodios_parsed) {
            nuevaSerie->AgregarEpisodio(ep);
        }
        videos.push_back(std::unique_ptr<Video>(nuevaSerie));
    } else {
        std::cerr << "Advertencia: Tipo de video desconocido en la línea: " << line << std::endl;
    }
}


std::vector<int> ServicioStreaming::parseCalificaciones(const std::string& s) {
    std::vector<int> calificaciones;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, '-')) {
        if (token.empty()) continue; // Skip empty tokens (e.g., trailing hyphen)
        try {
            int cal = std::stoi(token);
            if (cal >= 1 && cal <= 5) { // Ensure ratings are valid
                calificaciones.push_back(cal);
            }
            // No warning here, as parseLine handles direct parsing of main video ratings
        } catch (const std::invalid_argument& e) {
            // Ignore malformed ratings in this helper, or log a warning if desired
        } catch (const std::out_of_range& e) {
            // Ignore out of range ratings in this helper, or log a warning
        }
    }
    return calificaciones;
}

// Modified parseEpisodios to accept series_name
std::vector<Episodio> ServicioStreaming::parseEpisodios(const std::string& s, const std::string& series_name) {
    std::vector<Episodio> episodios_list;
    if (s.empty()) return episodios_list;

    std::stringstream ss(s);
    std::string episodio_segment;
    while (std::getline(ss, episodio_segment, '|')) {
        if (episodio_segment.empty()) continue; // Skip empty segments

        std::stringstream ss_ep(episodio_segment);
        std::string titulo, temporada_str, calificacion_ep_str;

        if (std::getline(ss_ep, titulo, ':') &&
            std::getline(ss_ep, temporada_str, ':') &&
            std::getline(ss_ep, calificacion_ep_str)) {
            try {
                int temporada = std::stoi(temporada_str);
                if (temporada <= 0) { // Basic validation for valid numbers
                    std::cerr << "Advertencia: Temporada inválida para episodio '" << titulo << "' en '" << series_name << "': '" << temporada_str << "'" << std::endl;
                    continue; // Skip this episode
                }
                Episodio newEp(titulo, temporada);

                std::stringstream ss_ep_ratings(calificacion_ep_str);
                std::string rating_token;
                while (std::getline(ss_ep_ratings, rating_token, '-')) {
                    if (rating_token.empty()) continue;
                    try {
                        int rating = std::stoi(rating_token);
                        if (rating >= 1 && rating <= 5) {
                            newEp.calificar(rating);
                        } else {
                            std::cerr << "Advertencia: Calificación inválida para episodio '" << titulo << "' en '" << series_name << "': '" << rating << "'" << std::endl;
                        }
                    } catch (const std::invalid_argument& e) {
                        std::cerr << "Advertencia: Calificación inválida para episodio '" << titulo << "' en '" << series_name << "': '" << rating_token << "'" << std::endl;
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Advertencia: Calificación fuera de rango para episodio '" << titulo << "' en '" << series_name << "': '" << rating_token << "'" << std::endl;
                    }
                }
                episodios_list.push_back(newEp);
            } catch (const std::invalid_argument& e) {
                // This catch block is for when std::stoi(temporada_str) fails (e.g., "invalid_season")
                std::cerr << "Advertencia: Temporada inválida para episodio '" << titulo << "' en '"
                          << series_name << "': '" << temporada_str << "'" << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Advertencia: Valor de temporada fuera de rango para episodio '" << titulo << "' en '" << series_name << "': '" << temporada_str << "'" << std::endl;
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
    for (const auto& video_ptr : videos) {
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
        for (const auto& video_ptr : videos) {
            Serie* serie_ptr = dynamic_cast<Serie*>(video_ptr.get());
            if (serie_ptr) { // If it's a Serie
                bool episode_found_in_series = false;
                // Use const_cast to get a mutable reference to episodes for modification
                for (Episodio& ep : const_cast<std::vector<Episodio>&>(serie_ptr->GetEpisodios())) {
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


void ServicioStreaming::mostrarVideosPorCalificacionOGenero(double calificacionMinima, const std::string& generoFiltro) const {
    bool found_videos = false;
    std::string lower_genero_filtro = toLower(generoFiltro);

    for (const auto& video_ptr : videos) {
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

void ServicioStreaming::mostrarEpisodiosDeSerieConCalificacion(const std::string& nombreSerie, double calificacionMinima) const {
    std::string lower_nombre_serie = toLower(nombreSerie);
    bool serie_found = false;

    for (const auto& video_ptr : videos) {
        Serie* serie_ptr = dynamic_cast<Serie*>(video_ptr.get());
        if (serie_ptr && toLower(serie_ptr->getNombre()) == lower_nombre_serie) {
            serie_ptr->MostrarEpisodiosConCalificacion(calificacionMinima);
            serie_found = true;
            break;
        }
    }

    if (!serie_found) {
        std::cout << "Serie '" << nombreSerie << "' no encontrada." << std::endl;
    }
}

void ServicioStreaming::mostrarPeliculasConCalificacion(double calificacionMinima) const {
    bool found_movies = false;
    std::cout << "Películas con calificación >= " << std::fixed << std::setprecision(1) << calificacionMinima << ":" << std::endl;

    for (const auto& video_ptr : videos) {
        Pelicula* pelicula_ptr = dynamic_cast<Pelicula*>(video_ptr.get());
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

const std::vector<std::unique_ptr<Video>>& ServicioStreaming::GetVideos() const {
    return videos;
}