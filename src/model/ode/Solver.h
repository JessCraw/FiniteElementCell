#ifndef SOLVER_H
#define SOLVER_H

#include "Constants.h"
#include "Odes.h"

#include <boost/numeric/odeint.hpp>

using ProteinConcentrations = std::array<double, Proteins::N_PROTEINS>;

namespace Odes
{
    namespace Solver
    {
        using ErrorStepper = boost::numeric::odeint::runge_kutta_cash_karp54<States>;
        using ControlledStepper = boost::numeric::odeint::controlled_runge_kutta<ErrorStepper>;

        States solve(const std::array<double, Proteins::N_PROTEINS> &concentrations, OdeDefinition definition);
    }  // namespace Solver
}  // namespace Odes
#endif  // SOLVER_H
