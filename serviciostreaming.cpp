/**
 * @file serviciostreaming.cpp
 * @brief Implementación de la clase ServicioStreaming.
 * @author Tu Nombre
 * @date 2025-06-15
 */

#include "serviciostreaming.h"
#include "pelicula.h"
#include "serie.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <vector>

// --- Métodos de Ayuda (Implementación) ---

std::string ServicioStreaming::ToLower(const std::string& str) const {
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lower_str;
}

void ServicioStreaming::ParseRatings(Video& video, const std::string& ratingsStr) {
    if (ratingsStr.empty()) return;

    std::stringstream ss(ratingsStr);
    std::string rating;
    while (std::getline(ss, rating, '-')) {
        try {
            if (!rating.empty()) { // Ignorar segmentos vacíos (ej. 5--4)
                int calificacion = std::stoi(rating);
                video.Calificar(calificacion);
            }
        } catch (const std::invalid_argument&) {
            std::cerr << "Advertencia: Calificacion invalida para "
                      << (dynamic_cast<Pelicula*>(&video) ? "Pelicula" : "Serie")
                      << " '" << video.GetNombre() << "': '" << rating << "'" << std::endl;
        }
    }
}

void ServicioStreaming::ParseEpisodios(Serie& serie, const std::string& episodesStr) {
    if (episodesStr.empty()) return;

    std::stringstream ss(episodesStr);
    std::string episodeData;
    while (std::getline(ss, episodeData, '|')) {
        std::stringstream ep_ss(episodeData);
        std::string titulo, temporada_str, ratings_str;

        // *** INICIO DE LA CORRECCIÓN ***
        // Se relaja la condición: solo se requiere título y temporada.
        // Las calificaciones son opcionales.
        if (std::getline(ep_ss, titulo, ':') && std::getline(ep_ss, temporada_str, ':'))
        {
            // Se lee el resto de la línea para las calificaciones, sin verificar el éxito.
            std::getline(ep_ss, ratings_str);
            
            try {
                int temporada = std::stoi(temporada_str);
                Episodio ep(titulo, temporada);
                
                std::stringstream rating_ss(ratings_str);
                std::string rating_val;
                while(std::getline(rating_ss, rating_val, '-')) {
                    try {
                        if (!rating_val.empty()) {
                           ep.Calificar(std::stoi(rating_val));
                        }
                    } catch (const std::invalid_argument&) {
                        // Ignora la calificación inválida y continúa
                    }
                }
                serie.AgregarEpisodio(ep);
            } catch (const std::invalid_argument&) {
                std::cerr << "Advertencia: Temporada invalida para episodio '" << titulo
                          << "' en '" << serie.GetNombre() << "': '" << temporada_str << "'" << std::endl;
            }
        }
        // *** FIN DE LA CORRECCIÓN ***
    }
}


void ServicioStreaming::ParsePeliculaLine(const std::string& line) {
    std::stringstream ss(line);
    std::string id, nombre, genero, ratingsStr;
    std::string duracionStr;
    double duracion = 0.0;

    std::getline(ss, id, ',');
    std::getline(ss, nombre, ',');
    std::getline(ss, duracionStr, ',');
    std::getline(ss, genero, ',');
    std::getline(ss, ratingsStr);

    try {
        duracion = std::stod(duracionStr);
    } catch (const std::invalid_argument&) {
        std::cerr << "Advertencia: Duracion invalida en la linea: Pelicula," << line << std::endl;
        return;
    }

    auto pelicula = std::make_unique<Pelicula>(id, nombre, duracion, genero);
    ParseRatings(*pelicula, ratingsStr);
    videos.push_back(std::move(pelicula));
}

void ServicioStreaming::ParseSerieLine(const std::string& line) {
    std::stringstream ss(line);
    std::string id, nombre, genero, seriesData, episodesStr;
    std::string duracionStr, ratingsStr;
    double duracion = 0.0;

    std::getline(ss, id, ',');
    std::getline(ss, nombre, ',');
    std::getline(ss, duracionStr, ',');
    std::getline(ss, genero, ',');
    
    std::string restOfLine;
    std::getline(ss, restOfLine);
    
    size_t semicolonPos = restOfLine.find(';');
    if (semicolonPos != std::string::npos) {
        ratingsStr = restOfLine.substr(0, semicolonPos);
        episodesStr = restOfLine.substr(semicolonPos + 1);
    } else {
        ratingsStr = restOfLine;
    }
    
    try {
        duracion = std::stod(duracionStr);
    } catch (const std::invalid_argument&) {
        duracion = 0.0;
    }

    auto serie = std::make_unique<Serie>(id, nombre, duracion, genero);
    ParseRatings(*serie, ratingsStr);
    ParseEpisodios(*serie, episodesStr);
    videos.push_back(std::move(serie));
}

