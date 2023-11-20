
#ifndef ABSTRACTFORCE_HPP_
#define ABSTRACTFORCE_HPP_

#include "Population.hpp"

#include <algorithm>
#include <boost/enable_shared_from_this.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <cmath>
#include <cstddef>
#include <map>
#include <numeric>
#include <utility>

// #include "Node.hpp"
// #include "Edge/Edge.hpp"
/**
 * An abstract force class, for use in cell-based simulations.
 */

// class Node;
// typedef boost::shared_ptr<Node> NodePtr;
// class Edge;
// typedef boost::shared_ptr<Edge> EdgePtr;

class AbstractForce;
typedef boost::shared_ptr<AbstractForce> ForcePtr;
class AbstractForce
{
    /** Needed for serialization. */
    friend class boost::serialization::access;
    /**
     * Serialize the object.
     *
     * @param archive the archive
     * @param version the current version of this class
     */
    template <class Archive>
    void serialize(Archive &archive, const unsigned int version)
    {
    }

public:
    /**
     * Default constructor.
     */
    AbstractForce();

    /**
     * Destructor.
     */
    virtual ~AbstractForce();

    /**
     * Calculates the force on each node.
     *
     * As this method is pure virtual, it must be overridden
     * in subclasses.
     *
     * @param rCellPopulation reference to the cell population
     */
    virtual void AddForceContribution(Population &rCellPopulation) = 0;
    virtual void SetupForce(Population &rCellPopulation) = 0;

    // Making mesh courser
    void UpdateInitialNodeConfiguration(std::vector<NodePtr> pNodesOld, NodePtr pNodeNew);
    void UpdateInitialEdgeConfiguration(std::vector<EdgePtr> pEdgesOld, EdgePtr pEdgeNode);

    // Making mesh finer
    void UpdateInitialNodeConfiguration(NodePtr pNodeOld,std::vector<NodePtr> pNodesNew);
    virtual void UpdateInitialEdgeConfiguration(EdgePtr pEdgeNode,std::vector<EdgePtr> pEdgesNew)= 0;

};


#endif /*ABSTRACTFORCE_HPP_*/
