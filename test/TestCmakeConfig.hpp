// MyTestSuite.h

#ifndef TestJessSuite_HPP_
#define TestJessSuite_HPP_


#include "Population.hpp"

// #include "CircleMeshGenerator.hpp"
// #include "LinearSpringForce.hpp"
// #include "MathsFunctions.hpp"
// #include "RadialForce.hpp"
// #include "Simulation.hpp"


#include </opt/homebrew/Cellar/vtk/9.2.6_5/include/vtk-9.2/vtkCellArray.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <utility>

class TestNewTest : public CxxTest::TestSuite
{
public:
    void TestBasicImplementation(void)
    {
        std::cout<<"  Test complete"<<std::endl;
    }
};
#endif