#include "JsonFormatter.h"




std::string JsonResponse::error_response_400(const std::string& message) const
{
	nlohmann::json response;
	response["succes"] = false;
	response["Error"] = "Error: " + message;
	return response.dump(4);
}

std::string JsonResponse::error_response_200(const std::vector<std::string>& warnings, const std::string& message) const
{
	nlohmann::json response;
	
	response["succes"] = false;
	response["message"] = message;
	response["warnings"] = warnings;
	return response.dump(4);
}

std::string JsonResponse::correct_response(const FinalAnalysisResponse& data) const
{
	nlohmann::json response;

	response["success"] = data.success;
	response["warnings"] = data.warnings;

	response["meta_data"] = {
		{"time", data.meta_data.generated_at},
		{"last_price", data.meta_data.last_price},
		{"sample_size" , data.meta_data.sample_size},
	};

	if (data.technical_analysis.has_value()) {
		response["technical_analysis"] = {
			{"volatility", data.technical_analysis->volatility},
			{"moving_average_50", data.technical_analysis->sma50},
			{"moving_average_200", data.technical_analysis->sma200},
			{"RSI",data.technical_analysis->rsi},
			{"trend_signal",data.technical_analysis->trend_signal},
		};
	}
	if (data.simulation_monte_carlo.has_value()) {
		response["simulation_monte_carlo"] = {
			{"median_future_price",data.simulation_monte_carlo->median_future_price},
			{"probability_of_profit",data.simulation_monte_carlo->prob_of_profit},
			{"5_conf_interval_low",data.simulation_monte_carlo->conf_interval_low},
			{"95_conf_interval_high", data.simulation_monte_carlo->conf_interval_high},
		};
	}
	return response.dump(4);
}
