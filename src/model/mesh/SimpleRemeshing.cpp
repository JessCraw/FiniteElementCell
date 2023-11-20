#include "SimpleRemeshing.hpp"

SimpleRemeshing::SimpleRemeshing()
{
    
}

void SimpleRemeshing::RemeshRandomEdges(Population &rCellPopulation)
{
    // Randomly select edges, then add a node to the middel, create two new edges and remove the current edge
    // upxdate what the noides are connected to 

    std::vector<CellPtr> pCells = rCellPopulation.GetCells();
    for (std::vector<CellPtr>::iterator celIt = pCells.begin(); celIt != pCells.end(); ++celIt) {
         /** Get the mesh from the cell  */
        MeshPtr pcellMesh = (*celIt)->GetMesh();

        // randomly select edges
        std::vector<EdgePtr> mpEdges = pcellMesh->GetEdges();

        double InitialNumberOfEdges = mpEdges.size(); 


        std::vector<EdgePtr> sampledEdges(7);

        // Seed the random number generator
        std::random_device rd;
        std::mt19937 rng(rd());

        // Use std::sample with a random number generator
        std::sample(mpEdges.begin(), mpEdges.end(), sampledEdges.begin(), sampledEdges.size(), rng);

        // Now loop over these edges and remesh them
        for (std::vector<EdgePtr>::iterator edgIt = sampledEdges.begin(); edgIt != sampledEdges.end(); ++edgIt) 
        {
            /** Get the nodes from this edge */
            NodePtr NodeA = (*edgIt)->GetNodeA();
            NodePtr NodeB = (*edgIt)->GetNodeB();

            c_vector<double, 2> NewNodeLocation = (NodeB->GetNodeLocation()+NodeA->GetNodeLocation())/2;

            /** Create new node */
            std::vector<NodePtr> pNodes = pcellMesh->GetNodes();
            int i = pNodes.size();
            MAKE_PTR_ARGS(Node, pNewNode, (NewNodeLocation, i));
            pcellMesh->AddNode(pNewNode);

            /** Create new edge */
            MAKE_PTR_ARGS(Edge, pNewEdge1, (std::make_pair(NodeA, pNewNode)));
            MAKE_PTR_ARGS(Edge, pNewEdge2, (std::make_pair(pNewNode, NodeB)));
            pcellMesh->AddEdge(pNewEdge1);
            pcellMesh->AddEdge(pNewEdge2);

            // Need to ensure the node knows their edges
            pNewNode->SetEdge(pNewEdge1);
            pNewNode->SetEdge(pNewEdge2);

            NodeA->SetEdge(pNewEdge2);
            NodeB->SetEdge(pNewEdge1);

            // Now need to make sure the nodes each know their neighbours. 
            std::vector<NodePtr> NeighboursVec1 = {NodeB,pNewNode};
            NodeB->SetNeighbours(NeighboursVec1);

            std::vector<NodePtr> NeighboursVec2 = {NodeA,pNewNode };
            NodeB->SetNeighbours(NeighboursVec2);

            

            pcellMesh->RemoveEdge(*edgIt);
            std::vector<EdgePtr> pEdgesNew = {pNewEdge1, pNewEdge2};
            // Update forces for the removed and added edge:
            /** Loop over all the forces and save them in the nodes */
   
            // for (typename std::vector<ForcePtr>::iterator iter = mpForceCollection.begin(); iter != mpForceCollection.end();
            //     ++iter) 
            // {
            //     (*iter)->UpdateInitialEdgeConfiguration(*edgIt, pEdgesNew);
            // }
            // Need to update anything calling this edge -- and create new ones 

            /** Loop over all the forces and save them in the nodes */
            // foNodeProperties


            // I want to go in and check everything is correct 
            // Check all the metrics of the three node

            // print_var2(pNewNode->GetNodeLocation()[0],pNewNode->GetNodeLocation()[0]);
            // print_var2(NodeA->GetNodeLocation()[0],NodeA->GetNodeLocation()[0]);
            // print_var2(NodeB->GetNodeLocation()[0],NodeB->GetNodeLocation()[0]);
        }
        // Check all the nodes and edges are the right number

        double NewEdgeCount =(pcellMesh->GetEdges()).size();
        assert((mpEdges.size() == (InitialNumberOfEdges + 7)));
        // TS_ASSERT(1==0);
        print_var2(InitialNumberOfEdges,NewEdgeCount);
    }




}




void SimpleRemeshing::RemeshAllEdges(Population &rCellPopulation)
{
    // Randomly select edges, then add a node to the middel, create two new edges and remove the current edge
    // upxdate what the noides are connected to 

    std::vector<CellPtr> pCells = rCellPopulation.GetCells();
    for (std::vector<CellPtr>::iterator celIt = pCells.begin(); celIt != pCells.end(); ++celIt) {
         /** Get the mesh from the cell  */
        MeshPtr pcellMesh = (*celIt)->GetMesh();

        // randomly select edges
        std::vector<EdgePtr> mpEdges = pcellMesh->GetEdges();
        // std::vector<EdgePtr> sampledEdges(1);

        // Seed the random number generator
        // std::random_device rd;
        // std::mt19937 rng(rd());

        // Use std::sample with a random number generator
        // std::sample(mpEdges.begin(), mpEdges.end(), sampledEdges.begin(), sampledEdges.size(), rng);

        // Now loop over these edges and remesh them
        for (std::vector<EdgePtr>::iterator edgIt = mpEdges.begin(); edgIt != mpEdges.end(); ++edgIt) 
        {
            /** Get the nodes from this edge */
            NodePtr NodeA = (*edgIt)->GetNodeA();
            NodePtr NodeB = (*edgIt)->GetNodeB();

            c_vector<double, 2> NewNodeLocation = (NodeB->GetNodeLocation()+NodeA->GetNodeLocation())/2;

            /** Create new node */
            std::vector<NodePtr> pNodes = pcellMesh->GetNodes();
            int i = pNodes.size();
            MAKE_PTR_ARGS(Node, pNewNode, (NewNodeLocation, i));
            pcellMesh->AddNode(pNewNode);

            /** Create new edge */
            MAKE_PTR_ARGS(Edge, pNewEdge1, (std::make_pair(NodeA, pNewNode)));
            MAKE_PTR_ARGS(Edge, pNewEdge2, (std::make_pair(pNewNode, NodeB)));
            pcellMesh->AddEdge(pNewEdge1);
            pcellMesh->AddEdge(pNewEdge2);

            

            pcellMesh->RemoveEdge(*edgIt);
            std::vector<EdgePtr> pEdgesNew = {pNewEdge1, pNewEdge2};
            // Update forces for the removed and added edge:
            /** Loop over all the forces and save them in the nodes */
   
            // for (typename std::vector<ForcePtr>::iterator iter = mpForceCollection.begin(); iter != mpForceCollection.end();
            //     ++iter) 
            // {
            //     (*iter)->UpdateInitialEdgeConfiguration(*edgIt, pEdgesNew);
            // }
            // Need to update anything calling this edge -- and create new ones 

            /** Loop over all the forces and save them in the nodes */
            // foNodeProperties



        }
    }



}

void SimpleRemeshing::CallRemeshing(Population &rCellPopulation)
{
    RemeshRandomEdges(rCellPopulation);
    // RemeshAllEdges(rCellPopulation);
}   



void SimpleRemeshing::AddForceCollection(std::vector<ForcePtr> pForceCollection)
{
    
    mpForceCollection=pForceCollection;
}

