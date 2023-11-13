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

            c_vector<double, 2> Spring = NodeA->GetNodeLocation() - NodeB->GetNodeLocation();

            /** Save the initial rest length of this spring to the map,
             * where the pointer for the spring is the key
             */
            double RestLength = mRestSpringLengths[*edgIt];

            double CurrentLength = norm_2(Spring);
            // print_var2(RestLength,CurrentLength);
            /** Calculate the force on the spring */
            c_vector<double, 2> Force = mSpringConstant * (CurrentLength - RestLength) * Spring / CurrentLength;
            NodeA->AddNodeForceContribution(Force * 0.5);
            NodeB->AddNodeForceContribution(-Force * 0.5);
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

            c_vector<double, 2> RestLength = NodeA->GetNodeLocation() - NodeB->GetNodeLocation();

            /** Save the initial rest length of this spring to the map,
             * where the pointer for the spring is the key
             */
            mRestSpringLengths[*edgIt] = norm_2(RestLength);
        }
    }
}

// c_vector<double, SPACE_DIM> LinearSpringForce::CalculateForceBetweenNodes(unsigned nodeAGlobalIndex,
//                                                                                     unsigned nodeBGlobalIndex,
//                                                                                     AbstractCellPopulation&
//                                                                                     rCellPopulation)
// {
//     // We should only ever calculate the force between two distinct nodes
//     assert(nodeAGlobalIndex != nodeBGlobalIndex);

//     Node<SPACE_DIM>* p_node_a = rCellPopulation.GetNode(nodeAGlobalIndex);
//     Node<SPACE_DIM>* p_node_b = rCellPopulation.GetNode(nodeBGlobalIndex);

//     // Get the node locations
//     const c_vector<double, SPACE_DIM>& r_node_a_location = p_node_a->rGetLocation();
//     const c_vector<double, SPACE_DIM>& r_node_b_location = p_node_b->rGetLocation();

//     // Get the node radii for a NodeBasedCellPopulation
//     double node_a_radius = 0.0;
//     double node_b_radius = 0.0;

//     if (bool(dynamic_cast<NodeBasedCellPopulation<SPACE_DIM>*>(&rCellPopulation)))
//     {
//         node_a_radius = p_node_a->GetRadius();
//         node_b_radius = p_node_b->GetRadius();
//     }

//     // Get the unit vector parallel to the line joining the two nodes
//     c_vector<double, SPACE_DIM> unit_difference;
//     /*
//      * We use the mesh method GetVectorFromAtoB() to compute the direction of the
//      * unit vector along the line joining the two nodes, rather than simply subtract
//      * their positions, because this method can be overloaded (e.g. to enforce a
//      * periodic boundary in Cylindrical2dMesh).
//      */
//     unit_difference = rCellPopulation.rGetMesh().GetVectorFromAtoB(r_node_a_location, r_node_b_location);

//     // Calculate the distance between the two nodes
//     double distance_between_nodes = norm_2(unit_difference);
//     assert(distance_between_nodes > 0);
//     assert(!std::isnan(distance_between_nodes));

//     unit_difference /= distance_between_nodes;

//     /*
//      * If mUseCutOffLength has been set, then there is zero force between
//      * two nodes located a distance apart greater than mMechanicsCutOffLength in AbstractTwoBodyInteractionForce.
//      */
//     if (this->mUseCutOffLength)
//     {
//         if (distance_between_nodes >= this->GetCutOffLength())
//         {
//             return zero_vector<double>(SPACE_DIM); // c_vector<double,SPACE_DIM>() is not guaranteed to be fresh
//             memory
//         }
//     }

//     /*
//      * Calculate the rest length of the spring connecting the two nodes with a default
//      * value of 1.0.
//      */
//     double rest_length_final = 1.0;

//     if (bool(dynamic_cast<MeshBasedCellPopulation*>(&rCellPopulation)))
//     {
//         rest_length_final = static_cast<MeshBasedCellPopulation*>(&rCellPopulation)->GetRestLength(nodeAGlobalIndex,
//         nodeBGlobalIndex);
//     }
//     else if (bool(dynamic_cast<NodeBasedCellPopulation<SPACE_DIM>*>(&rCellPopulation)))
//     {
//         assert(node_a_radius > 0 && node_b_radius > 0);
//         rest_length_final = node_a_radius+node_b_radius;
//     }

//     double rest_length = rest_length_final;

//     CellPtr p_cell_A = rCellPopulation.GetCellUsingLocationIndex(nodeAGlobalIndex);
//     CellPtr p_cell_B = rCellPopulation.GetCellUsingLocationIndex(nodeBGlobalIndex);

