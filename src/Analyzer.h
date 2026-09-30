#ifndef ANALYZER_H
#define ANALYZER_H
#include "IDataProvider.h"
#include "Simulator.h"

class Analyzer {
public:
	double calculate_mean(const std::vector<double>& data) const;
	std::vector<double> get_log_returns(const std::vector<StockRecord>& records, int days) const;
	double moving_average(const std::vector<StockRecord>& records, int time, int offset = 0) const;
	double RSI(const std::vector<StockRecord>& records) const;
	double calculate_standard_deviation(const std::vector<double>& data, double mean) const;
	std::string trend_signal(const std::vector<StockRecord>& records, const std::vector<double>& monte_carlo_result = {}) const;
};
#endif // !ANALYZER_H

