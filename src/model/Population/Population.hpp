#ifndef POPULATION
#define POPULATION

#include "AbstractMesh.hpp"
#include "Cell/Cell.hpp"
#include "CircleMeshGenerator.hpp"
#include "MathsFunctions.hpp"
#include "Parameters.h"
#include "utility/vec3.h"

#include </opt/homebrew/Cellar/vtk/9.2.6_5/include/vtk-9.2/vtkCellArray.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

// #include "AbstractNodeProperty.hpp"

class Population
{
public:
    Population(int numCells, int CellRefinment = 20, int CellRadius = 1);

    /** Member pointers */
    /** Vector of cells. */
    std::vector<CellPtr> mCells;
    std::vector<MeshPtr> mMeshes;

    /** Member variables */
    int mCellsRefinment;
    int mCellsRadius;

    /** Member functions */
    std::vector<CellPtr> GetCells();
    // void CreateNodeProperties(AbstractNodeProperty& prototype, std::string PropertiesName);

    // std::vector<std::unique_ptr<Cell>> mCells;
    // Population(AbstractMesh* pMesh, int numCells);
    // Population(CircleMeshGenerator *pMesh, int numCells);

    // Member variables
    AbstractMesh *mpMesh;
    int mNumCells;
};

#endif  // CELL_H
