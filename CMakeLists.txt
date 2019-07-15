cmake_minimum_required(VERSION 2.8)

project(GMockX)
add_executable(${PROJECT_NAME}
    "main.cpp"
)

project(GMockX_tests)
set(PROJECT_NAME_TESTS ${PROJECT_NAME})
include_directories(${PROJECT_NAME_TESTS},
    "."
    "Tests"
    "Tests/gmock"
    "Tests/gtest"
)
add_executable(${PROJECT_NAME_TESTS}
    "Tests/gmock_main.cc"
    "Tests/gmock-gtest-all.cc"
    "Tests/MockListTest.cpp"
    "Tests/MocksTest.cpp"
)
if (UNIX)
    target_link_libraries(${PROJECT_NAME_TESTS} pthread)
endif (UNIX)
