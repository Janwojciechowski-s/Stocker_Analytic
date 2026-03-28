#ifndef ANALYZER_H
#define ANALYZER_H
#include "IDataProvider.h"

class Analyzer {
public:
	double calculate_mean(const std::vector<double>& data) const;
	std::vector<double> get_log_returns(const std::vector<StockRecord>& records, int days) const;
	double moving_average(const std::vector<StockRecord>& records, int time) const;
	double RSI(const std::vector<StockRecord>& records) const;
	double calculate_standard_deviation(const std::vector<double>& data, double mean) const;
	bool trend_signal(const std::vector<StockRecord>& records) const;
};
#endif // !ANALYZER_H

