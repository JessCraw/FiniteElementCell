#include "CircleMeshGenerator.hpp"

#include "LinearSpringForce.hpp"

CircleMeshGenerator::CircleMeshGenerator(int nNodes, double Radius, double centerX, double centerY)
{
    // Fill the nodes vector,
    mNodesVector.reserve(nNodes);
    double theta = 2 * M_PI / nNodes;
    for (int i = 0; i < nNodes; i++) {
        c_vector<double, 2> Location =
            Create_c_vector(Radius * cos(i * theta) + centerX, Radius * sin(i * theta) + centerY);

        MAKE_PTR_ARGS(Node, NodeIter, (Location, i));
        mpNodes.push_back(NodeIter);
        mNodesVector.push_back(Create_c_vector(Radius * cos(i * theta) + centerX, Radius * sin(i * theta) + centerY));
    }

    // Define all the Edges
    int nEdges = nNodes;
    for (int i = 0; i < nEdges - 1; i++) {
        // Make new Edge
        MAKE_PTR_ARGS(Edge, EdgeIter, (std::make_pair(mpNodes[i], mpNodes[i + 1])));
        mpEdges.push_back(EdgeIter);

        mEdgesVector.push_back(std::make_pair(i, i + 1));
        mpNodes[i]->SetEdge(EdgeIter);
        mpNodes[i + 1]->SetEdge(EdgeIter);
    }
    mEdgesVector.push_back(std::make_pair(nEdges - 1, 0));
    // End point
    MAKE_PTR_ARGS(Edge, EdgeIter, (std::make_pair(mpNodes[nEdges - 1], mpNodes[0])));
    mpEdges.push_back(EdgeIter);
    mpNodes[0]->SetEdge(EdgeIter);
    mpNodes[nEdges - 1]->SetEdge(EdgeIter);

    // Define all the neighbours
    for (int i = 1; i < nEdges - 1; i++) {
        mpNodes[i]->SetNeighbours({mpNodes[i - 1], mpNodes[i + 1]});
        mNeighboursVector.push_back({i - 1, i + 1});
    }
    // The is for node 0
    mNeighboursVector.push_back({nEdges - 1, 1});
    mpNodes[0]->SetNeighbours({mpNodes[nEdges - 1], mpNodes[1]});
    // The is for node nEdges - 1
    mNeighboursVector.push_back({nEdges - 2, 0});
    mpNodes[nEdges - 1]->SetNeighbours({mpNodes[nEdges - 2], mpNodes[0]});

    // mpMesh = boost::make_shared<MutableMesh>();
    // mpMesh->ConstructFromMesh(mesh_reader);
}
