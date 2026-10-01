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