#ifndef EXPERIMENTO_H
#define EXPERIMENTO_H

#include <string>
#include <vector>

struct Experimento {
    int id;
    double barleyMalt;  // g/L
    double DAP;         // g/L
    double time;        // d
};

struct Preparacion {
    int idExperimento;
    std::string variante;     // "Sólido" o "Líquido"
    int replica;              // 1, 2, 3
    double barleyMaltFinal;
    double DAPFinal;
    double time;
};

// Lee experimentos desde CSV (retorna true si OK, false si error)
bool cargarExperimentosDeCSV(const std::string &filename,
                             std::vector<Experimento> &experimentos);

// Genera las preparaciones (2 variantes × 3 repeticiones)
std::vector<Preparacion> generarPreparaciones(const std::vector<Experimento> &experimentos);

#endif // EXPERIMENTO_H
