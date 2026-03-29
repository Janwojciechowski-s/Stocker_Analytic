#include "Analyzer.h"
#include <stdexcept>
#include <numeric>
#include <cmath>

double Analyzer::calculate_mean(const std::vector<double>& data) const {
    if (data.empty()) {
        throw std::invalid_argument("Mean calculation failed: data vector is empty");
    }
    return std::accumulate(data.begin(), data.end(), 0.0) / static_cast<double>(data.size());
}

std::vector<double> Analyzer::get_log_returns(const std::vector<StockRecord>& records, int days) const
{
    std::vector<double> log_returns;
    if (records.size() < days) {
        throw std::runtime_error("Too few records, require at least " + std::to_string(days));
    }
    int invalid_samples = 0;
    int valid_samples = 0;
    for (auto it = records.rbegin(); it + 1 != records.rend(); ++it) {
        if (valid_samples == days) {
            break;
        }
        if (invalid_samples >= days + 10) {
            throw std::runtime_error("Too many empty records (get_log_return)");
        }

        if (it->close == 0 || (it+1)->close == 0) {
            ++invalid_samples;
            continue;
        }
        double temp = std::log(it->close / (it+1)->close);
        log_returns.push_back(temp);
        ++valid_samples;
    }
    if (valid_samples < days) {
        throw std::runtime_error("Failed to collect enough log returns due to gaps in price history");
    }
    return log_returns;
}

double Analyzer::moving_average(const std::vector<StockRecord>& records, int time) const
{
    if (records.size() < time) {
        throw std::runtime_error("Too few records for " + std::to_string(time) + " days moving average");
    }

    int valid_samples = 0;
    double sum = 0.0;
    int processed = 0;

    for (auto it = records.rbegin(); it != records.rend(); ++it) {
        if (it->open > 0.0 && it->close > 0.0) {
            sum += it->close + it->open;
            ++valid_samples;
        }
        ++processed;

        if (valid_samples == time) break;

        if (processed > valid_samples + 15) {
            throw std::runtime_error("Too many empty records in the last " + std::to_string(time+30) +
                " days to calculate moving average");
        }
    }
    if (valid_samples < time) {
        throw std::runtime_error("Could not find " + std::to_string(time) + " valid records in history for moving average");
    }
    return sum / (2.0 * valid_samples);
}

double Analyzer::RSI(const std::vector<StockRecord>& records) const
{
    if (records.size() < 15) {
        throw std::runtime_error("Too few records to calculate RSI");
    }
    double loss = 0.0;
    double gain = 0.0;
    for (int i = 0; i < 14; ++i) {
        auto current = records.rbegin() + i;
        auto previous = records.rbegin() + i + 1;
        if (current->close == 0.0 || previous->close == 0.0) {
            throw std::runtime_error("Incomplete data: RSI requires 15 valid records");
        }
        double diff = current->close - previous->close;
        if (diff < 0) {
            loss += abs(diff);
        }
        else {
            gain += abs(diff);
        }
    }
    if (loss == 0) return 100;

    double RS = gain / loss;
    return 100 - (100 / (1 + RS));
}

double Analyzer::calculate_standard_deviation(const std::vector<double>& data, double mean) const
{
    if (data.size() < 2) {
        throw std::runtime_error("Standard deviation require at least 2 samples");
    }
    double sum_sq = std::accumulate(data.begin(), data.end(), 0.0, [mean](double s, const double d) {
        double diff = mean - d;
        return s + (diff * diff);
        });
    double variance = sum_sq / (data.size() - 1);
    return sqrt(variance);
};
