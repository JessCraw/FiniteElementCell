#include "Node.hpp"

// Think I will create a couple of different constructors for acouple of different types of meshes

Node::Node(c_vector<double, 2> Location, int Index)
    : mLocation(Location),
      mIndex(Index)
{
}
Node::~Node() {}

void Node::UpdateLocation(c_vector<double, 2> NewLocation) { mLocation = NewLocation; }

int Node::GetNodeIndex() { return mIndex; }

c_vector<double, 2> Node::GetNodeLocation() { return mLocation; }

void Node::SetNeighbours(std::vector<NodePtr> Neighbours) { std::vector<NodePtr> mNeighbours = Neighbours; }

void Node::SetEdge(EdgePtr pEdge) { mEdges.push_back(pEdge); }

std::vector<EdgePtr> Node::GetEdge() { return mEdges; }

void Node::AddNodeForceContribution(c_vector<double, 2> Force) { mCurrentForceTally += Force; }

c_vector<double, 2> Node::GetForce() { return mCurrentForceTally; }

void Node::ClearForce()
{
    mCurrentForceTally.clear();
    mCurrentForceTally = Create_c_vector(0, 0);
}
