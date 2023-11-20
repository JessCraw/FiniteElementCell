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

void Node::SetNeighbours(std::vector<NodePtr> Neighbours) 
{
    mpNeighbours = Neighbours; 
}


// void RemoveNeighbour(NodePtr pNeighbourNode)
// {
//    auto it = std::find(mpNeighbours.begin(), mpNeighbours.end(), pNeighbourNode);
//     // Check if the element was found before erasing
//     if (it != mpNeighbours.end()) {
//         // Erase the element from the vector
//         mpNeighbours.erase(it);
//        } else {
//         std::cout << "Neighbour not found in the Neighbour vector. -- this is bad" << std::endl;
//     }
// }

// void AddNeighbour(NodePtr pNeighbourNode)
// {
//     mpNeighbours.push_back(pNeighbourNode);
//     // std::vector<NodePtr> mpNeighbours;
// }


void Node::SetEdge(EdgePtr pEdge) { mEdges.push_back(pEdge); }

std::vector<EdgePtr> Node::GetEdges() { return mEdges; }

void Node::DeleteEdge(EdgePtr pEdge) 
{
    auto it = std::find(mEdges.begin(), mEdges.end(), pEdge);
    // Check if the element was found before erasing
    if (it != mEdges.end()) 
    {
        // Erase the element from the vector
        mEdges.erase(it);
    } 
}

void Node::AddNodeForceContribution(c_vector<double, 2> Force) { mCurrentForceTally += Force; }

c_vector<double, 2> Node::GetForce() { return mCurrentForceTally; }

void Node::ClearForce()
{
    mCurrentForceTally.clear();
    mCurrentForceTally = Create_c_vector(0, 0);
}


/** Map to Node Properties and functions to access and change properties */

double Node::GetNodeItem(std::string PropertyName)
{
    return mNodeProperties[PropertyName];
}
void Node::SetNodeItem(std::string PropertyName, double Value)
{
    mNodeProperties[PropertyName] = Value;
}
void Node::DeclareNodeProperty(std::string PropertyName, double Value)
{
    mNodeProperties[PropertyName] = Value;
    mListOfNodeProperties.push_back(PropertyName);
}

std::vector<std::string> Node::GetListOfNodeProperties()
{
    return mListOfNodeProperties;
}