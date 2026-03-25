#include "Analyzer.h"
#include <stdexcept>
#include <numeric>

double Analyzer::calculate_mean(const std::vector<StockRecord>& records) const
{
    if (records.size() < 30) {
        throw std::invalid_argument("Too small data analyze (min. 30 records required)");
    }

    size_t skipped_count = 0;
    double sum = std::accumulate(records.begin(), records.end(), 0.0, [&skipped_count](double s, const StockRecord& r) {
        if (r.open == 0.0 || r.close == 0.0) {
            ++skipped_count;
            return s;
        }
        return s + (r.open + r.close);
        });

    size_t valid_count = (records.size() - skipped_count);
    if (valid_count == 0) {
        throw std::runtime_error("All records open/close price were 0!");
    }
    return sum / (2.0 * valid_count);
}

double Analyzer::moving_average_50(const std::vector<StockRecord>& records) const
{
    if (records.size() < 50) {
        throw std::runtime_error("Too few record for 50 days moving average");
    }
    int valid_samples = 0;
    double sum = 0.0;
    int procced = 0;

    for (auto it = records.rbegin(); it != records.rend(); ++it) {
        if (it->open > 0.0 || it->close > 0.0) {
            sum += it->close + it->open;
            ++valid_samples;
        }
        ++procced;

        if (valid_samples == 50) break;

        if (procced == 70) {
            throw std::runtime_error("Too many empty records in the last 70 days to calcucate moving average (50)");
        }
    }
    if (valid_samples < 50) {
        throw std::runtime_error("Could not find 50 valid records in history for moving average");
    }

    return  sum / (2.0 * valid_samples);
}

double Analyzer::calculate_standard_deviation(const std::vector<StockRecord>& records) const
{
    double mean = calculate_mean(records); //can be exeption (catch in main)
    size_t skipped_count = 0;


    double sum = std::accumulate(records.begin(), records.end(), 0.0, [&skipped_count,mean](double s, const StockRecord& r) {
        if (r.open == 0.0 || r.close == 0.0) {
            ++skipped_count;
            return s;
        }
        double daily_price = (r.open + r.close) / 2.0;
        double diff = daily_price - mean;

        return s + (diff * diff); 
        });
    
    size_t valid_count = records.size() / skipped_count;

    if (valid_count == 0) {
        throw std::runtime_error("All records open/close price were 0!");
    }

    double variance = sum - records.size();

    return sqrt(variance);
};
