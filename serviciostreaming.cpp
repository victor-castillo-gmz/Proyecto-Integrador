#include "serviciostreaming.h"
#include <iostream>    // For std::cout, std::cerr
#include <limits>      // For std::numeric_limits
#include <algorithm>   // For std::transform
#include <cctype>      // For ::tolower
#include <stdexcept>   // For std::invalid_argument in stoi

// Helper to convert string to lowercase for case-insensitive comparisons
std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return static_cast<unsigned char>(std::tolower(c)); });
    return s;
}

// Destructor implementation
ServicioStreaming::~ServicioStreaming() {
    for (Video* video : videos) {
        delete video; // Free dynamically allocated Video objects
    }
    videos.clear();
}

// Helper to find a video by title (case-insensitive)
Video* ServicioStreaming::buscarVideoPorTitulo(const std::string& titulo) const {
    std::string searchTitleLower = toLower(titulo);
    for (Video* video : videos) {
        if (toLower(video->getNombre()) == searchTitleLower) {
            return video;
        }
    }
    // Only search for top-level videos (Movies or Series), not individual episodes here.
    return nullptr;
}


// Load data from file
void ServicioStreaming::cargarArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return;
    }

    // Clear existing videos to avoid duplicates if loading multiple times
    for (Video* video : videos) {
        delete video;
    }
    videos.clear();

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::stringstream ss(linea);
        std::string tipo, id, nombre, genero, initialRatingsStr;
        double duracion;

        // General Video data parsing (Type, ID, Name, Duration, Genre)
        std::getline(ss, tipo, ',');
        std::getline(ss, id, ',');
        std::getline(ss, nombre, ',');
        
        // Read duration. If it fails, print a warning and skip the line.
        if (!(ss >> duracion)) {
            std::cerr << "Advertencia: Duración inválida en la línea: " << linea << std::endl;
            continue;
        }
        ss.ignore(std::numeric_limits<std::streamsize>::max(), ','); // Consume comma after duration
        
        std::getline(ss, genero, ',');
        
        // Trim leading/trailing whitespace from essential string fields
        tipo.erase(0, tipo.find_first_not_of(" \t\n\r\f\v")); tipo.erase(tipo.find_last_not_of(" \t\n\r\f\v") + 1);
        id.erase(0, id.find_first_not_of(" \t\n\r\f\v")); id.erase(id.find_last_not_of(" \t\n\r\f\v") + 1);
        nombre.erase(0, nombre.find_first_not_of(" \t\n\r\f\v")); nombre.erase(nombre.find_last_not_of(" \t\n\r\f\v") + 1);
        genero.erase(0, genero.find_first_not_of(" \t\n\r\f\v")); genero.erase(genero.find_last_not_of(" \t\n\r\f\v") + 1);

        if (tipo == "Pelicula") {
            // For Pelicula, the next part is initial ratings separated by ';'
            // It's followed by a newline, so no specific delimiter needed for the end of the line.
            std::getline(ss, initialRatingsStr);
            initialRatingsStr.erase(0, initialRatingsStr.find_first_not_of(" \t\n\r\f\v")); initialRatingsStr.erase(initialRatingsStr.find_last_not_of(" \t\n\r\f\v") + 1);

            Pelicula* nuevaPelicula = new Pelicula(id, nombre, duracion, genero);
            std::stringstream ssRatings(initialRatingsStr);
            std::string ratingValStr;
            while (std::getline(ssRatings, ratingValStr, ';')) {
                try {
                    int rating = std::stoi(ratingValStr);
                    nuevaPelicula->calificar(rating);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Advertencia: Calificación inválida para Pelicula '" << nombre << "': '" << ratingValStr << "'" << std::endl;
                }
            }
            videos.push_back(nuevaPelicula);

        } else if (tipo == "Serie") {
            // For Serie, the next part is initial series ratings (e.g., 5-4-5) followed by ';', then episode data
            std::getline(ss, initialRatingsStr, ';');
            initialRatingsStr.erase(0, initialRatingsStr.find_first_not_of(" \t\n\r\f\v")); initialRatingsStr.erase(initialRatingsStr.find_last_not_of(" \t\n\r\f\v") + 1);

            Serie* nuevaSerie = new Serie(id, nombre, duracion, genero);

            // Apply initial series ratings
            std::stringstream ssSeriesRatings(initialRatingsStr);
            std::string seriesRatingValStr;
            while (std::getline(ssSeriesRatings, seriesRatingValStr, '-')) {
                try {
                    int rating = std::stoi(seriesRatingValStr);
                    nuevaSerie->calificar(rating);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Advertencia: Calificación inválida para Serie '" << nombre << "': '" << seriesRatingValStr << "'" << std::endl;
                }
            }

            // Read the rest of the line which contains episode data (separated by '|')
            std::string episodeDataStr;
            if (std::getline(ss, episodeDataStr)) {
                std::stringstream ssEpisodes(episodeDataStr);
                std::string episodeSegment;
                while (std::getline(ssEpisodes, episodeSegment, '|')) {
                    size_t firstColon = episodeSegment.find(':');
                    size_t secondColon = episodeSegment.find(':', firstColon + 1);

                    if (firstColon != std::string::npos && secondColon != std::string::npos) {
                        std::string epTitulo = episodeSegment.substr(0, firstColon);
                        epTitulo.erase(0, epTitulo.find_first_not_of(" \t\n\r\f\v")); epTitulo.erase(epTitulo.find_last_not_of(" \t\n\r\f\v") + 1); // Trim episode title

                        int epTemporada = 0;
                        try {
                            epTemporada = std::stoi(episodeSegment.substr(firstColon + 1, secondColon - (firstColon + 1)));
                        } catch (const std::invalid_argument& e) {
                            std::cerr << "Advertencia: Temporada inválida para episodio '" << epTitulo << "' en '" << nombre << "': '" << episodeSegment.substr(firstColon + 1, secondColon - (firstColon + 1)) << "'" << std::endl;
                            continue; // Skip this episode if season is invalid
                        }

                        Episodio newEp(epTitulo, epTemporada);

                        // Parse episode ratings (e.g., 5-5)
                        std::string epRatingsStr = episodeSegment.substr(secondColon + 1);
                        std::stringstream ssEpRatings(epRatingsStr);
                        std::string epRatingValStr;
                        while (std::getline(ssEpRatings, epRatingValStr, '-')) {
                            try {
                                int rating = std::stoi(epRatingValStr);
                                newEp.calificar(rating);
                            } catch (const std::invalid_argument& e) {
                                std::cerr << "Advertencia: Calificación inválida para episodio '" << epTitulo << "' en '" << nombre << "': '" << epRatingValStr << "'" << std::endl;
                            }
                        }
                        nuevaSerie->agregarEpisodio(newEp);
                    } else {
                        std::cerr << "Advertencia: Formato de segmento de episodio inválido en la serie '" << nombre << "': '" << episodeSegment << "'" << std::endl;
                    }
                }
            }
            videos.push_back(nuevaSerie);
        } else {
            std::cerr << "Advertencia: Tipo de video desconocido en la línea: " << linea << std::endl;
        }
    }
    archivo.close();
    std::cout << "Datos cargados exitosamente desde " << nombreArchivo << ". Total de videos: " << videos.size() << std::endl;
}

