#include "Solver.h"

#include "Constants.h"

using namespace Odes;

States Solver::solve(const std::array<double, Proteins::N_PROTEINS> &concentrations, OdeDefinition definition)
{
    ControlledStepper controlledStepper;
    States states(Proteins::N_PROTEINS, 0);
    for (int i = 0; i < Proteins::N_PROTEINS; ++i)
        states[i] = concentrations[i];
    integrate_adaptive(controlledStepper, definition, states, 0.0, 1.0, 0.1);
    return states;
}
