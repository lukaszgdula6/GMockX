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
    typedef MockList1::MockForNullException MockForNullException1;
    typedef MockList1::NoMockException NoMockException1;
    typedef MockList1::PendingMockException PendingMockException1;
    typedef MockList1::MockAlreadyRegisteredException MockAlreadyRegisteredException1;
    typedef MockList1::MockNotRegisteredException MockNotRegisteredException1;
    typedef MockList<Mock2, Class2> MockList2;
    typedef MockList2::NoMockException NoMockException2;

    MockList1 mockList1;
    MockList2 mockList2;
};

TEST_F(MockListTestSuite, shouldThrowWhenLookingForMockForNullPtr)
{
    Class1 *c = NULL;
    EXPECT_THROW(mockList1.findMockFor(c), MockForNullException1);
}

TEST_F(MockListTestSuite, shouldThrowWhenNoMocksRegistered)
{
    Class1 c;
    EXPECT_THROW(mockList1.findMockFor(&c), NoMockException1);
}

TEST_F(MockListTestSuite, shouldNotAllowUnregisterNotRegisteredMock)
{
    Mock1 m;
    EXPECT_THROW(mockList1.unregisterMock(&m), MockNotRegisteredException1);
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
    EXPECT_EQ(&m, mockList1.findMockFor(&c));
    EXPECT_EQ(&m, mockList1.findMockFor(&c));
}

TEST_F(MockListTestSuite, shouldNotAllowRegisteringSameMockTwice)
{
    Class1 c;
    Mock1 m;
    mockList1.registerMock(&m);
    EXPECT_THROW(mockList1.registerMock(&m), MockAlreadyRegisteredException1);
}

TEST_F(MockListTestSuite, shouldMockListsBeIndependent)
{
    Class1 c1;
    Mock1 m1;
    Class2 c2;
    Mock2 m2;
    mockList1.registerMock(&m1);
    mockList2.registerMock(&m2);

    EXPECT_EQ(&m1, mockList1.findMockFor(&c1));
    EXPECT_EQ(&m2, mockList2.findMockFor(&c2));
    EXPECT_THROW(mockList1.findMockFor(reinterpret_cast<Class1*>(&c2)), NoMockException1);
    EXPECT_THROW(mockList2.findMockFor(reinterpret_cast<Class2*>(&c1)), NoMockException2);
}

TEST_F(MockListTestSuite, shouldMocksBeIndependlyBinded)
{
    Class1 c1, c2;
    Mock1 m1, m2;

    mockList1.registerMock(&m1);
    EXPECT_EQ(&m1, mockList1.findMockFor(&c1));
    mockList1.registerMock(&m2);
    EXPECT_EQ(&m2, mockList1.findMockFor(&c2));
    EXPECT_EQ(&m1, mockList1.findMockFor(&c1));
}

TEST_F(MockListTestSuite, shouldMockBeBindedToOnlyOneObject)
{
    Class1 c1, c2;
    Mock1 m;

    mockList1.registerMock(&m);
    mockList1.findMockFor(&c1);
    EXPECT_THROW(mockList1.findMockFor(&c2), NoMockException1);
}

TEST_F(MockListTestSuite, shouldMockBeBindedOnce)
{
    Class1 c;
    Mock1 m1, m2;

    mockList1.registerMock(&m1);
    mockList1.registerMock(&m2);
    EXPECT_EQ(&m1, mockList1.findMockFor(&c));
    EXPECT_EQ(&m1, mockList1.findMockFor(&c));
}

TEST_F(MockListTestSuite, unregisterMockShouldThrowOnUnusedMock)
{
    Mock1 m;

    mockList1.registerMock(&m);
    EXPECT_THROW(mockList1.unregisterMock(&m), PendingMockException1);
}

TEST_F(MockListTestSuite, unregisteredMockShouldBeNotAssigned)
{
    Class1 c;
    Mock1 m;

    mockList1.registerMock(&m);
    mockList1.findMockFor(&c);
    mockList1.unregisterMock(&m);
    EXPECT_THROW(mockList1.findMockFor(&c), NoMockException1);
}

TEST_F(MockListTestSuite, shouldForgetMock)
{
    Class1 c;
    Mock1 m1, m2;

    mockList1.registerMock(&m1);
    mockList1.registerMock(&m2);
    EXPECT_EQ(&m1, mockList1.findMockFor(&c));
    mockList1.forget(&m1);
    EXPECT_EQ(&m2, mockList1.findMockFor(&c));
}

TEST_F(MockListTestSuite, shouldNotForgetUnusedMock)
{
    Mock1 m;
    EXPECT_THROW(mockList1.forget(&m), MockNotRegisteredException1);
    mockList1.registerMock(&m);
    EXPECT_THROW(mockList1.forget(&m), MockNotRegisteredException1);
}
