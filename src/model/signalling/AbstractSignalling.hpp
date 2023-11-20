
#ifndef ABSTRACTSIGNALLING_HPP_
#define ABSTRACTSIGNALLING_HPP_

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
 * An abstract Signalling class, for use in cell-based simulations.
 */

class AbstractSignalling;
typedef boost::shared_ptr<AbstractSignalling> SignallingPtr;
class AbstractSignalling
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
    AbstractSignalling();

    /**
     * Destructor.
     */
    virtual ~AbstractSignalling();

    /**
     * Calculates the Signalling on each node.
     *
     * As this method is pure virtual, it must be overridden
     * in subclasses.
     *
     * @param rCellPopulation reference to the cell population
     */
    virtual void UpdateSignalling(Population &rCellPopulation) = 0;
    virtual void SetupSignalling(Population &rCellPopulation, double dt) = 0;

    /**
     * Outputs Signalling used in the simulation to file and then calls OutputSignallingParameters to output all relevant
     * parameters.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    // void OutputSignallingInfo(out_stream& rParamsFile);

    /**
     * Outputs Signalling parameters to file.
     *
     * As this method is pure virtual, it must be overridden
     * in subclasses.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    // virtual void OutputSignallingParameters(out_stream& rParamsFile)=0;

    /**
     * Write any data necessary to a visualization setup file.
     * Used by AbstractCellBasedSimulation::WriteVisualizerSetupFile().
     *
     * @param pVizSetupFile a visualization setup file
     */
    // virtual void WriteDataToVisualizerSetupFile(out_stream& pVizSetupFile);
};

// TEMPLATED_CLASS_IS_ABSTRACT_2_UNSIGNED(AbstractSignalling)

#endif /*AbstractSignalling_HPP_*/
