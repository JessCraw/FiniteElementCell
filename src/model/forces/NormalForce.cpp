#include "NormalForce.hpp"

NormalForce::NormalForce() {}

NormalForce::~NormalForce() {}

void NormalForce::AddForceContribution(Population &rCellPopulation)
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
            std::vector<EdgePtr> Edges = (*noIt)->GetEdges();
            c_vector<double, 2> Normal = Create_c_vector(0.0, 0.0);
            for (std::vector<EdgePtr>::iterator edgIt = Edges.begin(); edgIt != Edges.end(); ++edgIt) {
                /** Get normal from the edge */
                Normal += (*edgIt)->GetEdgeNormal();
            }
            //    print_var2(Normal[0],Normal[1]);
            (*noIt)->AddNodeForceContribution(mPressure * Normal);
        }
    }
}

void NormalForce::SetupForce(Population &rCellPopulation) {}

void NormalForce::SetPressure(double Pressure)
{
    assert(Pressure > 0.0);
    mPressure = Pressure;
}


void NormalForce::UpdateInitialEdgeConfiguration(EdgePtr pEdgeNode,std::vector<EdgePtr> pEdgesNew)
{
    assert(pEdgesNew.size()==2);
}
