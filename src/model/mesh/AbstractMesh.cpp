#include "AbstractMesh.hpp"

AbstractMesh::AbstractMesh() {}

std::vector<c_vector<double, 2>> AbstractMesh::GetNodesVector() { return mNodesVector; }

std::vector<std::pair<int, int>> AbstractMesh::GetEdgeVector() { return mEdgesVector; }

std::vector<std::vector<int>> AbstractMesh::GetNeighbours() { return mNeighboursVector; }

std::string AbstractMesh::GetMeshType() { return mMeshType; }

std::vector<EdgePtr> AbstractMesh::GetEdges() { return mpEdges; }
std::vector<NodePtr> AbstractMesh::GetNodes() { return mpNodes; }


NodePtr AbstractMesh::AddNode(c_vector<double, 2> Location)
{
    int i = mpNodes.size();
    MAKE_PTR_ARGS(Node, pNewNode, (Location, i));
    mpNodes.push_back(pNewNode);
    return pNewNode;
}
EdgePtr AbstractMesh::AddEdge(NodePtr Node1, NodePtr Node2)
{
    MAKE_PTR_ARGS(Edge, EdgeIter, (std::make_pair(Node1, Node2)));
    mpEdges.push_back(EdgeIter);
    return EdgeIter;
}

void AbstractMesh::AddNode(NodePtr pNewNode)
{
    // int i = mpNodes.size();
    // MAKE_PTR_ARGS(Node, pNewNode, (Location, i));
    mpNodes.push_back(pNewNode);
}
void AbstractMesh::AddEdge(EdgePtr pEdge)
{
    // MAKE_PTR_ARGS(Edge, EdgeIter, (std::make_pair(Node1, Node2)));
    mpEdges.push_back(pEdge);
}


void AbstractMesh::RemoveEdge(EdgePtr pEdge)
{
    // Find the iterator pointing to the element to be removed
    auto it = std::find(mpEdges.begin(), mpEdges.end(), pEdge);

    // Check if the element was found before erasing
    if (it != mpEdges.end()) {
        // Erase the element from the vector
        mpEdges.erase(it);
       } else {
        std::cout << "Element not found in the vector. -- this is bad" << std::endl;
    }

     // Remove the information of the connected node from each node
    // pEdge->GetNodeA()->RemoveNeighbour(pEdge->GetNodeB());
    // pEdge->GetNodeB()->RemoveNeighbour(pEdge->GetNodeA());

    // I also want to remove this edge from the two nodes it holds 
    pEdge->GetNodeA()->DeleteEdge(pEdge);
    pEdge->GetNodeB()->DeleteEdge(pEdge);

   
    // delete(pEdge);
}

