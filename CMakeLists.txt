cmake_minimum_required(VERSION 3.20)
project(StockerAnalytic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(Threads REQUIRED)


include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG        v1.2.0
)
FetchContent_MakeAvailable(nlohmann_json Crow)


add_library(stocker_core)
target_include_directories(stocker_core PUBLIC src/)

target_sources(stocker_core PRIVATE
    src/AnalysisManager.cpp
    src/Analyzer.cpp
    src/FileDataProvider.cpp
    src/JsonFormatter.cpp
    src/NetworkDataProvider.cpp
    src/Simulator.cpp
)

target_link_libraries(stocker_core PUBLIC
    nlohmann_json::nlohmann_json
    CURL::libcurl
)


add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    stocker_core 
    Crow::Crow
    Threads::Threads
)


