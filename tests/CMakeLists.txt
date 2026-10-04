add_executable(unit_test 
    stocker_analytic_test.cpp
    stocker_data_test.cpp
)

target_link_libraries(unit_test PRIVATE
    stocker_analytics
    stocker_data
    GTest::gtest_main
)

