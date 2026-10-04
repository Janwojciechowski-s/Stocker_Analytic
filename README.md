# Stocker Analytic

High-performance C++ REST API service to analyze stock market data and run price simulations.

**Status:** Working / Tested

## Features
- **Data from API (AlphaVantage):** Fetches real-time stock quotes via HTTPS (`libcurl`).
- **Data from CSV:** Parses CSV data sent directly in JSON requests.
- **Technical Analysis:** Moving averages (SMA), RSI, market trends, and volatility.
- **Monte Carlo Simulation:** Multithreaded price prediction using the Geometric Brownian Motion (GBM) model.
- **REST API:** Lightweight HTTP service built with Crow, returning responses in JSON.
- **Automated Testing:** 29 unit tests covering providers, analysis logic, and edge cases.
- **Containerized:** Reproducible multi-stage Docker build verifying tests before production image creation.

## Architecture
* **AnalysisManager**: Coordinates data flow between data providers and calculation modules.
* **IDataProvider**: Abstract interface for fetching stock records.
* **NetworkDataProvider**: Fetches and parses data from the AlphaVantage API.
* **FileDataProvider**: Robust parser for CSV data sent inside JSON requests.
* **Analyzer**: Computes statistical indicators (averages, RSI, trends, volatility).
* **Simulator**: Multithreaded Monte Carlo simulation engine.
* **JsonFormatter**: Prepares final JSON responses.
* **StockRecord**: Core data structure representing an individual market data point.

## Technologies & Tools
* **Language:** C++20
* **Build System:** CMake (with `FetchContent` for external dependencies)
* **Testing:** Google Test (GTest)
* **Libraries:** Crow (HTTP Framework), libcurl, nlohmann/json, Asio
* **DevOps:** Docker (Multi-stage build), Docker Compose
* **Platform:** Linux (Ubuntu / WSL2)

## How to Run

### Option 1: Docker Compose (Recommended)  

Builds the multi-stage Docker image, runs all 29 unit tests, and exposes the application on port `18080`.

```bash
docker compose up --build
```

### Option 2: Local Build (Linux / WSL2)

Install the required dependencies:
```bash
sudo apt update && sudo apt install -y build-essential cmake git libcurl4-openssl-dev libasio-dev
```
Configure and build the project:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```
Run the unit tests:
```bash
./build/tests/unit_test
```
Start the application:
```bash
./build/StockerAnalytic
```
