#ifndef EDGE_HPP_
#define EDGE_HPP_

#include "MathsFunctions.hpp"
#include "Parameters.h"
#include "utility/vec3.h"

#include <iostream>
#include <memory>
#include <vector>

// #include </opt/homebrew/Cellar/vtk/9.2.6_5/include/vtk-9.2/vtkCellArray.h>

#include "Nodes/Node.hpp"

#include <algorithm>
#include <array>
#include <boost/enable_shared_from_this.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <utility>

// #include "Cell/Cell.hpp"
class Edge;
typedef boost::shared_ptr<Edge> EdgePtr;
class Edge
{
public:
    Edge(std::pair<NodePtr, NodePtr> NodePair);
    ~Edge();

    /** Member variables */
    std::pair<NodePtr, NodePtr> mNodePair;
    NodePtr mNodeA;
    NodePtr mNodeB;

    int mNodeAIndex;
    int mNodeBIndex;

    /** Member functions */
    void UpdateNodePair(std::pair<NodePtr, NodePtr> NewNodePair);
    c_vector<double, 2> GetEdgeNormal();

    NodePtr GetNodeA();
    NodePtr GetNodeB();
    std::pair<int, int> GetNodeIndexPair();
};

#endif  // CELL_H