void ServicioStreaming::mostrarVideosPorCalificacionOGenero(double calificacionMinima, const std::string& generoFiltro) {
    std::cout << "\n--- Videos filtrados por Calificación (" << std::fixed << std::setprecision(1) << calificacionMinima << "+) o Género ('" << (generoFiltro.empty() ? "Todos" : generoFiltro) << "') ---\n";
    bool found = false;
    std::string generoFiltroLower = toLower(generoFiltro);

    for (const auto& video : videos) {
        bool matchesRating = video->getCalificacionPromedio() >= calificacionMinima;
        bool matchesGenre = generoFiltro.empty() || toLower(video->getGenero()) == generoFiltroLower;

        // For series, also check if any episode meets the rating
        bool hasEpisodeMeetingRating = false;
        Serie* serie = dynamic_cast<Serie*>(video);
        if (serie) {
            for (const auto& ep : serie->getEpisodios()) {
                if (ep.getCalificacionPromedio() >= calificacionMinima) {
                    hasEpisodeMeetingRating = true;
                    break;
                }
            }
        }

        if ((matchesRating || hasEpisodeMeetingRating) && matchesGenre) {
            video->mostrarDatos();
            found = true;
        }
    }

    if (!found) {
        std::cout << "No se encontraron videos con los criterios especificados.\n";
    }
    std::cout << "------------------------------------------------------------------\n";
}

