#include "Odes.h"

#include "Constants.h"

void Odes::Local::definition(const States &x, States &dxdt, double t)
{
    /* if undefined value left alone */
    dxdt = States(Proteins::N_PROTEINS, 0);

    /* VEGF - VEGFR reaction equations */

    /* initial values */
    auto vegf = x[Proteins::Index::VEGF_TOTAL];
    auto vegfr = x[Proteins::Index::VEGFR_UNBOUND];
    auto vegfVegfrComplex = x[Proteins::Index::VEGFR_ACTIVE];

    /* forward reaction of VEGF binding receptor VEGFR */
    auto v1 = ReactionRates::k1 * vegf * vegfr;
    /* reverse reaction of VEGF-VEGFR complex dissociation */
    auto v2 = ReactionRates::k_1 * vegfVegfrComplex;
    double k_off = ReactionRates::k_1;
    double k_on = ReactionRates::k1;
    /* vegf.vegfr complex degradation */
    auto vegfrActiveDegradation = ReactionRates::vegfrActiveDegradationRate * vegfVegfrComplex;

    /* define changes */
    dxdt[Proteins::Index::VEGFR_ACTIVE] = v1 - v2 - vegfrActiveDegradation;
    dxdt[Proteins::Index::VEGFR_UNBOUND] = v2 - v1;
    dxdt[Proteins::Index::VEGF_TOTAL] = k_on * vegf * vegfr - k_off * vegfVegfrComplex;
}

void Odes::Cell::definition(const States &x, States &dxdt, double t)
{
    /* if undefined value left alone */
    dxdt = States(Proteins::N_PROTEINS, 0);

    auto vegfr = x[Proteins::Index::VEGFR_UNBOUND];

    auto vegfrDegradation = ReactionRates::vegfrDegradationRate * vegfr;

    /* VEGFR degradation and production */
    dxdt[Proteins::Index::VEGFR_UNBOUND] = ReactionRates::productionRate - vegfrDegradation;
}
