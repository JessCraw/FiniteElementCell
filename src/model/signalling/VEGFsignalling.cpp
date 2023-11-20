#include "VEGFsignalling.hpp"

VEGFsignalling::VEGFsignalling() {}

VEGFsignalling::~VEGFsignalling() {}

void VEGFsignalling::UpdateSignalling(Population &rCellPopulation)
{
    // Loop over all the cells in the population
    std::vector<CellPtr> pCells = rCellPopulation.GetCells();
    for (std::vector<CellPtr>::iterator celIt = pCells.begin(); celIt != pCells.end(); ++celIt) {
        /** Get the mesh from the cell  */
        MeshPtr pcellMesh = (*celIt)->GetMesh();

        /** Get the edges from the mesh  */
        std::vector<EdgePtr> mpEdges = pcellMesh->GetEdges();
        for (std::vector<EdgePtr>::iterator edgIt = mpEdges.begin(); edgIt != mpEdges.end(); ++edgIt) {

            /** 
             * I need to write the functions to get the background vegf
             * from fenics and and update the contained VEGF
             */
            //  mVEGF[*edgIt] = 10;


            double dV_dt = -mk_on*mVEGF[*edgIt]*mEdgeReceptors[*edgIt] + mk_off*mBoundVEGF[*edgIt];
            double dR_dt = -mk_on*mVEGF[*edgIt]*mEdgeReceptors[*edgIt] + mk_off*mBoundVEGF[*edgIt];
            double dB_dt = mk_on*mVEGF[*edgIt]*mEdgeReceptors[*edgIt] - mk_off*mBoundVEGF[*edgIt];

            mVEGF[*edgIt] = mTimeStep*dV_dt+mVEGF[*edgIt];
            mEdgeReceptors[*edgIt] = mTimeStep*dR_dt+mEdgeReceptors[*edgIt];
            mBoundVEGF[*edgIt] = mTimeStep*dB_dt+mBoundVEGF[*edgIt];
            // I think this needs to be updated to the following line. do a google 
            // integrate_adaptive(controlledStepper, definition, states, 0.0, 1.0, 0.1);
            // print_var3(mVEGF[*edgIt], mEdgeReceptors[*edgIt],mBoundVEGF[*edgIt] )
           
            (*edgIt)->SetEdgeItem("EdgeReceptors", mEdgeReceptors[*edgIt] );
            (*edgIt)->SetEdgeItem("VEGF", mVEGF[*edgIt]);
            (*edgIt)->SetEdgeItem("BoundVEGF", mBoundVEGF[*edgIt]);
            
            assert(dV_dt+dR_dt-2*dB_dt ==0);
        
        }
    }

}

void VEGFsignalling::SetupSignalling(Population &rCellPopulation, double dt) {

    mTimeStep = dt;
    // Loop over all the cells in the population
    std::vector<CellPtr> pCells = rCellPopulation.GetCells();

    for (std::vector<CellPtr>::iterator celIt = pCells.begin(); celIt != pCells.end(); ++celIt) {
        /** Get the mesh from the cell  */
        MeshPtr pcellMesh = (*celIt)->GetMesh();

        /** Get the edges from the mesh  */
        std::vector<EdgePtr> mpEdges = pcellMesh->GetEdges();

        /** Set the initial receptor and signalling levels across the cell
         * Currently this is defined on the edges, I am assuming this will change the local 
         * stiffness of the cell 
        */
        for (std::vector<EdgePtr>::iterator edgIt = mpEdges.begin(); edgIt != mpEdges.end(); ++edgIt) {
           
            /** 
             * Save the initial rest length of this spring to the map,
             * where the pointer for the spring is the key
             */
            mEdgeReceptors[*edgIt] = 1;
            mVEGF[*edgIt] = 10;
            mBoundVEGF[*edgIt] = 0;

           
            (*edgIt)->DeclareEdgeProperty("EdgeReceptors", 1);
            (*edgIt)->DeclareEdgeProperty("VEGF", 10);
            (*edgIt)->DeclareEdgeProperty("BoundVEGF", 0);
       
            /** 
             * I need to write the functions to get the background vegf
             * from fenics and and update the contained VEGF
             */

 
        }
    }
}
