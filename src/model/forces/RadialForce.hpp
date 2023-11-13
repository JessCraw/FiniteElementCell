
#ifndef RADIALFORCE_HPP_
#define RADIALFORCE_HPP_

#include "AbstractForce.hpp"
#include "Edges/Edge.hpp"
#include "Nodes/Node.hpp"
#include "Population.hpp"

#include <algorithm>
#include <boost/serialization/base_object.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

class RadialForce : public AbstractForce
{
protected:
    /**
     * Spring stiffness.
     */
    double mPressure = 0.001;

    /**
     * Initial resting spring length after cell division.
     * Has units of cell size at equilibrium rest length
     *
     * The value of this parameter should be larger than mDivisionSeparation,
     * because of pressure from neighbouring springs.
     */

public:
    /**
     * Constructor.
     */
    RadialForce();

    /**
     * Destructor.
     */
    virtual ~RadialForce();

    /** Member variables */
    void AddForceContribution(Population &rCellPopulation);
    void SetupForce(Population &rCellPopulation);
    /**
     * Set mSpringConstant.
     */
    void SetPressure(double Pressure);
};

#endif /*RadialForce_HPP_*/
