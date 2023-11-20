#include "LinearSpringForce.hpp"

LinearSpringForce::LinearSpringForce() {}

LinearSpringForce::~LinearSpringForce() {}

void LinearSpringForce::AddForceContribution(Population &rCellPopulation)
{
    // Loop over all the cells in the population
    std::vector<CellPtr> pCells = rCellPopulation.GetCells();
    for (std::vector<CellPtr>::iterator celIt = pCells.begin(); celIt != pCells.end(); ++celIt) {
        /** Get the mesh from the cell  */
        MeshPtr pcellMesh = (*celIt)->GetMesh();

        /** Get the edges from the mesh  */
        std::vector<EdgePtr> mpEdges = pcellMesh->GetEdges();

        for (std::vector<EdgePtr>::iterator edgIt = mpEdges.begin(); edgIt != mpEdges.end(); ++edgIt) {
            /** Get the nodes from this edge */
            NodePtr NodeA = (*edgIt)->GetNodeA();
            NodePtr NodeB = (*edgIt)->GetNodeB();

            c_vector<double, 2> Spring = NodeB->GetNodeLocation()-NodeA->GetNodeLocation() ;

            /** Save the initial rest length of this spring to the map,
             * where the pointer for the spring is the key
             */
            double RestLength = mRestSpringLengths[*edgIt];

            double CurrentLength = norm_2(Spring);
            // print_var2(RestLength,CurrentLength);
            /** Calculate the force on the spring */
            c_vector<double, 2> Force = mSpringConstant * (CurrentLength - RestLength) * Spring / CurrentLength;
            double EdgeLength=0;
            EdgeLength += (*edgIt)->GetEdgeLength()/2;
            
            NodeA->AddNodeForceContribution(Force * 0.5/EdgeLength);
            NodeB->AddNodeForceContribution(-Force * 0.5/EdgeLength);
        }
    }
}

void LinearSpringForce::SetupForce(Population &rCellPopulation)
{
    // Loop over all the cells in the population
    std::vector<CellPtr> pCells = rCellPopulation.GetCells();

    for (std::vector<CellPtr>::iterator celIt = pCells.begin(); celIt != pCells.end(); ++celIt) {
        /** Get the mesh from the cell  */
        MeshPtr pcellMesh = (*celIt)->GetMesh();

        /** Get the edges from the mesh  */
        std::vector<EdgePtr> mpEdges = pcellMesh->GetEdges();
          
        for (std::vector<EdgePtr>::iterator edgIt = mpEdges.begin(); edgIt != mpEdges.end(); ++edgIt) {
            /** Get the nodes from this edge */
            NodePtr NodeA = (*edgIt)->GetNodeA();
            NodePtr NodeB = (*edgIt)->GetNodeB();
            
            // c_vector<double, 2> Normal = Create_c_vector(0.0, 0.0);
             /** Get normal from the edge */

            c_vector<double, 2> RestLength = NodeA->GetNodeLocation() - NodeB->GetNodeLocation();

            /** Save the initial rest length of this spring to the map,
             * where the pointer for the spring is the key
             */
            mRestSpringLengths[*edgIt] = norm_2(RestLength)/2;
            // (*edgIt)->DeclareEdgeProperty("RestLength", norm_2(RestLength));
        }
    }
}

double LinearSpringForce::GetSpringConstant() { return mSpringConstant; }

void LinearSpringForce::SetSpringConstant(double springStiffness)
{
    assert(springStiffness > 0.0);
    mSpringConstant = springStiffness;
}


// Making mesh finer
// void UpdateInitialNodeConfiguration(NodePtr pNodeOld,std::vector<NodePtr> pNodesNew);
void LinearSpringForce::UpdateInitialEdgeConfiguration(EdgePtr pEdgeNode,std::vector<EdgePtr> pEdgesNew)
{
    assert(pEdgesNew.size()==2);
    double OldRestLength  = mRestSpringLengths[pEdgeNode];
    mRestSpringLengths[pEdgesNew[0]] = OldRestLength/2;
    mRestSpringLengths[pEdgesNew[1]] = OldRestLength/2;
}
