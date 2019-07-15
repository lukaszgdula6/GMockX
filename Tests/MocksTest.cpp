#include "gtestx.hpp"
#include "gmock.h"
#include "gtest.h"

using namespace ::testing;

struct Class1
{

};

struct Mock1
{
    MAKE_GMOCKX_MOCK_TORS(Mock1, Class1)
    MOCK_METHOD0(method, void());
};

typedef MockList<Mock1, Class1> MockList1;

TEST(MocksTest, shouldRegisterNewMock)
{
    Mock1 m;
    Class1 c;
    EXPECT_EQ(&m, MockList1::instance().findMockFor(&c));
}

TEST(MocksTest, shouldUnregisterOldMock)
{
    Class1 c;
    {
        Mock1 m;
        EXPECT_EQ(&m, MockList1::instance().findMockFor(&c));
    }
    EXPECT_THROW(MockList1::instance().findMockFor(&c), MockList1::NoMockException);
}

TEST(MocksTest, shouldRegisterNewStaticMock)
{
    Mock1 m(GMOCKX_NO_DYNAMIC_REGISTRATION, GMOCKX_STATIC_REGISTRATION);
    Class1 c;
    EXPECT_EQ(&m, MockList1::instance().getStaticMock());
}

TEST(MocksTest, shouldRegisterNewDYnamicAndStaticMock)
{
    Mock1 m(GMOCKX_DYNAMIC_REGISTRATION, GMOCKX_STATIC_REGISTRATION);
    Class1 c;
    EXPECT_EQ(&m, MockList1::instance().getStaticMock());
    EXPECT_EQ(&m, MockList1::instance().findMockFor(&c));
}