void ServicioStreaming::mostrarEpisodiosDeSerieConCalificacion(const std::string& nombreSerie, double calificacionMinima) {
    std::string searchSerieNameLower = toLower(nombreSerie);
    bool serieFound = false;
    for (const auto& video : videos) {
        Serie* serie = dynamic_cast<Serie*>(video);
        if (serie && toLower(serie->getNombre()) == searchSerieNameLower) {
            serieFound = true;
            serie->mostrarEpisodiosConCalificacion(calificacionMinima);
            break;
        }
    }
    if (!serieFound) {
        std::cout << "Serie '" << nombreSerie << "' no encontrada.\n";
    }
}

void ServicioStreaming::mostrarPeliculasConCalificacion(double calificacionMinima) {
    std::cout << "\n--- Películas con calificación >= " << std::fixed << std::setprecision(1) << calificacionMinima << " ---\n";
    bool found = false;
    for (const auto& video : videos) {
        Pelicula* pelicula = dynamic_cast<Pelicula*>(video);
        if (pelicula && pelicula->getCalificacionPromedio() >= calificacionMinima) {
            pelicula->mostrarDatos();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No se encontraron películas con calificación >= " << std::fixed << std::setprecision(1) << calificacionMinima << ".\n";
    }
    std::cout << "---------------------------------------------------------\n";
}

void ServicioStreaming::calificarVideo(const std::string& tituloVideo, int calificacion) {
    std::string titleToRateLower = toLower(tituloVideo);
    bool found = false;

    // First, try to find a direct Video match (Movie or Series itself)
    for (Video* video : videos) {
        if (toLower(video->getNombre()) == titleToRateLower) {
            video->calificar(calificacion);
            std::cout << "Video '" << video->getNombre() << "' calificado. Nueva calificación promedio: "
                      << std::fixed << std::setprecision(1) << video->getCalificacionPromedio() << std::endl;
            found = true;
            break;
        }
    }

    // If not found as a direct video, check if it's an episode within a series
    if (!found) {
        for (Video* video : videos) {
            Serie* serie = dynamic_cast<Serie*>(video);
            if (serie) {
                std::vector<Episodio>& episodios = serie->getEpisodios(); // Get mutable list of episodes
                bool episodeFoundInSerie = false;
                for (Episodio& ep : episodios) {
                    if (toLower(ep.getTitulo()) == titleToRateLower) {
                        ep.calificar(calificacion);
                        std::cout << "Episodio '" << ep.getTitulo() << "' de la serie '" << serie->getNombre() << "' calificado. Nueva calificación promedio: "
                                  << std::fixed << std::setprecision(1) << ep.getCalificacionPromedio() << std::endl;
                        episodeFoundInSerie = true;
                        found = true; // Mark overall rating as found
                        break;
                    }
                }
                if (episodeFoundInSerie) {
                    break; // Stop searching once episode is rated
                }
            }
        }
    }

    if (!found) {
        std::cout << "Video o episodio '" << tituloVideo << "' no encontrado.\n";
    }
}