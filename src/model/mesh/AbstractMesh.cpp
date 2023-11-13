#include "AbstractMesh.hpp"

AbstractMesh::AbstractMesh() {}

std::vector<c_vector<double, 2>> AbstractMesh::GetNodesVector() { return mNodesVector; }

std::vector<std::pair<int, int>> AbstractMesh::GetEdgeVector() { return mEdgesVector; }

std::vector<std::vector<int>> AbstractMesh::GetNeighbours() { return mNeighboursVector; }

std::string AbstractMesh::GetMeshType() { return mMeshType; }

std::vector<EdgePtr> AbstractMesh::GetEdges() { return mpEdges; }
std::vector<NodePtr> AbstractMesh::GetNodes() { return mpNodes; }
