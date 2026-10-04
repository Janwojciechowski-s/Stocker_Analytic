#include <gtest/gtest.h>
#include "FileDataProvider.h"


TEST(StockerData_FileDataProvider, EmptyInput_RecordsEmptyAndWarningGenerated){
    FileDataProvider provider {};
    DataResult result = provider.get_data("");

    EXPECT_TRUE(result.records.empty());
    ASSERT_FALSE(result.warnings.empty());
    EXPECT_EQ(result.warnings[0], "Error: received empty data string.");
}


TEST(StockerData_FileDataProvider, ValidInput_CorrectOutput){
    FileDataProvider provider {};
    std::string valid_csv = 
    "Date,Open,High,Low,Close,Volume\n"
    "2024-01-02,150.5,152.0,149.0,151.2,1200500\n"
    "2024-01-03,151.2,155.0,150.8,154.6,980000\n";

    DataResult result = provider.get_data(valid_csv);

    EXPECT_EQ(result.records.size(), 2);
    EXPECT_TRUE(result.warnings.empty());
    EXPECT_EQ(result.records[0].date, "2024-01-02");
    EXPECT_DOUBLE_EQ(result.records[0].open, 150.5);
    EXPECT_EQ(result.records[0].volume, 1200500);
}


TEST(StockerData_FileDataProvider, EnterOnlyHeader_RecordsEmptyAndWarningGenerated){
    FileDataProvider provider {};
    std::string header_only_csv = "Date,Open,High,Low,Close,Volume\n";
    DataResult result = provider.get_data(header_only_csv);

    EXPECT_TRUE(result.records.empty());
    ASSERT_FALSE(result.warnings.empty());
    EXPECT_EQ(result.warnings[0], "Data contains only header.");
}
