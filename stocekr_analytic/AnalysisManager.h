#ifndef ANALYSISMANAGER  
#define ANALYSISMANAGER  
#include "StockRecord.h"
#include "IDataProvider.h"
#include "Simulator.h"
#include "Analyzer.h"
#include "AnalysisModels.h"


class AnalysisManager
{
private:
	Analyzer analyzer_worker;
	Simulator simulator_worker;
	MetaData build_metadata(const std::vector<StockRecord>& records) const;
public:
	FinalAnalysisResponse process_everything(DataResult& data, const AnalysisSettings& settings) const;
};


#endif // !ANALYSISMANAGER  