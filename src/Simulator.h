#ifndef SIMULATOR_H    
#define SIMULATOR_H
#include "Analyzer.h"

class Simulator {
private:
	std::vector<double> worker_monte_carlo_GBM(double start_price, double mean, double standard_deviation,
		int days_ahead, int num_simulations) const;
public:
	std::vector<double> monte_carlo_GBM(const std::vector<StockRecord>& records, int days_ahead = 180, int num_simulation = 10000) const;
};
#endif // !SIMULATOR 

