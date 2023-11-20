
#ifndef VEGFSIGNALLING_HPP_
#define VEGFSIGNALLING_HPP_

#include "AbstractSignalling.hpp"
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
#include <boost/numeric/odeint.hpp>

class VEGFsignalling : public AbstractSignalling
{
protected:
    /**
     * Set up initial conditions
     */

    double mk_on = 1;
    double mk_off = 1;

    std::map<EdgePtr, double> mEdgeReceptors;
    std::map<EdgePtr, double> mVEGF;
    std::map<EdgePtr, double> mBoundVEGF;

    double mTimeStep;



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
    VEGFsignalling();

    /**
     * Destructor.
     */
    virtual ~VEGFsignalling();

    /** Member variables */
    void UpdateSignalling(Population &rCellPopulation);
    void SetupSignalling(Population &rCellPopulation, double dt);
    /**
     * Set mSpringConstant.
     */
};

#endif /*VEGFsignalling_HPP_*/
