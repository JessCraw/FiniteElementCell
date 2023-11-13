#ifndef CELLPROTEINS_H
#define CELLPROTEINS_H

#include "Constants.h"
#include "Ligand.h"
#include "Receptor.h"

#include <array>
#include <utility>

/**
 * Container to hold Cell + Spring proteins.
 */
struct CellProteins
{
    CellProteins() = default;

    CellProteins(const std::array<Receptor, Proteins::MembraneReceptorsArray.size()> &receptors,
                 const std::array<Ligand, Proteins::MembraneLigandsArray.size()> &ligands)
        : receptors(receptors),
          ligands(ligands)
    {
    }

    Receptor &operator[](Proteins::MembraneReceptors receptor)
    {
        return const_cast<Receptor &>(std::as_const(*this)[receptor]);
    };

    const Receptor &operator[](Proteins::MembraneReceptors receptor) const
    {
        return receptors[static_cast<unsigned long>(receptor)];
    };

    Ligand &operator[](Proteins::MembraneLigands ligand) { return const_cast<Ligand &>(std::as_const(*this)[ligand]); };

    const Ligand &operator[](Proteins::MembraneLigands ligand) const
    {
        return ligands[static_cast<unsigned long>(ligand)];
    };

    /*
     * Return a split of current proteins. Does not update current protein values.
     *
     * @return    Split of current protein values.
     */
    CellProteins getSplit()
    {
        std::array<Receptor, Proteins::MembraneReceptorsArray.size()> receptorSplit {Receptor()};
        std::array<Ligand, Proteins::MembraneLigandsArray.size()> ligandSplit {Ligand()};

        for (unsigned long i = 0; i < Proteins::MembraneReceptorsArray.size(); ++i)
            receptorSplit[i] = Receptor(receptors[i].getTotal() / 2, receptors[i].getActive() / 2);

        for (unsigned long i = 0; i < Proteins::MembraneLigandsArray.size(); ++i)
            ligandSplit[i] = Ligand(ligands[i].getTotal() / 2);

        return {receptorSplit, ligandSplit};
    }

    double getReceptorActivation(Proteins::MembraneReceptors receptor) const
    {
        auto protein = (*this)[receptor];
        return protein.getActive() / protein.getTotal();
    }

    /*
     * Return a split of current proteins and update proteins with values.
     *
     * @return    Split of current protein values.
     */
    CellProteins split() { return (*this = getSplit()); }

private:
    std::array<Receptor, Proteins::MembraneReceptorsArray.size()> receptors {Receptor()};
    std::array<Ligand, Proteins::MembraneLigandsArray.size()> ligands {Ligand()};
};

#endif  // CELLPROTEINS_H
