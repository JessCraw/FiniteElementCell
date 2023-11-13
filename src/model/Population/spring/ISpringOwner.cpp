#include "ISpringOwner.h"

#include "Constants.h"
#include "Spring.h"
#include "model/Population/node/Node.h"
#include "model/environment/Grid.h"

#include <cassert>
#include <utility>

ISpringOwner::ISpringOwner(std::list<Spring> springs)
    : springs(std::move(springs))
{
    for (auto &spring : this->springs)
        spring.owner = this;
}

ISpringOwner::ISpringOwner(ISpringOwner &&other) noexcept
    : springs(std::move(other.springs))
{
    for (auto &spring : springs)
        spring.owner = this;
}

ISpringOwner &ISpringOwner::operator=(ISpringOwner &&other) noexcept
{
    if (this != &other) {
        springs = std::move(other.springs);
        for (auto &spring : springs)
            spring.owner = this;
    }
    return *this;
}

void ISpringOwner::increaseSpringsLigandTotal(Proteins::MembraneLigands ligand, double increase)
{
    increase /= static_cast<double>(springs.size());
    for (auto &spring : springs)
        spring.increaseLigandTotal(ligand, increase);
}

void ISpringOwner::decreaseSpringsLigandTotal(Proteins::MembraneLigands ligand, double decrease)
{
    decrease /= static_cast<double>(springs.size());
    for (auto &spring : springs)
        spring.decreaseLigandTotal(ligand, decrease);
}

void ISpringOwner::updateSpringsReceptorTotal(Proteins::MembraneReceptors receptor, double total)
{
    total /= static_cast<double>(springs.size());
    for (auto &spring : springs)
        spring.setReceptorTotal(receptor, total);
}

void ISpringOwner::updateSpringsProteinTotalConserveActiveReceptors(Proteins::MembraneReceptors receptor, double total)
{
    total /= static_cast<double>(springs.size());
    auto lost = 0.;

    for (auto &spring : springs) {
        auto active = spring.getReceptorActive(receptor);
        spring.setReceptorTotal(receptor, total);
        lost += active - spring.getReceptorActive(receptor);
    }

    if (lost != 0.)
        distributeLostActiveReceptor(receptor, lost);
}

void ISpringOwner::distributeLostActiveReceptor(Proteins::MembraneReceptors receptor, double n)
{
    for (auto &spring : springs)
        n -= spring.increaseReceptorActive(receptor, n);
}

void ISpringOwner::distributeDeletedSpringProteins(const CellProteins &proteins, const CellProteins &parentProteins)
{
    // TODO: make this distribute active receptors randomly or as locally as possible
    for (auto ligand : Proteins::MembraneLigandsArray)
        increaseSpringsLigandTotal(ligand, proteins[ligand].getTotal());

    for (auto receptor : Proteins::MembraneReceptorsArray) {
        updateSpringsReceptorTotal(receptor, parentProteins[receptor].getTotal());
        distributeLostActiveReceptor(receptor, proteins[receptor].getActive());
    }
}

void ISpringOwner::updateChildSpringReceptorTotalsAfterSplit(const CellProteins &parentProteins)
{
    for (auto receptor : Proteins::MembraneReceptorsArray)
        updateSpringsProteinTotalConserveActiveReceptors(receptor, parentProteins[receptor].getTotal());
}

void ISpringOwner::checkChildSpringLengths(const CellProteins &parentProteins, Grid *grid)
{
    for (auto it = springs.begin(); it != springs.end();) {
        if (!it->remeshed()) {
            float len = it->length();
            if (len > it->maxLength()) {
                splitChildSpring(it, parentProteins, grid);
                /* move iterator back to check length of newly inserted spring from split, then re-check current spring
                 * after */
                --it;
            } else if (len < it->minLength() && !it->isFilopodiaBase()) {
                bool deleted = deleteSpringFromList(it, parentProteins, grid);
                if (deleted)
                    continue;
            }
        } else {
            it->checkChildSpringLengths(it->proteins, grid);
            if (it->springs.empty()) {
                bool deleted = deleteSpringFromList(it, parentProteins, grid);
                if (deleted)
                    continue;
            } else if (it->springs.size() > 3) {
                /* child spring holds too many of its own child springs so needs to be split */
                splitRemeshedChildSpring(it, parentProteins, grid);
                /* move iterator back to check number of springs of newly inserted spring from split, then re-check
                 * current spring after */
                --it;
            }
        }
        ++it;
    }
}

