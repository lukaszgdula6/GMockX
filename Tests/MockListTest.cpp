#include "gmock.h"
#include "gtest.h"
#include "gtestx.hpp"

using namespace ::testing;

struct Class1 {};
struct Mock1 {};

struct Class2 {};
struct Mock2 {};

typedef MockList<Mock1, Class1> MockList1;
typedef MockList1::NoMockException NoMockException1;
typedef MockList1::PendingMockException PendingMockException1;

TEST(MockListTest, shouldThrowWhenNoMocksRegistered)
{
    Class1 c;
    EXPECT_THROW(MockList1::instance().findMockFor(&c), NoMockException1);
}

TEST(MockListTest, shouldAllowUnregisterNotRegisteredMock)
{
    Mock1 m;
    MockList1::instance().unregisterMock(&m);
}

TEST(MockListTest, shouldThrowWhenMockNotCalled)
{
    Class1 c;
    EXPECT_THROW(MockList1::instance().findMockFor(&c), NoMockException1);
}

TEST(MockListTest, shouldHaveRegisteredMock)
{
    Class1 c;
    Mock1 m;
    MockList1::instance().registerMock(&m);
    EXPECT_EQ(&m, MockList1::instance().findMockFor(&c));
}
