#ifndef ABSTRACTMESH_HPP_
#define ABSTRACTMESH_HPP_

#include "MathsFunctions.hpp"
#include "Parameters.h"

#include <iostream>
#include <memory>
#include <vector>
// #include "debug/Instrumentor.h"
#include "utility/vec3.h"

#include </opt/homebrew/Cellar/vtk/9.2.6_5/include/vtk-9.2/vtkCellArray.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <ctime>
// #include <iostream>
// #include <memory>
#include "Edges/Edge.hpp"
#include "Nodes/Node.hpp"
#include "SmartPointers.hpp"

#include <utility>

class AbstractMesh
// typedef boost::shared_ptr<AbstractMesh> MeshPtr;
{
public:
    AbstractMesh();

    /** Member pointers */
    std::vector<NodePtr> mpNodes;
    std::vector<EdgePtr> mpEdges;

    /** Member variables */
    std::vector<c_vector<double, 2>> mNodesVector;
    std::vector<std::pair<int, int>> mEdgesVector;
    std::vector<std::vector<int>> mNeighboursVector;

    int mNumberOfCells = 1;
    std::string mMeshType;

    /** Function */
    std::vector<c_vector<double, 2>> GetNodesVector();
    std::vector<std::pair<int, int>> GetEdgeVector();
    std::vector<std::vector<int>> GetNeighbours();
    std::vector<NodePtr> GetNodes();
    std::vector<EdgePtr> GetEdges();

    std::string GetMeshType();
    // This funciton will tell us how many cells are in a mesh
    // to be implemented when I code is reading in mesh
    std::vector<std::vector<int>> DetermineMeshConnectivity();

    void SetMeshType();
};

#endif  // CELL_H
