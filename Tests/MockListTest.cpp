#include "gmock.h"
#include "gtest.h"
#include "gtestx.hpp"

using namespace ::testing;

struct Class1 {};
struct Mock1 {};

struct Class2 {};
struct Mock2 {};

struct MockListTestSuite : Test
{
    MockList<Mock1, Class1> list1;
    MockList<Mock2, Class2> list2;
};

TEST_F(MockListTestSuite, shouldThrowWhenNoMocksRegistered)
{
    typedef MockList<Mock1, Class1>::NoMockException NoMockException;
    Class1 c;
    EXPECT_THROW(list1.findMockFor(&c), NoMockException);
}

TEST_F(MockListTestSuite, shouldAllowUnregisterNotRegisteredMock)
{
    Mock1 m;
    list1.unregisterMock(&m);
}
