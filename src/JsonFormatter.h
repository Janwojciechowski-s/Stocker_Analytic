#ifndef JSONFORMATER
#define JSONFORMATER
#include <nlohmann/json.hpp>
#include "StockRecord.h"


class JsonResponse {
public:
	std::string error_response_400(const std::string& message) const;
	std::string error_response_200(const std::vector<std::string>& warnings, const std::string& message) const;
	std::string correct_response(const FinalAnalysisResponse& data) const;
};
#endif // !SIMULATOR 

