#include "Edge.hpp"

// Think I will create a couple of different constructors for acouple of different types of meshes

Edge::Edge(std::pair<NodePtr, NodePtr> NodePair)
    : mNodePair(NodePair),
      mNodeA(NodePair.first),
      mNodeB(NodePair.second)
{
    // mNodeA = NodePair.first;
    // mNodeB = NodePair.Second;
    mNodeAIndex = mNodeA->GetNodeIndex();
    mNodeBIndex = mNodeB->GetNodeIndex();
}
Edge::~Edge() {}

std::pair<int, int> Edge::GetNodeIndexPair()
{
    return std::make_pair(mNodeAIndex, mNodeBIndex);
    ;
}

c_vector<double, 2> Edge::GetEdgeNormal()
{
    c_vector<double, 2> EdgeVector = mNodeB->GetNodeLocation() - mNodeA->GetNodeLocation();
    double EdgeVectorMagnitude = norm_2(EdgeVector);
    c_vector<double, 2> Normal =
        Create_c_vector(EdgeVector[1] / EdgeVectorMagnitude, -EdgeVector[0] / EdgeVectorMagnitude);

    return Normal;
}

NodePtr Edge::GetNodeA() { return mNodeA; }

NodePtr Edge::GetNodeB() { return mNodeB; }
