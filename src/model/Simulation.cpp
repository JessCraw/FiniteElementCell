#include "Simulation.hpp"

#include "LinearSpringForce.hpp"
#include "RadialForce.hpp"

// #include "helperFunctions/SmartPointers.hpp"

Simulation::Simulation(Population *rCellPopulation)
    : mpCellPopulation(rCellPopulation)
{
}
void Simulation::RunSimulation()
{
    SetUpSimulation();

    // Time stepping loop
    double t_current = 0;
    int counter = 0;
    while (t_current < mEndTime) {
        // Loop over all forces
        for (typename std::vector<ForcePtr>::iterator iter = mpForceCollection.begin(); iter != mpForceCollection.end();
             ++iter) {
            (*iter)->AddForceContribution(*mpCellPopulation);
        }

        /** Update the positions for each cell mesh, basic forwards time stepping */
        CalculateNextPositons();

        /** Write out the mesh for the current time step */
        // print_var3(counter, mSamplingTimestepMultiple, (counter%mSamplingTimestepMultiple));
        if ((mSamplingTimestepMultiple % counter) == 0) {
            mMeshWriter.WriteVTKCellMesh(counter);
        }

        counter += 1;

        t_current += mTimeStep;
    }
    print(" --- Simulation complete --- ");
}

void Simulation::CalculateNextPositons()
{
    // Loop over all the cells in the population
    std::vector<CellPtr> pCells = mpCellPopulation->GetCells();
    for (std::vector<CellPtr>::iterator celIt = pCells.begin(); celIt != pCells.end(); ++celIt) {
        /** Get the mesh from the cell  */
        MeshPtr pcellMesh = (*celIt)->GetMesh();

        /** Get the edges from the mesh  */
        std::vector<NodePtr> mpNodes = pcellMesh->GetNodes();

        for (std::vector<NodePtr>::iterator noIt = mpNodes.begin(); noIt != mpNodes.end(); ++noIt) {
            c_vector<double, 2> Force = (*noIt)->GetForce();
            // print_var2(Force[0],Force[1]);
            c_vector<double, 2> x_t = (*noIt)->GetNodeLocation();

            c_vector<double, 2> x_t1 = mTimeStep / mDragCoefficient * Force + x_t;
            (*noIt)->UpdateLocation(x_t1);
            (*noIt)->ClearForce();
        }
    }
}

// AddForce(boost::shared_ptr<AbstractForce<ELEMENT_DIM,SPACE_DIM> > pForce)
void Simulation::AddForce(boost::shared_ptr<AbstractForce> pForce) { mpForceCollection.push_back(pForce); }

void Simulation::SetUpSimulation()
{
    assert(mOutputDirectory != "");

    /** Set up the initial force configurations*/
    for (typename std::vector<ForcePtr>::iterator iter = mpForceCollection.begin(); iter != mpForceCollection.end();
         ++iter) {
        (*iter)->SetupForce(*mpCellPopulation);
    }

    VtkMeshWriter mMeshWriter = VtkMeshWriter(*mpCellPopulation, mOutputDirectory);
    mMeshWriter.WriteVTKCellMesh(0);
}

void Simulation::SetOutputDirectory(std::string Directory)
{
    if (Directory.back() != '/') {
        Directory += "/";
    }

    mOutputDirectory = OUTPUT_DIR + Directory;  // OUTPUT_DIR is defined in the cmake file
    if (!std::filesystem::exists(mOutputDirectory)) {
        std::filesystem::create_directory(mOutputDirectory);
    }
}

//  -----------------------------------

void Simulation::SetDt(double timestep) { mTimeStep = timestep; };

void Simulation::SetSamplingTimestepMultiple(int SamplingTimestepMultiple)
{
    mSamplingTimestepMultiple = SamplingTimestepMultiple;
}

// Jess wrote these functions: I want it to mimic chaste
void Simulation::SetEndTime(double endTime)
{
    assert(endTime > 0);
    mEndTime = endTime;
}