void ServicioStreaming::IndexarContenido() {
    videosPorTituloLower.clear();
    episodiosPorTituloLower.clear();

    for (const auto& video : videos) {
        videosPorTituloLower[ToLower(video->GetNombre())] = video.get();
        if (Serie* serie = dynamic_cast<Serie*>(video.get())) {
            for (auto& episodio : serie->GetEpisodiosMutables()) {
                episodiosPorTituloLower[ToLower(episodio.GetTitulo())] = &episodio;
            }
        }
    }
}

// --- Métodos Públicos (Implementación) ---

void ServicioStreaming::CargarArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return;
    }

    videos.clear();
    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty() || linea == "\r") { // Ignorar líneas vacías
            continue;
        }

        std::stringstream ss(linea);
        std::string tipo;
        std::getline(ss, tipo, ',');

        std::string restoDeLinea;
        std::getline(ss, restoDeLinea);

        if (tipo == "Pelicula") {
            ParsePeliculaLine(restoDeLinea);
        } else if (tipo == "Serie") {
            ParseSerieLine(restoDeLinea);
        } else {
             std::cerr << "Advertencia: Tipo de video desconocido '" << tipo << "'" << std::endl;
        }
    }
    IndexarContenido();
    std::cout << "Datos cargados exitosamente. Total de videos: " << videos.size() << std::endl;
}

void ServicioStreaming::CalificarVideo(const std::string& titulo, int calificacion) {
    std::string tituloLower = ToLower(titulo);

    auto it_ep = episodiosPorTituloLower.find(tituloLower);
    if (it_ep != episodiosPorTituloLower.end()) {
        it_ep->second->Calificar(calificacion);
        std::cout << "Episodio '" << it_ep->second->GetTitulo() << "' calificado. Nueva calificacion promedio: " << std::fixed << std::setprecision(1) << it_ep->second->GetCalificacionPromedio() << std::endl;
        return;
    }

    auto it_vid = videosPorTituloLower.find(tituloLower);
    if (it_vid != videosPorTituloLower.end()) {
        it_vid->second->Calificar(calificacion);
        std::cout << "Video '" << it_vid->second->GetNombre() << "' calificado. Nueva calificacion promedio: " << std::fixed << std::setprecision(1) << it_vid->second->GetCalificacionPromedio() << std::endl;
        return;
    }

    std::cout << "Video o episodio '" << titulo << "' no encontrado." << std::endl;
}


void ServicioStreaming::MostrarVideosPorCalificacionOGenero(double calificacionMinima, const std::string& genero) {
    bool found = false;
    std::string generoLower = ToLower(genero);

    for (const auto& video : videos) {
        bool calificacionOk = video->GetCalificacionPromedio() >= calificacionMinima;
        bool generoOk = genero.empty() || ToLower(video->GetGenero()) == generoLower;

        if (calificacionOk && generoOk) {
            video->MostrarDatos();
            std::cout << "--------------------" << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "No se encontraron videos con los criterios especificados." << std::endl;
    }
}

void ServicioStreaming::MostrarEpisodiosDeSerieConCalificacion(const std::string& tituloSerie, double calificacionMinima) {
    auto it = videosPorTituloLower.find(ToLower(tituloSerie));
    if (it != videosPorTituloLower.end()) {
        if (Serie* serie = dynamic_cast<Serie*>(it->second)) {
            std::cout << "Episodios de la serie '" << serie->GetNombre() << "' con calificacion >= " << calificacionMinima << ":" << std::endl;
            serie->MostrarEpisodiosConCalificacion(calificacionMinima);
            return;
        }
    }
    std::cout << "Serie '" << tituloSerie << "' no encontrada." << std::endl;
}

void ServicioStreaming::MostrarPeliculasConCalificacion(double calificacionMinima) {
    bool found = false;
    for (const auto& video : videos) {
        if (dynamic_cast<Pelicula*>(video.get()) && video->GetCalificacionPromedio() >= calificacionMinima) {
            video->MostrarDatos();
            std::cout << "--------------------" << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "No se encontraron peliculas con calificacion >= " << std::fixed << std::setprecision(1) << calificacionMinima << "." << std::endl;
    }
}