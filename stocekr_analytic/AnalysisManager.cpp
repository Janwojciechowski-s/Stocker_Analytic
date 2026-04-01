#include "AnalysisManager.h"
#include <format>
#include <chrono>


MetaData AnalysisManager::build_metadata(const std::vector<StockRecord>& records) const
{
	MetaData to_return;

	to_return.last_price = (records.empty()) ? 0.0 : records.rbegin()->close;
	to_return.sample_size = static_cast<int>(records.size());
	to_return.generated_at = std::format("{:%Y-%m-%d %H:%M:%S}", std::chrono::system_clock::now());
	return to_return;
}

FinalAnalysisResponse AnalysisManager::process_everything(DataResult& data, const AnalysisSettings& settings) const
{
	FinalAnalysisResponse response;
	
	response.meta_data = build_metadata(data.records);

	if (settings.include_monte_carlo == true) {
	int num_simulations = 10000; 
    int num_days_ahead = 180;
		if(settings.num_days_ahead != 0)
			num_days_ahead = settings.num_days_ahead;
		if (settings.num_simulations > 2)
			num_simulations = settings.num_simulations;
		try {
			SimulationMonteCarlo temp_simulation;
			auto result = simulator_worker.monte_carlo_GBM(data.records, num_days_ahead, num_simulations);
			temp_simulation.median_future_price = result[0];
			temp_simulation.prob_of_profit = result[1];
			temp_simulation.conf_interval_low = result[2];
			temp_simulation.conf_interval_high = result[3];
			response.simulation_monte_carlo = temp_simulation;
		}
		catch (const std::exception& e) {
			response.warnings.push_back(e.what());
		}
	}
	if (settings.include_technical == true) {
		try {
			TechnicalAnalysis temp_technical_analysis;

			std::vector<double> close_prices;
			for (const auto& r : data.records) 
				close_prices.push_back(r.close);

			std::vector<double> log_returns = analyzer_worker.get_log_returns(data.records, 0.9* static_cast<double>(data.records.size()));

			double mean = analyzer_worker.calculate_mean(log_returns);
			temp_technical_analysis.volatility = analyzer_worker.calculate_standard_deviation(log_returns,mean);
			temp_technical_analysis.rsi = analyzer_worker.RSI(data.records);
			temp_technical_analysis.sma50 = analyzer_worker.moving_average(data.records, 50);
			temp_technical_analysis.sma200 = analyzer_worker.moving_average(data.records, 200);
			
			if (settings.include_monte_carlo && response.simulation_monte_carlo.has_value()) {
				std::vector<double> temp_monte_carlo = {
				response.simulation_monte_carlo->median_future_price,
				response.simulation_monte_carlo->prob_of_profit,
				response.simulation_monte_carlo->conf_interval_low,
				response.simulation_monte_carlo->conf_interval_high
				};
				temp_technical_analysis.trend_signal = analyzer_worker.trend_signal(data.records, temp_monte_carlo);
			}
			else
			{
				temp_technical_analysis.trend_signal = analyzer_worker.trend_signal(data.records);
			}
			response.technical_analysis = temp_technical_analysis;
		}
		catch (const std::exception& e) {
			response.warnings.push_back(e.what());
		}
	}

	response.warnings.insert(
		response.warnings.end(),
		std::make_move_iterator(data.warnings.begin()),
		std::make_move_iterator(data.warnings.end())
	);

	return response;
}
