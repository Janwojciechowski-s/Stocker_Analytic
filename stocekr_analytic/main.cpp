#include "crow.h"
#include <nlohmann/json.hpp>
#include "IDataProvider.h"
#include "NetworkDataProvider.h"
#include "FileDataProvider.h"
#include "JsonFormatter.h" 
#include <iostream>
#include <memory>

using json = nlohmann::json;


int main()
{
	crow::SimpleApp app;
	JsonResponse response;

	CROW_ROUTE(app, "/analyze").methods(crow::HTTPMethod::POST)
		([&](const crow::request& req) {
		try {
			auto j = json::parse(req.body);
			if (!j.contains("mode")) {
				std::string message = "Error: no \"mode\" provided";
				return crow::response(400, response.error_response_400(message));
			}
			std::unique_ptr<IDataProvider> provider;
			std::string input_for_provider;

			if (j["mode"] == "auto") {
				std::string api_key = j.at("api_key");
				provider = std::make_unique<NetworkDataProvider>(api_key);
				input_for_provider = j.at("ticker");
			}
			else if (j["mode"] == "manual") {
				provider = std::make_unique<FileDataProvider>();
				input_for_provider = j.at("csv_data");
			}
			else {
				std::string message = "Error: Invalid mode. Use 'auto' or 'manual'.";
				return crow::response(400, response.error_response_400(message));
			}

			DataResult data_to_analyze;
			try {
				data_to_analyze = provider->get_data(input_for_provider);
			}
			catch (const std::exception& e) {
				return crow::response(400, std::string(e.what()));
			}
			if (data_to_analyze.records.empty()) {

				std::string message = "No data found for given input";
				data_to_analyze.warnings;	
				return crow::response(200, response.error_response_200(data_to_analyze.warnings, message));
			}
			

		} catch (const std::exception& e) {

			return crow::response(400, "Error: " + std::string(e.what()));
		}
		});
	app.port(18080).multithreaded().run();
}