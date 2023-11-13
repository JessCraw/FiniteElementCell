#ifndef ODES_H
#define ODES_H

#include <vector>

/* equations and parameters adapted from Venkatraman et al., 2016 (https://doi.org/10.1371/journal.pone.0166489) */

namespace Odes
{
    using States = std::vector<double>;
    using OdeDefinition = void (*)(const States &x, States &dxdt, double t);

    namespace ReactionRates
    {
        /* cu.sec^-1 used in forward reaction of ligand VEGF binding receptor VEGFR2 */
        static constexpr double k1 = 0.1;
        /* cu.sec^-1 used in reverse reaction of VEGF-VEGFR2 complex dissociation */
        static constexpr double k_1 = 0.001;
        /* sec^-1 */
        static constexpr double vegfrDegradationRate = 0.001;
        static constexpr double vegfrActiveDegradationRate = 0.001;
        /* sec^-1 */
        static constexpr double productionRate = 0.005;

    }  // namespace ReactionRates

    namespace Local
    {
        void definition(const States &x, States &dxdt, double t);
    };  // namespace Local

    namespace Cell
    {
        void definition(const States &x, States &dxdt, double t);
    };
};  // namespace Odes

#endif  // ODES_H
