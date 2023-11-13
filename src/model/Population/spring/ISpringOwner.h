#ifndef ISPRINGOWNER_H
#define ISPRINGOWNER_H

#include "Constants.h"

#include <list>
#include <memory>

class Spring;
class NodeOLD;
struct CellProteins;
class Grid;

/**
 * Interface for Spring Ownership.
 *
 * Abstract class representing Spring Ownership.
 * Exposes an interface for Spring Ownership related methods.
 * Spring class relies on pointer to ISpringOwner to contact its parent.
 */
class ISpringOwner
{
public:
    /**
     * Update each child spring's given receptor total with an even split of given total value. If any of child springs
     * are remeshed then this function will be called recursively for that spring and its child springs.
     *
     * This version of the function does not attempt to conserve any active receptors which may be lost due to totals
     * being updated and therefore is slightly more efficient.
     *
     * This can be used when total split between total springs is being increased since this
     * does not risk any active receptor values being capped.
     *
     * @param receptor    Receptor to update total of
     * @param n           Total to divide between child springs.
     */
    void updateSpringsReceptorTotal(Proteins::MembraneReceptors receptor, double n);

    /**
     * Same as updateChildSpringReceptorTotals except keeps track of whether any active receptors are lost when setting
     * the new totals. If any are lost then redistributes this amongst other child springs by calling
     * distributeLostActiveReceptor.
     *
     * @param receptor    Receptor to update total of
     * @param n           Total to divide between child springs.
     */
    void updateSpringsProteinTotalConserveActiveReceptors(Proteins::MembraneReceptors receptor, double n);

    /**
     * Distributes given receptor active amount between child springs. Loops through each spring in vector allocating as
     * high proportion of the given amount n that is still unallocated that the child spring can hold. This is the
     * most simple method but means there will likely be an uneven distribution of increase.
     *
     * Intended to be used to redistribute lost active receptors that can occur from updating totals or when a spring
     * deletes.
     *
     * @param receptor    Receptor to distribute active amount of.
     * @param n           Active amount to redistribute to child springs.
     */
    void distributeLostActiveReceptor(Proteins::MembraneReceptors receptor, double n);

    /*
     * Increase child springs given ligand's total by even split of given increase n.
     *
     * @param ligand    Ligand to increase total of
     * @param n         Increase to divide between child springs.
     */
    void increaseSpringsLigandTotal(Proteins::MembraneLigands ligand, double n);

    /*
     * Decrease child springs given ligand's total by even split of given decrease n.
     *
     * @param ligand    Ligand to decrease total of
     * @param n         Decrease to divide between child springs.
     */
    void decreaseSpringsLigandTotal(Proteins::MembraneLigands ligand, double n);

    virtual ~ISpringOwner() = default;

    std::list<Spring> springs;

    void checkChildSpringLengths(const CellProteins &parentProteins, Grid *grid);
    virtual bool deleteSpringFromList(decltype(springs)::iterator &it, const CellProteins &parentProteins, Grid *grid);
    void splitChildSpring(decltype(springs)::iterator it, const CellProteins &parentProteins, Grid *grid);
    void splitRemeshedChildSpring(decltype(springs)::iterator it, const CellProteins &parentProteins, Grid *grid);

    void distributeDeletedSpringProteins(const CellProteins &proteins, const CellProteins &parentProteins);
    void updateChildSpringReceptorTotalsAfterSplit(const CellProteins &parentProteins);

protected:
    /**
     * Default Constructor.
     *
     * @param springs Optional std::list of Spring objects (defaults to empty std::list).
     */
    ISpringOwner(std::list<Spring> springs = {});

    /**
     * Move Constructor.
     *
     * Updates the *owner pointers of the moved Springs to point to this instance.
     * @note    Must be called by Move Constructor of derived classes.
     *
     * @param other r-value reference to the instance being constructed from.
     */
    ISpringOwner(ISpringOwner &&other) noexcept;

    /**
     * Move Assignment Operator.
     *
     * Updates the *owner pointers of the moved Springs to point to this instance.
     * @note    Must be called by Move Assignment Operator of derived classes.
     *
     * @param other r-value reference to the instance being assigned from.
     */
    ISpringOwner &operator=(ISpringOwner &&other) noexcept;

    ISpringOwner(const ISpringOwner &other) = delete;
    ISpringOwner &operator=(const ISpringOwner &other) = delete;
};

#endif  // ISPRINGOWNER_H
