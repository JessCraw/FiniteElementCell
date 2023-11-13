// MyTestSuite.h
#include <cxxtest/TestSuite.h>

#include <array>
#include <cmath>
#include <iostream>
class TestNewTest : public CxxTest::TestSuite
{
public:
    void testAddition(void)
    {
        TS_ASSERT(1 + 1 > 1);
        TS_ASSERT_EQUALS(1 + 1, 2);
        std::cout << "\n Words" << std::endl;
    }
};
