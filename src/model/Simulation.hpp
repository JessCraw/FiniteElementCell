#ifndef SIMULATION
#define SIMULATION

#include "Population.hpp"
// #include "Constants.h"
#include "MathsFunctions.hpp"
// #include "Parameters.h"
#include "/Users/crawshj/Documents/Projects/Tufts/adaptive-cell-effort-model/build/config.h"
#include "AbstractForce.hpp"
#include "AbstractSignalling.hpp"
#include "Cell/Cell.hpp"
#include "SmartPointers.hpp"
#include "VtkMeshWriter.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include "SimpleRemeshing.hpp"

#include <cstdlib>   // For rand() function
#include <ctime>     // For seeding the random number generator

class Simulation
{
public:
    Simulation(Population *rCellPopulation);

    /** Run the simulation */
    void RunSimulation();

    /** Functions */
    void AddForce(boost::shared_ptr<AbstractForce> pForce);
    void AddSignalling(SignallingPtr pSignal);
    void AddMeshAdaption(MeshPtr pMeshAdaption);
    void CalculateNextPositons();

    /** Member pointers */
    Population *mpCellPopulation;
    /** The mechanics used to determine the new location of the cells, a list of the forces. */
    std::vector<ForcePtr> mpForceCollection;
    /** Collection of all the signalling functions in the populaiton */
    std::vector<SignallingPtr> mpSignallingCollection;    // std::vector<boost::shared_ptr<AbstractForce<ELEMENT_DIM, SPACE_DIM> > > mForceCollection;
    /** Collection of classes to update the mesh */
    std::vector<MeshPtr> mpMeshAdaptionCollection;    // std::vector<boost::shared_ptr<AbstractForce<ELEMENT_DIM, SPACE_DIM> > > mForceCollection;



    /** Member variables */
    double mDragCoefficient = 10;
    VtkMeshWriter mMeshWriter = VtkMeshWriter(*mpCellPopulation, "");

    /** Time keeping functions and members */
    /** Time keeping functions */
    void SetEndTime(double endTime);
    void oneTimestep();
    int getTimestep() const { return mTimeStep; }
    void SetDt(double timestep);
    void SetSamplingTimestepMultiple(int SamplingTimestepMultiple);

    /** Time keeping members */
    double mTimeStep = 1;
    double mEndTime = 1000;
    double mtime = 0;
    int mSamplingTimestepMultiple;

    /** Book keeping functions and members */
    /** Book keeping functions */
    void SetOutputDirectory(std::string Directory);
    void SetUpSimulation();
    /** Book keeping members */
    std::string mOutputDirectory;
};

#endif  // Simulation_H
