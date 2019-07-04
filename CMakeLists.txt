cmake_minimum_required(VERSION 2.8)

project(GMockX)
add_executable(${PROJECT_NAME}
    "main.cpp"
)

project(GMockX_tests)
set(PROJECT_NAME_TESTS ${PROJECT_NAME}_tests)
include_directories(${PROJECT_NAME_TESTS},
    "tests"
    "tests/gmock"
    "tests/gtest"
)
add_executable(${PROJECT_NAME_TESTS}
    "tests/gmock_main.cc"
    "tests/gmock-gtest-all.cc"
)
