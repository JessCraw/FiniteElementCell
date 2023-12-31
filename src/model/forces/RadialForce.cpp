#include "RadialForce.hpp"

RadialForce::RadialForce() {}

RadialForce::~RadialForce() {}

void RadialForce::AddForceContribution(Population &rCellPopulation)
{
    // Loop over all the cells in the population
    std::vector<CellPtr> pCells = rCellPopulation.GetCells();
    for (std::vector<CellPtr>::iterator celIt = pCells.begin(); celIt != pCells.end(); ++celIt) {
        /** Get the mesh from the cell  */
        MeshPtr pcellMesh = (*celIt)->GetMesh();

        /** Get the nodes from the mesh  */
        std::vector<NodePtr> pNodes = pcellMesh->GetNodes();

        for (std::vector<NodePtr>::iterator noIt = pNodes.begin(); noIt != pNodes.end(); ++noIt) {
            /** Get the normal for the node, will be the direction of force
             * this will be the summation of the normal from the edges
             * Might be an issue when we have filopodia develope
             */
            c_vector<double, 2> Normal = (*noIt)->GetNodeLocation();
            Normal/=norm_2(Normal);

         
            /** Get the normal for the node, will be the direction of force
             * this will be the summation of the normal from the edges
             * Might be an issue when we have filopodia develope
             */
            double Length =0;
            double counter =1;
            std::vector<EdgePtr> Edges = (*noIt)->GetEdges();
            assert(Edges.size==2);
            for (std::vector<EdgePtr>::iterator edgIt = Edges.begin(); edgIt != Edges.end(); ++edgIt) 
            {
                Length += (*edgIt)->GetEdgeLength()/2;
         
            }
            (*noIt)->AddNodeForceContribution(mPressure * Normal);
        }
    }
}

void RadialForce::SetupForce(Population &rCellPopulation) {}

void RadialForce::SetPressure(double Pressure)
{
    assert(Pressure > 0.0);
    mPressure = Pressure;
}


void RadialForce::UpdateInitialEdgeConfiguration(EdgePtr pEdgeNode,std::vector<EdgePtr> pEdgesNew)
{
    assert(pEdgesNew.size()==2);
}
