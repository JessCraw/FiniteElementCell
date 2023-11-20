#include "Simulation.hpp"

#include "LinearSpringForce.hpp"
#include "RadialForce.hpp"
// #include "AbstractNodeProperty.hpp"
Simulation::Simulation(Population *rCellPopulation)
    : mpCellPopulation(rCellPopulation)
{
}
void Simulation::RunSimulation()
{
    print(" ---  Setup simulation  --- need to put this all in the SetUpSimulation() function --- ");
    SimpleRemeshing Remesher = SimpleRemeshing();
    // Remesher.AddForceCollection(mpForceCollection);
    VtkMeshWriter mMeshWriter = VtkMeshWriter(*mpCellPopulation, mOutputDirectory);
    mMeshWriter.WriteVTKCellMesh(0);
    Remesher.CallRemeshing(*mpCellPopulation);
    
    SetUpSimulation();
    mMeshWriter.WriteVTKCellMesh(1);


    // Setup remesher 
    


    std::srand(static_cast<unsigned int>(std::time(0)));

    // Time stepping loop
    double t_current = 0;
    int counter = 2;
    print(" --- Running simulation --- ");
    while (t_current < mEndTime) {

        /** Update the signalling across all cells */
        for (typename std::vector<SignallingPtr>::iterator iter = mpSignallingCollection.begin(); iter != mpSignallingCollection.end();
            ++iter) {
            (*iter)->UpdateSignalling(*mpCellPopulation);
        }

        /** Loop over all the forces and save them in the nodes */
        for (typename std::vector<ForcePtr>::iterator iter = mpForceCollection.begin(); iter != mpForceCollection.end();
             ++iter) {
            (*iter)->AddForceContribution(*mpCellPopulation);
        }

        /** Update the positions for each cell mesh, basic forwards time stepping */
        CalculateNextPositons();

        // Call remeshing from here. currently random 
        // Generate a random number between 0 and 1
        double randomNumber = static_cast<double>(std::rand()) / RAND_MAX;
        // double threshold = 0.00001;
        // if (randomNumber < threshold) 
        // {
        //     Remesher.CallRemeshing(*mpCellPopulation);
        // }

        


        /** Write out the mesh for the current time step */
        if ((counter%mSamplingTimestepMultiple) == 0) {
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

void Simulation::AddSignalling(SignallingPtr pSignal) { mpSignallingCollection.push_back(pSignal); }

void Simulation::SetUpSimulation()
{
    assert(mOutputDirectory != "");

    /** Set up the initial force configurations*/
    for (typename std::vector<ForcePtr>::iterator iter = mpForceCollection.begin(); iter != mpForceCollection.end();
         ++iter) {
        (*iter)->SetupForce(*mpCellPopulation);
    }

    /** Set up the initial configuration for the signalling factors*/
    for (typename std::vector<SignallingPtr>::iterator iter = mpSignallingCollection.begin(); iter != mpSignallingCollection.end();
         ++iter) {
        (*iter)->SetupSignalling(*mpCellPopulation, mTimeStep);
    }

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
