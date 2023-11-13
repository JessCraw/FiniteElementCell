#ifndef LIGAND_H
#define LIGAND_H

#include <algorithm>

struct Ligand
{
    explicit Ligand(double total = 0)
        : total(total)
    {
    }

    void setTotal(double n) { total = std::max(n, static_cast<double>(0)); }

    double getTotal() const { return total; }

    double increaseTotal(double n) { return total += n; }

    double decreaseTotal(double n)
    {
        setTotal(total - n);
        return total;
    }

    double reduceTotal(double n)
    {
        auto oldTotal = total;
        total = std::max(total - n, static_cast<double>(0));
        return oldTotal - total;
    }

    void increaseConc(double val, double vol) { total += val * vol; }

    void setConc(double val, double vol) { total = val * vol; }

    double getConc(double vol) const { return total / vol; }

private:
    double total;
};

#endif  // LIGAND_H
