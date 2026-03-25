#ifndef ANALYZER_H
#define ANALYZER_H
#include "IDataProvider.h"

class Analyzer {
public:
	double calculate_mean(const std::vector<StockRecord>& records) const;
	double moving_average_50(const std::vector<StockRecord>& records) const;
	double moving_average_200(const std::vector<StockRecord>& records) const;
	double RSI(const std::vector<StockRecord>& records) const;
	bool trend_signal(const std::vector<StockRecord>& records) const;
	double calculate_standard_deviation(const std::vector<StockRecord>& records) const;
};
#endif // !ANALYZER_H