//     double ageA = p_cell_A->GetAge();
//     double ageB = p_cell_B->GetAge();

//     assert(!std::isnan(ageA));
//     assert(!std::isnan(ageB));

//     /*
//      * If the cells are both newly divided, then the rest length of the spring
//      * connecting them grows linearly with time, until 1 hour after division.
//      */
//     if (ageA < mMeinekeSpringGrowthDuration && ageB < mMeinekeSpringGrowthDuration)
//     {
//         AbstractCentreBasedCellPopulation* p_static_cast_cell_population =
//         static_cast<AbstractCentreBasedCellPopulation*>(&rCellPopulation);

//         std::pair<CellPtr,CellPtr> cell_pair = p_static_cast_cell_population->CreateCellPair(p_cell_A, p_cell_B);

//         if (p_static_cast_cell_population->IsMarkedSpring(cell_pair))
//         {
//             // Spring rest length increases from a small value to the normal rest length over 1 hour
//             double lambda = mMeinekeDivisionRestingSpringLength;
//             rest_length = lambda + (rest_length_final - lambda) * ageA/mMeinekeSpringGrowthDuration;
//         }
//         if (ageA + SimulationTime::Instance()->GetTimeStep() >= mMeinekeSpringGrowthDuration)
//         {
//             // This spring is about to go out of scope
//             p_static_cast_cell_population->UnmarkSpring(cell_pair);
//         }
//     }

//     /*
//      * For apoptosis, progressively reduce the radius of the cell
//      */
//     double a_rest_length = rest_length*0.5;
//     double b_rest_length = a_rest_length;

//     if (bool(dynamic_cast<NodeBasedCellPopulation<SPACE_DIM>*>(&rCellPopulation)))
//     {
//         assert(node_a_radius > 0 && node_b_radius > 0);
//         a_rest_length = (node_a_radius/(node_a_radius+node_b_radius))*rest_length;
//         b_rest_length = (node_b_radius/(node_a_radius+node_b_radius))*rest_length;
//     }

//     /*
//      * If either of the cells has begun apoptosis, then the length of the spring
//      * connecting them decreases linearly with time.
//      */
//     if (p_cell_A->HasApoptosisBegun())
//     {
//         double time_until_death_a = p_cell_A->GetTimeUntilDeath();
//         a_rest_length = a_rest_length * time_until_death_a / p_cell_A->GetApoptosisTime();
//     }
//     if (p_cell_B->HasApoptosisBegun())
//     {
//         double time_until_death_b = p_cell_B->GetTimeUntilDeath();
//         b_rest_length = b_rest_length * time_until_death_b / p_cell_B->GetApoptosisTime();
//     }

//     rest_length = a_rest_length + b_rest_length;
//     //assert(rest_length <= 1.0+1e-12); ///\todo #1884 Magic number: would "<= 1.0" do?

//     // Although in this class the 'spring constant' is a constant parameter, in
//     // subclasses it can depend on properties of each of the cells
//     double overlap = distance_between_nodes - rest_length;
//     bool is_closer_than_rest_length = (overlap <= 0);
//     double multiplication_factor = VariableSpringConstantMultiplicationFactor(nodeAGlobalIndex, nodeBGlobalIndex,
//     rCellPopulation, is_closer_than_rest_length); double spring_stiffness = mMeinekeSpringStiffness;

//     if (bool(dynamic_cast<MeshBasedCellPopulation*>(&rCellPopulation)))
//     {
//         return multiplication_factor * spring_stiffness * unit_difference * overlap;
//     }
//     else
//     {
//         // A reasonably stable simple force law
//         if (is_closer_than_rest_length) //overlap is negative
//         {
//             //log(x+1) is undefined for x<=-1
//             assert(overlap > -rest_length_final);
//             c_vector<double, SPACE_DIM> temp = multiplication_factor*spring_stiffness * unit_difference *
//             rest_length_final* log(1.0 + overlap/rest_length_final); return temp;
//         }
//         else
//         {
//             double alpha = 5.0;
//             c_vector<double, SPACE_DIM> temp = multiplication_factor*spring_stiffness * unit_difference * overlap *
//             exp(-alpha * overlap/rest_length_final); return temp;
//         }
//     }
// }

double LinearSpringForce::GetSpringConstant() { return mSpringConstant; }

void LinearSpringForce::SetSpringConstant(double springStiffness)
{
    assert(springStiffness > 0.0);
    mSpringConstant = springStiffness;
}
