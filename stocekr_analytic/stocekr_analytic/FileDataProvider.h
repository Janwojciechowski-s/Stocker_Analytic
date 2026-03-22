#ifndef FILEDATAPROVIDER_H    
#define FILEDATAPROVIDER_H  
#include<vector>
#include <string>
#include "IDataProvider.h"

class FileDataProvider : public IDataProvider {
public:
	virtual ~FileDataProvider() = default;
	virtual DataResult get_data(const std::string& ticker) override;

};
#endif // !FILEDATAPROVIDER_H  
