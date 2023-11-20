
#ifndef ABSTRACTNODEPROPERTY_HPP_
#define ABSTRACTNODEPROPERTY_HPP_

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

#include "MathsFunctions.hpp"

#include "Edges/Edge.hpp"
#include "Nodes/Node.hpp"


/**
 * An abstract NodeProperties class, for use in cell-based simulations.
 */
class Node;
typedef boost::shared_ptr<Node> NodePtr;

class AbstractNodeProperty;
typedef boost::shared_ptr<AbstractNodeProperty> NodePropPtr;
class AbstractNodeProperty
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
    AbstractNodeProperty();

    /**
     * Destructor.
     */
    virtual ~AbstractNodeProperty();


    /** Member variables */
        NodePtr mpNode;
        std::string mPropertyName;
        
    /** Member functions */
        void SetLabel(std::string PropertySetLabel);
};

// TEMPLATED_CLASS_IS_ABSTRACT_2_UNSIGNED(AbstractNodeProperty)

#endif /*AbstractNodeProperty_HPP_*/
