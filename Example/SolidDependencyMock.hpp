#pragma once

#include "gmockx.hpp"
#include "gmock/gmock.h"
#include "SolidDependency.hpp"

using namespace ::testing;

struct SolidDependencyMock
{
    MAKE_GMOCKX_MOCK_TORS(SolidDependencyMock, SolidDependency)
    MOCK_METHOD1(setField, int (int field));
    MOCK_METHOD1(setStaticField, int (int field));
};
