/*
 * Copyright (C) 2020 Lukasz Gdula
 *
 * Licensed under the Mozilla Public License 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.mozilla.org/en-US/MPL/2.0/
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