void ISpringOwner::splitChildSpring(decltype(springs)::iterator it, const CellProteins &parentProteins, Grid *grid)
{
    auto midpoint = it->createMidpointNode();

    Spring newSpring {it->start, midpoint, this, grid, it->coarsenessLevel};
    newSpring.proteins = it->proteins.split();

    it->changeNode(midpoint, SpringConnectionType::Start);

    midpoint->updateCoordinates(midpoint->getCoordinates());

    Spring::setFilStatesAfterSplit(*it, newSpring);

    springs.insert(it, std::move(newSpring));

    updateChildSpringReceptorTotalsAfterSplit(parentProteins);
}

bool ISpringOwner::deleteSpringFromList(decltype(springs)::iterator &it, const CellProteins &parentProteins, Grid *grid)
{
    /* cannot delete base but this should be checked before */
    assert(!it->isFilopodiaBase());

    /* if tip, then find neighbouring spring that is not tip and set this to be new tip.
     * however, if neighbour is a base then don't delete. */
    Spring *springToConnectToRemainingNode = nullptr;
    std::shared_ptr<Node> nodeToConnectTo, nodeToSwapOut;

    if (it->isFilopodiaTip()) {
        /* neighbour spring that is not a tip */
        springToConnectToRemainingNode = it->getOppositeNode(it->getTipNode())->getOppositeSpring(&*it);

        if (springToConnectToRemainingNode->isFilopodiaBase())
            return false;

        nodeToConnectTo = it->getTipNode();
        nodeToSwapOut = it->getSharedNode(*springToConnectToRemainingNode);

        springToConnectToRemainingNode->filState = it->filState;
        springToConnectToRemainingNode->filTipRetractionTimer = it->filTipRetractionTimer;
    } else {
        // TODO: check setting node like this is ok (just connecting connecting to start every time)
        springToConnectToRemainingNode = it->start->getOppositeSpring(&*it);
        nodeToConnectTo = it->end;
        nodeToSwapOut = it->start;
    }

    /* if spring connected to new node is not same coarseness, then call on spring updated to make sure grid at that
     * coarseness level is generated and also reset surrounding deallocation timers at that coarseness level. */
    if (springToConnectToRemainingNode->coarsenessLevel != it->coarsenessLevel) {
        grid->onSpringPositionUpdated(nodeToSwapOut->getCoordinates(), nodeToConnectTo->getCoordinates(),
                                      springToConnectToRemainingNode->coarsenessLevel);
    }

    springToConnectToRemainingNode->swapNode(nodeToSwapOut, nodeToConnectTo);

    // TODO: call check spring length from this newly connected spring's parent? would this work with update loop?

    /* if not tip or base then no fil state transfer needs to happen */
    auto deletedProteins = it->proteins;
    it = springs.erase(it);
    distributeDeletedSpringProteins(deletedProteins, parentProteins);

    return true;
}

void ISpringOwner::splitRemeshedChildSpring(decltype(springs)::iterator it, const CellProteins &parentProteins,
                                            Grid *grid)
{
    /* determine the midpoint index */
    auto midpoint = it->springs.begin();
    std::advance(midpoint, it->springs.size() / 2);

    /* move first half of child springs into new list */
    std::list<Spring> newList;
    newList.splice(newList.begin(), it->springs, it->springs.begin(), midpoint);

    /* create new spring with moved child springs and insert before original parent spring */
    springs.insert(it, {nullptr, nullptr, this, grid, it->coarsenessLevel, std::move(newList)});

    updateChildSpringReceptorTotalsAfterSplit(parentProteins);
}
