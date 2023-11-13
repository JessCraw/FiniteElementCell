
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

/**
 * An abstract force class, for use in cell-based simulations.
 */

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

    /**
     * Outputs force used in the simulation to file and then calls OutputForceParameters to output all relevant
     * parameters.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    // void OutputForceInfo(out_stream& rParamsFile);

    /**
     * Outputs force parameters to file.
     *
     * As this method is pure virtual, it must be overridden
     * in subclasses.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    // virtual void OutputForceParameters(out_stream& rParamsFile)=0;

    /**
     * Write any data necessary to a visualization setup file.
     * Used by AbstractCellBasedSimulation::WriteVisualizerSetupFile().
     *
     * @param pVizSetupFile a visualization setup file
     */
    // virtual void WriteDataToVisualizerSetupFile(out_stream& pVizSetupFile);
};

// TEMPLATED_CLASS_IS_ABSTRACT_2_UNSIGNED(AbstractForce)

#endif /*ABSTRACTFORCE_HPP_*/
