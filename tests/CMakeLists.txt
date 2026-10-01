add_executable(unit_test 
    stocker_analytic_test.cpp
)

target_link_libraries(unit_test PRIVATE
    stocker_analytics
    GTest::gtest_main
)

