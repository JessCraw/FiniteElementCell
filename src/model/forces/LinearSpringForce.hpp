
#ifndef LINEARSPRINGFORCE_HPP_
#define LINEARSPRINGFORCE_HPP_

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

class LinearSpringForce : public AbstractForce
{
protected:
    /**
     * Spring stiffness.
     */
    double mSpringConstant = 0.001;

    /**
     * Initial resting spring length after cell division.
     * Has units of cell size at equilibrium rest length
     *
     * The value of this parameter should be larger than mDivisionSeparation,
     * because of pressure from neighbouring springs.
     */
    double mRestSpringLength;

public:
    /**
     * Constructor.
     */
    LinearSpringForce();

    /**
     * Destructor.
     */
    virtual ~LinearSpringForce();

    /** Member variables */
    void AddForceContribution(Population &rCellPopulation);
    void SetupForce(Population &rCellPopulation);
    double GetSpringConstant(); /** return mSpringConstant **/
    /**
     * Set mSpringConstant.
     */
    void SetSpringConstant(double springStiffness);

    /** Member maps */
    std::map<EdgePtr, double> mRestSpringLengths;
};

#endif /*LINEARSPRINGFORCE_HPP_*/
