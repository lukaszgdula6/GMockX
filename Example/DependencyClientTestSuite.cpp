#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SolidDependencyMock.hpp"
#include "DependencyClient.hpp"

using namespace ::testing;

struct DependencyClientTestSuite : Test
{
// Mock for SolidDependency instance
    StrictMock<SolidDependencyMock> solidMock;
// We can also have multiple instances of same mock, each instance of SolidDependency
// would get one instance of SolidDependencyMock
    StrictMock<SolidDependencyMock> solidMock2;
// Mock for SolidDependency statics only
// changing parameters we can instantiate one mock both for static and instance
    StrictMock<SolidDependencyMock> staticSolidMock { GMOCKX_NO_DYNAMIC_REGISTRATION, GMOCKX_STATIC_REGISTRATION };

    DependencyClient sut;
};

TEST_F(DependencyClientTestSuite, shouldForwardToMockWhenDependencyCalled)
{
    const int result = 42;
    const int param = 666;

    EXPECT_CALL(solidMock, setField(param)).WillOnce(Return(result));

    EXPECT_EQ(result, sut.useDependency(param));
}

TEST_F(DependencyClientTestSuite, shouldForwardToTwoMocksWhenDependencyCalledTwice)
{
    const int result1 = 11, result2 = 22, result3 = 33;
    const int param1 = 44, param2 = 55, param3 = 66;

    EXPECT_CALL(solidMock, setField(param1)).WillOnce(Return(result1));
// after second call we need to forgot mapping of solidMock to SolidDependency instance on the stack because... it will always have the same address!
// mapping class instance to mock instance is based on types and addresses
    EXPECT_CALL(solidMock, setField(param2)).WillOnce(DoAll(Forget(&solidMock), Return(result2)));
// solidMock is forgotten so next call to setField has to map next registered mock - solidMock2
    EXPECT_CALL(solidMock2, setField(param3)).WillOnce(Return(result3));

    EXPECT_EQ(result1, sut.useDependency(param1));
    EXPECT_EQ(result2, sut.useDependency(param2));
    EXPECT_EQ(result3, sut.useDependency(param3));
}

TEST_F(DependencyClientTestSuite, shouldForwardToMockWhenStaticDependencyCalled)
{
    const int result = -42;
    const int param = -666;

    EXPECT_CALL(staticSolidMock, setStaticField(param)).WillOnce(Return(result));

    EXPECT_EQ(result, sut.useStaticDependency(param));
}
