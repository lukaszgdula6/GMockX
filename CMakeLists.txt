cmake_minimum_required(VERSION 2.8)

project(GMockX)

set(PROJECT_NAME_TESTS ${PROJECT_NAME})
include_directories(${PROJECT_NAME_TESTS},
    "."
    "Tests"
    "Tests/gmock"
    "Tests/gtest"
    "Example"
)
add_executable(${PROJECT_NAME_TESTS}
    "Tests/gmock_main.cc"
    "Tests/gmock-gtest-all.cc"
    "Tests/MockListTest.cpp"
    "Tests/MocksTest.cpp"
# notice there is no SolidDependency.cpp built
# this is because we replace this with mock in tests
    "Example/DependencyClient.cpp"
    "Example/DependencyClientTestSuite.cpp"
    "Example/SolidDependencyMock.cpp"
)
if (UNIX)
    target_link_libraries(${PROJECT_NAME_TESTS} pthread)
endif (UNIX)
