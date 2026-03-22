#ifndef IDATAPROVIDER_H    
#define IDATAPROVIDER_H
#include<vector>
#include <string>
#include "StockRecord.h"

struct DataResult {
	std::vector<StockRecord> records;
	std::vector<std::string> warnings;
};

class IDataProvider {
public:
	virtual ~IDataProvider() {};
	virtual DataResult get_data(const std::string& ticker) = 0;
}; 
#endif // !IDATAPROVIDER_H 

