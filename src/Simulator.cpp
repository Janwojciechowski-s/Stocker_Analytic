#include "Simulator.h"
#include <thread>
#include <future>
#include <random>
#include <algorithm>

std::vector<double> Simulator::worker_monte_carlo_GBM(double start_price, double mean, double standard_deviation, int days_ahead, int num_simulations) const
{
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::normal_distribution<double> dist(0.0, 1.0);

	std::vector<double> results;
	results.reserve(num_simulations);

	const double drift = mean - (0.5 * standard_deviation * standard_deviation);

	for (int i = 0; i < num_simulations; ++i) {
		double current_price = start_price;
		for (int j = 0; j < days_ahead; ++j) {
			double epsilon = dist(gen);
			current_price *= std::exp(drift + (standard_deviation * epsilon));
		}
		results.push_back(current_price);
	}
	return results;
}

std::vector<double> Simulator::monte_carlo_GBM(const std::vector<StockRecord>& records, int days_ahead, int num_simulation) const
{
	if (records.empty()) {
		throw std::runtime_error("No stock records provided (simulator)");
	}
	if (num_simulation <= 0) {
		throw std::invalid_argument("Number of simulations must be greater than 0");
	}
	if (days_ahead <= 0) {
		throw std::invalid_argument("Number of days ahead must be greater than 0");
	}

	int window = (days_ahead > 360 ? 360 : std::max(days_ahead, 90));

	Analyzer analyzer;
	std::vector<double> log_returns = analyzer.get_log_returns(records, window);

	double start_price = records.rbegin()->close;
	double mean = analyzer.calculate_mean(log_returns);
	double standard_deviation = analyzer.calculate_standard_deviation(log_returns, mean);

	unsigned int num_threads = std::thread::hardware_concurrency();
	if (num_threads == 0) num_threads = 2;
	int operations_per_thread = num_simulation / static_cast<int>(num_threads);
	int left = num_simulation % static_cast<int>(num_threads);

	std::vector<std::future<std::vector<double>>> futures;

	for (unsigned int i = 0; i < num_threads; ++i) {
		int operations = operations_per_thread + (i == 0 ? left : 0);
		
		futures.push_back(std::async(std::launch::async,
			&Simulator::worker_monte_carlo_GBM, this,
			start_price, mean, standard_deviation, days_ahead, operations));
	}
	
	std::vector<double> all_final_prices;
	all_final_prices.reserve(num_simulation);

	for (auto& f : futures) {
		std::vector<double> one_thread_prices = f.get();
		all_final_prices.insert(all_final_prices.end(), one_thread_prices.begin(), one_thread_prices.end());
	}
	
	std::sort(all_final_prices.begin(), all_final_prices.end(), [](const double a, const double b) {
		return a < b;
		});
	std::vector<double> to_return;
	
	double median = (num_simulation % 2 != 0) 
		? all_final_prices[num_simulation/2] 
		: (all_final_prices[(num_simulation / 2) - 1] + all_final_prices[num_simulation / 2]) / 2.0;
	to_return.push_back(median);

	int count_win = 0;
	for (double w : all_final_prices) {
		if (w > start_price)
			++count_win;
	}
	double prob_of_win = (static_cast<double>(count_win) / num_simulation) * 100;
	to_return.push_back(prob_of_win);

	int index_5pc = static_cast<int>(num_simulation * 0.05);
	to_return.push_back(all_final_prices[index_5pc]);

	int index_95pc = static_cast<int>(num_simulation * 0.95);
	to_return.push_back(all_final_prices[index_95pc]);

	return to_return;
}
