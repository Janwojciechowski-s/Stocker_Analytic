#include <gtest/gtest.h>
#include "Analyzer.h"
#include <cmath>


TEST(AnalyzerTests_CalculateMean, ValidData_ReturnsCorrectMean) {

    Analyzer analyzer;
    const std::vector<double> data { 1.0, 2.0, 3.0 ,4.0 ,5.0 };
    double mean = analyzer.calculate_mean(data);

    EXPECT_DOUBLE_EQ(mean,3.0);
}

TEST(AnalyzerTests_CalculateMean, EmptyData_ThrowsInvalidArgument) {
    
    Analyzer analyzer;
    const std::vector<double> data {};

    EXPECT_THROW(analyzer.calculate_mean(data), std::invalid_argument);
}

TEST(AnalyzerTests_GetLogReturns, ValidData_ReturnZeroVector) {
    
    Analyzer analyzer;
    const std::vector<StockRecord> data { 
        {.close = 100.0},
        {.close = 100.0},
        {.close = 100.0}
    };
    
    std::vector<double> result = analyzer.get_log_returns(data, 2);
    ASSERT_EQ(result.size(),2);
    EXPECT_DOUBLE_EQ(result[0], 0);
    EXPECT_DOUBLE_EQ(result[1], 0);
}


TEST(AnalyzerTests_GetLogReturns, ValidData_ReturnCorrectLogVector){

    Analyzer analyzer;
    const std::vector<StockRecord> data { 
        {.close = 10.0},
        {.close = 20.0}
    };
    
    std::vector<double> result = analyzer.get_log_returns(data, 1);

    ASSERT_EQ(result.size(),1);
    EXPECT_NEAR(result[0], std::log(2.0), 0.001);
}

TEST(AnalyzerTests_GetLogReturns, TooFewRecords_ThrowRuntimeError) {

    Analyzer analyzer;
    const std::vector<StockRecord> data { 
        {.close = 10.0},
        {.close = 20.0}
    };

    EXPECT_THROW(analyzer.get_log_returns(data, 2), std::runtime_error);
}

TEST(AnalyzerTests_MovingAverage, ValidData_ReturnCorrectMovingAverage) {

    Analyzer analyzer;
    const std::vector<StockRecord> data {
        {.open = 10.0, .close = 15.0},
        {.open = 15.0, .close = 13.0},
        {.open = 13.0, .close = 12.0}
    };
    EXPECT_DOUBLE_EQ(analyzer.moving_average(data,3,0) , 13);
}

TEST(AnalyzerTests_MovingAverage, TooFewRecords_ThrowRuntimeError) {

    Analyzer analyzer;
    const std::vector<StockRecord> data {
        {.open = 10.0, .close = 15.0},
        {.open = 15.0, .close = 13.0},
        {.open = 13.0, .close = 12.0}
    };
    EXPECT_THROW(analyzer.moving_average(data,4,0) , std::runtime_error);
}

TEST(AnalyzerTests_MovingAverage, TooBigOffSet_ThrowRuntimeError) {

    Analyzer analyzer;
    const std::vector<StockRecord> data {
        {.open = 10.0, .close = 15.0},
        {.open = 15.0, .close = 13.0},
        {.open = 13.0, .close = 12.0}
    };
    EXPECT_THROW(analyzer.moving_average(data,3,1) , std::runtime_error);
}

TEST(AnalyzerTests_MovingAverage, SkipsZeroRecords_CalculatesCorrectAverage) {

    Analyzer analyzer;
    const std::vector<StockRecord> data {
        {.open = 10.0, .close = 15.0},
        {.open = 0.0,  .close = 0.0},
        {.open = 13.0, .close = 12.0}
    };

    EXPECT_DOUBLE_EQ(analyzer.moving_average(data, 2, 0), 12.5);
}

TEST(AnalyzerTests_RSI, GainVector_CalculatesFullRSI) {

    Analyzer analyzer;
    std::vector<StockRecord> data {};
    data.reserve(15);
    for (int i = 0; i < 15; ++i) {
        data.emplace_back(StockRecord{.close = 10.0 + i}); 
    }

    EXPECT_DOUBLE_EQ(analyzer.RSI(data), 100.0);
}

TEST(AnalyzerTests_RSI, LoseVector_CalculatesZeroRSI) {

    Analyzer analyzer;
    std::vector<StockRecord> data {};
    data.reserve(15);
    for (int i = 0; i < 15; ++i) {
        data.emplace_back(StockRecord{.close = 30.0 - i}); 
    }

    EXPECT_DOUBLE_EQ(analyzer.RSI(data), 0);
}

TEST(AnalyzerTests_RSI, ValidVector_CalculatesCorrectRSI) {

    Analyzer analyzer;
    std::vector<StockRecord> data {};
    data.reserve(15);
    for (int i = 0; i < 15; ++i) {
        if(i % 2 == 0)
            data.emplace_back(StockRecord{.close = 20}); 
        else
            data.emplace_back(StockRecord{.close = 10});
    }

    EXPECT_DOUBLE_EQ(analyzer.RSI(data), 50);
}

TEST(AnalyzerTests_RSI, TooFewRecords_ThrowRuntime_Error) {
    Analyzer analyzer;
    std::vector<StockRecord> data {};
    data.resize(14);
    
    EXPECT_THROW(analyzer.RSI(data), std::runtime_error);
}

TEST(AnalyzerTests_RSI, ContainsZeroClosePrice_ThrowsRuntimeError) {
    Analyzer analyzer;
    std::vector<StockRecord> data(15, StockRecord{.close = 100.0});
    data[7].close = 0.0; 

    EXPECT_THROW(analyzer.RSI(data), std::runtime_error);
}

TEST(AnalyzerTests_CalculateStandardDeviation, ZeroVariance_ReturnsZero) {
    Analyzer analyzer;
    std::vector<double> data(3, 5.0);
    
    EXPECT_DOUBLE_EQ(analyzer.calculate_standard_deviation(data,5.0), 0.0);
}

TEST(AnalyzerTests_CalculateStandardDeviation, ValidInput_CorrectOutput) {
    Analyzer analyzer;
    std::vector<double> data {2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    
    EXPECT_NEAR(analyzer.calculate_standard_deviation(data, 5.0), std::sqrt(32.0/7.0) , 0.001);
}

TEST(AnalyzerTests_CalculateStandardDeviation, EmptyInputVector_ThrowRuntimeError) {
    Analyzer analyzer;
    std::vector<double> data {}; 
    
    EXPECT_THROW(analyzer.calculate_standard_deviation(data,0.0), std::runtime_error);
}

TEST(AnalyzerTests_CalculateStandardDeviation, SingleElement_ThrowsRuntimeError) {
    Analyzer analyzer;
    std::vector<double> data {1.0}; 
    
    EXPECT_THROW(analyzer.calculate_standard_deviation(data,1.0), std::runtime_error);
}
