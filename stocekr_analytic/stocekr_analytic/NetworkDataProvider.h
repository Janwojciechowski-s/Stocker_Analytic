#ifndef NETWORKDATAPROVIDER_H    
#define NETWORKDATAPROVIDER_H  
#include<vector>
#include <string>
#include "IDataProvider.h"
#include <curl/curl.h>

class NetworkDataProvider : public IDataProvider {
private:
	std::string build_url(const std::string& ticker);  //server alphavantage
	std::string api_key;
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
public:
	NetworkDataProvider(std::string key);
	virtual ~NetworkDataProvider() = default;
	virtual DataResult get_data(const std::string& ticker) override;
};
#endif // !NETWORKDATAPROVIDER_H  