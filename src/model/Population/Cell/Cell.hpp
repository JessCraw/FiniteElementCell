#ifndef CELL_H
#define CELL_H


#include "MathsFunctions.hpp"
#include "opencv2/core/types.hpp"

#include "CircleMeshGenerator.hpp"

#include <boost/enable_shared_from_this.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <cmath>
// #include <gcem.hpp>
#include <list>
#include <memory>
#include <queue>
#include <vector>

#include "opencv2/core/types.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <map>
#include <numeric>
#include <utility>

class Grid;
// class NodeOLD;

// class Spring;
class Cell;
typedef boost::shared_ptr<Cell> CellPtr;

class Cell

{
public:
    // Cell(const Coordinates &coordinates, Grid *grid);
    Cell();

    void CreateMesh(MeshPtr Mesh);

    // Member variable that is the mesh
    // CircleMeshGenerator* mMesh;
    MeshPtr mpMesh;
    MeshPtr GetMesh();

    //

    // After here is old and to be stripped
    Cell(const std::vector<Coordinates> &coords, Grid *grid);

    // static constexpr std::array<double, Proteins::MembraneReceptorsArray.size()> InitialReceptorValues = [] {
    //     std::array<double, Proteins::MembraneReceptorsArray.size()> retval {0};
    //     retval[static_cast<unsigned long>(Proteins::MembraneReceptors::VEGFR)] = 128;
    //     retval[static_cast<unsigned long>(Proteins::MembraneReceptors::NOTCH)] = 128;
    //     return retval;
    // }();

    // TODO: move this to constants file?
    static constexpr double InitialVegfr = 128;
    static constexpr double InitialNotch = 128;
    static constexpr unsigned int width = 100;
    static constexpr unsigned int mRadius = 23;
    // Calculate nodesPerCell to ensure spring lengths are within range
    // static constexpr unsigned int nodesPerCell = gcem::ceil(  (2.0 * M_PI * mRadius) / (2.0 *
    // Environment::Grid::getRangeAtCoarseness(Environment::Grid::coarsenessMax)));
    // static constexpr unsigned int nodesPerCell = static_cast<unsigned int>(gcem::ceil(
    //     (2.0 * M_PI * mRadius) / (2.0 * Environment::Grid::getRangeAtCoarseness(Environment::Grid::coarsenessMax))));

    /**
     * Calls update on springs.
     */
    void updateMembrane();

    /*
     * Update receptor active totals and ligand totals from springs.
     * Active values accumulated in current timestep are added to back of queue of predefined length. Queue is popped to
     * get active value for cell to use in current timestep. This models translocation time.
     */
    void updateProteins();

    /*
     * Gene regulation network of cell. Uses Cell level ODE definition to calculate new protein values. Cell GRN occurs
     * after agent level update. New protein values from ODE are scaled out to agents.
     */
    void geneRegulationNetwork();

    float getTotalLength() const;

    // std::array<double, Proteins::N_CELL_PROTEINS> getProteinValues() const;

    /*
     * Container holding Cell instance's protein values.
     */
    // CellProteins mProteins;

    int getActinUsed() const { return actinUsed; }

    std::vector<cv::Point2f> getAllNodePositionsForPolygonTest();

    /*
     * Functions Jess has added
     */
    // std::vector<std::shared_ptr<Node>> GetNodes();
    std::vector<Coordinates> GetAllNodePositions();
    // std::vector<Spring *> GetSpringPointers();
    // std::vector<c_vector<int, 2>> GetSprings();

    std::vector<int> GetSpringIndicies();
    int GetCellID();
    std::vector<int> GetNodeList();
    // List Of nodes in the cell
    std::vector<int> mNodeList;

    // double GetNumberOfSprings();
    // int mNumberOfSprings;

    int ELEMENT_DIM = 2;  // I need to update this for the triangle

private:
    // std::vector<std::shared_ptr<Node>> mNodes;
    static int idCounter;
    const int id;

    int timerHack = 0;
    double changeHack = 0.05;  // What is this?? Jess changed to double because it keeps throwing up a stupid error

    void updateProteinTotals();

    void createMembrane(const Coordinates &coordinates);

    Grid *grid;

    void applyHookesLawToNodes();

    /**
     * Gets approximate center of cell by averaging coordinates of all nodes.
     *
     * @return    Approximate center of cell by averaging coordinates of all nodes.
     */
    Coordinates getCenter() const;

    /*
     * FIFO queue modelling the delay protein translocation takes to / from cell nucleus.
     * Delay time set in Constants.h Proteins namespace.
     */
    // std::queue<double, std::list<double>> vegfrTranslocation;
    // std::array<std::queue<double, std::list<double>>, Proteins::MembraneReceptorsArray.size()>
    //     proteinActiveTranslocation;

    int actinUsed = 0;
};

#endif  // CELL_H
