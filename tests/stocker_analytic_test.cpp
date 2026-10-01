#include <gtest/gtest.h>
#include "Analyzer.h"

TEST(AnalyzerTests_CalculateMean, ValidData_ReturnsCorrectMean) {

    Analyzer analyzer;
    const std::vector<double> data { 1.0 , 2.0 , 3.0 , 4.0 , 5.0 };
    double mean = analyzer.calculate_mean(data);

    EXPECT_DOUBLE_EQ(mean,3.0);
}

TEST(AnalyzerTests_CalculateMean, EmptyData_ThrowsInvalidArgument) {
    
    Analyzer analyzer;
    const std::vector<double> data {};

    EXPECT_THROW(analyzer.calculate_mean(data), std::invalid_argument);
}