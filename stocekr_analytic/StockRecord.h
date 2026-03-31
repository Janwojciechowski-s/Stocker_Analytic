#ifndef STOCKRECORD_H    
#define STOCKRECORD_H 
#include<vector>
#include <string>
#include "AnalysisModels.h"
#include <memory>
#include <optional>

struct StockRecord {
    std::string date;
    double open = 0.0;
    double high = 0.0;
    double low = 0.0;
    double close = 0.0;
    long long volume = 0;
};

struct FinalAnalysisResponse {
    MetaData meta_data;

    bool success = false;
    std::vector<std::string> warnings;
    std::optional<TechnicalAnalysis> technical_analysis;
    std::optional<SimulationMonteCarlo> simulation_monte_carlo;
};
#endif // !JSONFORMATER

