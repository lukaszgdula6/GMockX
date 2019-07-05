#include "gtestx.hpp"
#include "gmock.h"
#include "gtest.h"

using namespace ::testing;

struct Mock1
{
    MAKE_GMOCKX_MOCK_TORS(Mock1)

};
