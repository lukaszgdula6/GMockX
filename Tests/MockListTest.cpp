#include "gmock.h"
#include "gtest.h"
#include "gtestx.hpp"

using namespace ::testing;

struct Class1 {};
struct Mock1 {};

struct Class2 {};
struct Mock2 {};

TEST(MockListTest, shouldHaveInstance)
{
    EXPECT_THAT((&MockList<Mock1, Class1>::instance()), Not(IsNull()));
    EXPECT_THAT((&MockList<Mock2, Class2>::instance()), Not(IsNull()));
    EXPECT_NE(reinterpret_cast<void*>(&MockList<Mock1, Class1>::instance()),
              reinterpret_cast<void*>(&MockList<Mock2, Class2>::instance()));
}

struct MockListTestSuite : Test
{
    typedef MockList<Mock1, Class1> MockList1;
    typedef MockList1::NoMockException NoMockException1;
    typedef MockList1::PendingMockException PendingMockException1;

    MockList1 mockList1;
};

TEST_F(MockListTestSuite, shouldThrowWhenNoMocksRegistered)
{
    Class1 c;
    EXPECT_THROW(mockList1.findMockFor(&c), NoMockException1);
}

TEST_F(MockListTestSuite, shouldAllowUnregisterNotRegisteredMock)
{
    Mock1 m;
    mockList1.unregisterMock(&m);
}

TEST_F(MockListTestSuite, shouldThrowWhenMockNotCalled)
{
    Class1 c;
    EXPECT_THROW(mockList1.findMockFor(&c), NoMockException1);
}

TEST_F(MockListTestSuite, shouldHaveRegisteredMock)
{
    Class1 c;
    Mock1 m;
    mockList1.registerMock(&m);
    EXPECT_EQ(&m, mockList1.findMockFor(&c));
}
