// MyTestSuite.h

#ifndef TestJessSuite_HPP_
#define TestJessSuite_HPP_


#include "Population/Population.hpp"

#include "CircleMeshGenerator.hpp"
#include "LinearSpringForce.hpp"
#include "MathsFunctions.hpp"
#include "RadialForce.hpp"
#include "Simulation.hpp"

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
        // Create Circular Mesh

        // Set up the mesh writer enviroment
        // CircleMeshGenerator mesh(10, 5);
        // MutableMesh<2, 3> *mutable_mesh = generator.GetMesh();

        // Set up the cell population, the meshes for each cell are in here
        Population cellPopulation(3);

        // Create the simultor where all the magic happens
        Simulation simulator(&cellPopulation);

        simulator.SetOutputDirectory("ThreeCells");
        simulator.SetEndTime(6);
        simulator.SetDt(0.1);
        simulator.SetSamplingTimestepMultiple(4);

        /** Make the force pointers and send them into the simulation */
        MAKE_PTR(LinearSpringForce, pspringforce);
        pspringforce->SetSpringConstant(0.00001);
        MAKE_PTR(RadialForce, pRadialForce);
        pRadialForce->SetPressure(0.05);

        simulator.AddForce(pspringforce);
        simulator.AddForce(pRadialForce);

        simulator.SetUpSimulation();

        // simulator.runSimulation();  // Equivalent to simulator.Solve()

        // int counter = 0;
        // MeshWriter.WriteVTKMesh(counter);

        // for (int time = 0; time < mEndTime; ++time)
        // {
        //     oneTimestep();
        //     mtime += mTimeStep;

        //     if (counter % mSamplingTimestepMultiple == 1)
        //     {
        //         MeshWriter.WriteVTKMesh(counter);
        //     }
        //     counter += 1;
        // }

        // OffLatticeSimulation<2> simulator(cell_population); // Want to be able to feed in the cells to the world,
        // which is basically the simulator
        // Instrumentor::get().beginSession("Profile");

        // std::cout << "\n Graphics is true \n  \n " << std::endl;
        // // Graphics::run();

        // // Create cells std::vector<std::unique_ptr<Cell>> cells;
        // // c_vector<double, 3> NewCev = Create_c_vector(1, 1, 1);
        // double Nd = 1;
        // auto world = std::make_unique<World>();
        // // world->CreateCells();

        // world->SetOutputDirectory("Test5");
        // world->SetEndTime(1);
        // world->SetDt(1);
        // world->SetSamplingTimestepMultiple(200);
        // world->runSimulation();  // Equivalent to simulator.Solve()
        // Instrumentor::get().endSession();

        // / opt / homebrew / Cellar / petsc / 3.20.1
        // print(1);
        // print_var2(Nd, Nd);
        // print_var3(Nd, Nd, Nd);
        // print_var1(Nd);

        // c_vector<double, 3> Vector = Create_c_vector(1, 1, 1);
        // print(Vector);
    }
};
#endif