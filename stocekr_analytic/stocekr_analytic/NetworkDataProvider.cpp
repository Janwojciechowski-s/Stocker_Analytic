#include "NetworkDataProvider.h"
#include <stdexcept>
#include <sstream>
std::string NetworkDataProvider::build_url(const std::string& ticker) //server alphavantage
{
    std::string size = "compact";   //size = compact (last 100 records), size=full (all history)
    return "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" + ticker + "&apikey=" + api_key + "&outputsize=" + size + "&datatype=csv";
}

size_t NetworkDataProvider::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t totalSize = size * nmemb;
    ((std::string*)userp)->append((char*)contents, totalSize);
    return totalSize;
}

NetworkDataProvider::NetworkDataProvider(std::string key) : api_key(std::move(key)) {};



DataResult NetworkDataProvider::get_data(const std::string& ticker)
{   
    //uploading file
    CURL* curl;
    CURLcode res;
    std::string read_buffer;

    curl = curl_easy_init();

    if (curl) {
        std::string url = build_url(ticker);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            throw std::runtime_error("Critical curl error: " + std::string(curl_easy_strerror(res)));
        }

        // Parsing section

        DataResult stock_data;
        std::stringstream data_stream(read_buffer);
        std::string temp_line;
        int line_num = 1;

        if (read_buffer.empty()) {
            stock_data.warnings.push_back("Server returned an empty response.");
            return stock_data;
        }
        std::getline(data_stream, temp_line);
        if (data_stream.peek() == std::stringstream::traits_type::eof()) {
            stock_data.warnings.push_back("Server returned an response with only header.");
            return stock_data;
        }

        while (std::getline(data_stream, temp_line))
        {
            ++line_num;
            if (temp_line.empty()) continue;
            std::stringstream ss(temp_line);

            StockRecord temp_stockrecord;
            try {
                std::string temp_help;

                if (!std::getline(ss, temp_stockrecord.date, ',')) throw std::runtime_error("Missing Date");

                if (!std::getline(ss, temp_help, ',')) throw std::runtime_error("Missing Open");
                temp_stockrecord.open = std::stod(temp_help);

                if (!std::getline(ss, temp_help, ',')) throw std::runtime_error("Missing High");
                temp_stockrecord.high = std::stod(temp_help);

                if (!std::getline(ss, temp_help, ',')) throw std::runtime_error("Missing Low");
                temp_stockrecord.low = std::stod(temp_help);

                if (!std::getline(ss, temp_help, ',')) throw std::runtime_error("Missing Close");
                temp_stockrecord.close = std::stod(temp_help);

                if (!std::getline(ss, temp_help, ',')) throw std::runtime_error("Missing Volume");
                temp_stockrecord.volume = std::stoll(temp_help);

                stock_data.records.push_back(temp_stockrecord);

            }
            catch (const std::exception& e) {
                std::string warning = "Line " + std::to_string(line_num) + " error: " + e.what();
                stock_data.warnings.push_back(warning);
            }
        }
        return stock_data;
    }
    else {
        throw std::runtime_error("Critical curl error: Failed to initialize curl handle");
    }
}

