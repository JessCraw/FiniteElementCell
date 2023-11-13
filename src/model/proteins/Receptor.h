#ifndef RECEPTOR_H
#define RECEPTOR_H

#include <algorithm>

struct Receptor
{
    Receptor(double total = 0, double active = 0)
        : total(total),
          active(active)
    {
    }

    void setTotal(double n)
    {
        total = std::max(0., n);
        active = std::min(active, total);
    }

    double increaseTotal(double n)
    {
        setTotal(getTotal() + n);
        return total;
    }

    double getUnbound() const { return total - active; }

    void setActive(double n) { active = std::max(std::min(n, total), static_cast<double>(0)); }

    /**
     * Attempt to increase active value by n. This can be capped by total value.
     *
     * @return Amount active increased by.
     */
    double increaseActive(double n)
    {
        auto original = getActive();
        setActive(original + n);
        return getActive() - original;
    }

    /**
     * Attempt to decrease active value by n. Capped at 0.
     *
     * @return Amount active decreased by.
     */
    double decreaseActive(double n)
    {
        auto original = getActive();
        setActive(original - n);
        return original - getActive();
    }

    double getTotal() const { return total; }
    double getActive() const { return active; }

    double getActiveConc(double length) const { return getActive() / length; }
    double getUnboundConc(double length) const { return getUnbound() / length; }
    double getTotalConc(double length) const { return getTotal() / length; }

private:
    double total;
    double active;
};

#endif  // RECEPTOR_H
