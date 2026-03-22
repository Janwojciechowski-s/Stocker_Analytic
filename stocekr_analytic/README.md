# Stock Analysis Engine

A high-performance Modern C++ microservice for financial data processing.

## Overview
A REST API for stock market data processing. 

**Status:** Active Development (Core Data Sourcing implemented)

## Features
* **REST API**: Multi-threaded request handling using the **Crow** framework.
* **Network Sourcing**: Fetches stock data from the internet **libcurl** (AlphaVantage API). 
* **Manual Sourcing**: Direct CSV data processing from JSON payloads.
* **Robust Parsing**: Line-by-line CSV validation with detailed error reporting (warnings).
* **Provider Pattern**: Decoupled architecture using the `IDataProvider` interface.

## Tech Stack
* **Language**: Modern C++ 
* **Networking**: Crow, libcurl
* **Serialization**: nlohmann/json
* **Environment**: Microsoft Visual Studio, vcpkg
