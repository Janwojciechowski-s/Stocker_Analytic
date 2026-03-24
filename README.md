# Stocker Analytic

Program to analyze stock market data and simulate prices.

**Status:** Working / Active Development

## Progress
- **Done:** Data from API (AlphaVantage)
- **Done:** Data from CSV (JSON request)
- **To do:** Moving averages and RSI
- **To do:** Monte Carlo simulation
- **To do:** Sending results in JSON format

## Architecture
* **IDataProvider**: Main interface for data.
* **NetworkDataProvider**: Gets data from API (server alphavantage) and parses.
* **FileDataProvider**: Parses CSV data sent inside JSON requests.
* **Analyzer**: Data analysis(Averages, RSI, Trends, Volatility).
* **Simulator**: Monte Carlo simulation (Price prediction).
* **JsonFormatter**: Prepares the final JSON response.
* **StockRecord**: Basic structure for stock data.

## Technologies
* **Language**: modern C++
* **Libraries**: Crow, libcurl, nlohmann/json
* **Tools**: Visual Studio 2022, vcpkg

## How to run
1. Open `stocekr_analytic.sln` in Visual Studio.
2. Open terminal in Visual Studio and type: `vcpkg install`.
3. Press **F5** to start the server.

## Usage API Testing
Once the server is running (port 18080), you can send POST requests to `/analyze`. 
Here are examples using **PowerShell** (recommended for Windows):

Send stock data directly as a CSV string inside a JSON object:

1) Valid JSON request with CSV data parameter:
   
Invoke-RestMethod -Uri http://localhost:18080/analyze -Method Post -Body '{
    "mode": "manual",
    "csv_data": "timestamp,open,high,low,close,volume\n2024-03-20,150.0,155.0,149.0,153.5,100000\n2024-03-21,153.5,158.0,152.0,157.2,120000"
}' -ContentType "application/json" | ConvertTo-Json

2) Fetch real-time market data directly from Alpha Vantage. (Replace YOUR_API_KEY with your actual key)

Invoke-RestMethod -Uri http://localhost:18080/analyze -Method Post -Body '{
    "mode": "auto",
    "ticker": "AAPL",
    "api_key": "YOUR_API_KEY"
}' -ContentType "application/json" | ConvertTo-Json

3) Missing Parameters (Currently, the server returns a detailed error response):

Invoke-RestMethod -Uri http://localhost:18080/analyze -Method Post -Body '{
    "mode": "manual"
}' -ContentType "application/json" | ConvertTo-Json

4) Robustness: Fault-Tolerant Parsing:

Invoke-RestMethod -Uri http://localhost:18080/analyze -Method Post -Body '{
    "mode": "manual",
    "csv_data": "timestamp,open,high,low,close,volume\n2024-03-20,150.0,155.0,149.0,153.5,100000\nWRONG_LINE\n2024-03-21,153.5,158.0,152.0,157.2,120000"
}' -ContentType "application/json" | ConvertTo-Json
