#ifndef ANALYSISMODELS_H 
#define ANALYSISMODELS_H 
#include <string>

struct AnalysisSettings {
    bool include_technical = true;
    bool include_monte_carlo = true;
    int num_simulations = 1000;
    int num_days_ahead = 180;
};

struct MetaData {
    int sample_size;
    std::string generated_at;
    double last_price;
};

struct TechnicalAnalysis
{
    double volatility;
    double sma50;
    double sma200;
    double rsi;
    std::string trend_signal;
};

struct SimulationMonteCarlo {
    double median_future_price;
    double prob_of_profit;
    double conf_interval_low;
    double conf_interval_high;
};


#endif // !ANALYSISMODELS_H 
