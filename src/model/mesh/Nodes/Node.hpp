#ifndef NODE_HPP_
#define NODE_HPP_

// #include "AbstractMesh.hpp"

#include "MathsFunctions.hpp"
// #include "CircleMeshGenerator.hpp"
#include "Parameters.h"
#include "utility/vec3.h"

#include <iostream>
#include <memory>
#include <vector>

// #include </opt/homebrew/Cellar/vtk/9.2.6_5/include/vtk-9.2/vtkCellArray.h>

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

// #include "AbstractNodeProperty.hpp"

// #include "Cell/Cell.hpp"

class Edge;
typedef boost::shared_ptr<Edge> EdgePtr;

class Node;
typedef boost::shared_ptr<Node> NodePtr;

class Node
{
public:
    Node(c_vector<double, 2> Location, int Index);
    ~Node();

    /** Member pointers */
        // std::vector<NodePropPtr> mpNodeProperties;

    /** Member variables */
        c_vector<double, 2> mLocation;
        c_vector<double, 2> mCurrentForceTally = Create_c_vector(0, 0);
        std::vector<NodePtr> mpNeighbours;
        std::vector<EdgePtr> mEdges;
        int mIndex;

    /** Member functions */
        c_vector<double, 2> GetNodeLocation();
        void UpdateLocation(c_vector<double, 2> NewLocation);
        void AddNodeForceContribution(c_vector<double, 2> Force);
        c_vector<double, 2> GetForce();
        void ClearForce();
        void SetNeighbours(std::vector<NodePtr>);
        // void RemoveNeighbour(NodePtr pNeighbourNode);
        void AddNeighbour(NodePtr pNeighbourNode);
        std::vector<NodePtr> GetNeighbours();
        void UpdateNeighbours(NodePtr);
        void SetEdge(EdgePtr pEdge);
        std::vector<EdgePtr> GetEdges();
        int GetNodeIndex();
        // void AddNodeProperty(NodePropPtr pNodeProperty, std::string PropertySetLabel);
        void DeleteEdge(EdgePtr pEdge);

    /** Map to Node Properties and functions to access and change properties */
        std::map<std::string, double> mNodeProperties;
        std::vector<std::string> mListOfNodeProperties;
        double GetNodeItem(std::string PropertyName);
        void SetNodeItem(std::string PropertyName, double Value);
        void DeclareNodeProperty(std::string PropertyName, double Value);
        std::vector<std::string> GetListOfNodeProperties();
        



};

#endif  // CELL_H
