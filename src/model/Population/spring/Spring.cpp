#include "Spring.h"

#include "Constants.h"
#include "Parameters.h"
#include "Population/node/NodeOLD.h"
#include "Population/spring/ISpringOwner.h"
#include "Population/spring/SpringConnectionType.h"
#include "environment/Grid.h"
#include "utility/Colour.h"
#include "utility/Geometry.h"
#include "utility/Random.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <sstream>
#include <type_traits>
#include <utility>

int Spring::nTipNodes = 0;
int Spring::maxTipNodes = 0;
int Spring::idCounter = 0;

Spring::Spring(std::shared_ptr<Node> start, std::shared_ptr<Node> end, ISpringOwner *owner, Grid *grid,
               int coarsenessLevel, std::list<Spring> springs)
    : ISpringOwner(std::move(springs)),
      start(std::move(start)),
      end(std::move(end)),
      owner(owner),
      grid(grid),
      coarsenessLevel(coarsenessLevel)
{
    static int id_ = 0;
    mID = id_++;
    if (!remeshed())
        connectToNodes();
}

Spring::~Spring() { disconnectFromNodes(); }

Spring::Spring(Spring &&other) noexcept
    : ISpringOwner(std::move(other.springs)),
      unmeshTimer(other.unmeshTimer),
      filState(other.filState),
      nFilopodiaBase(other.nFilopodiaBase),
      start(std::move(other.start)),
      end(std::move(other.end)),
      owner(other.owner),
      grid(other.grid),
      mID(other.mID),
      filTipRetractionTimer(other.filTipRetractionTimer),
      prevProtrusionAngle(other.prevProtrusionAngle),
      proteins(other.proteins),
      coarsenessLevel(other.coarsenessLevel)
{
    if (!remeshed())
        moveNodeConnections(other);
}

Spring &Spring::operator=(Spring &&other) noexcept
{
    if (this != &other) {
        ISpringOwner::operator=(std::move(other));
        mID = other.mID;
        unmeshTimer = other.unmeshTimer;
        grid = other.grid;
        start = std::move(other.start);
        end = std::move(other.end);
        owner = other.owner;
        proteins = other.proteins;
        filState = other.filState;
        prevProtrusionAngle = other.prevProtrusionAngle;
        nFilopodiaBase = other.nFilopodiaBase;
        filTipRetractionTimer = other.filTipRetractionTimer;
        coarsenessLevel = other.coarsenessLevel;

        if (!remeshed())
            moveNodeConnections(other);
    }
    return *this;
};

bool Spring::operator==(const Spring &other) { return this == &other; }

void Spring::connectToNodes()
{
    start->addSpring(this, SpringConnectionType::Start);
    end->addSpring(this, SpringConnectionType::End);
}

void Spring::disconnectFromNodes()
{
    if (start) {
        start->removeSpring(this);
        start.reset();
    }
    if (end) {
        end->removeSpring(this);
        end.reset();
    }
}

void Spring::moveNodeConnections(const Spring &other)
{
    if (start) {
        start->replaceSpring(&other, this);
    }
    if (end) {
        end->replaceSpring(&other, this);
    }
}

void Spring::takeNodeConnections(Spring *other)
{
    if (other->start) {
        changeNode(other->start, SpringConnectionType::Start);
        other->start->removeSpring(other);
    }

    if (other->end) {
        changeNode(other->end, SpringConnectionType::End);
        other->end->removeSpring(other);
    }
}

void Spring::update()
{
    if (!remeshed()) {
        // std::cout << "Remeshing the spring " << !remeshed() << std::endl;
        signalling();
        checkRemeshing();
    } else {
        for (auto &spring : springs)
            spring.update();
    }
    // checkUnmeshing();
}

bool Spring::retractionFinished()
{
    static constexpr float tolerance = 1.05;
    auto tipNode = getTipNode();
    auto tipNeighbour1 = getOppositeNode(tipNode);
    auto tipNeighbour2 = tipNode->getOppositeSpring(this)->getOppositeNode(tipNode);

    return Geometry::onLineBetween(tipNeighbour1->getCoordinates(), tipNeighbour2->getCoordinates(),
                                   tipNode->getCoordinates(), tolerance);
}

void Spring::startDisablingCurrentFilopodia()
{
    auto tipNode = getTipNode();
    auto otherNode = getOppositeNode(tipNode);
    auto *connectedStem = otherNode->getOpposteSpring2dHack(this);

    tipNode->adhered = true;
    prevProtrusionAngle = Filopodia::NO_PREV_PROTRUSION_ANGLE;
    filTipRetractionTimer = 0;
    retractionInactivityTimer = 0;
    filState = Filopodia::State::NONE;
    connectedStem->disableRestOfFilopodia(otherNode);
    tipNode->getOpposteSpring2dHack(this)->disableRestOfFilopodia(tipNode);
    --nTipNodes;
}

void Spring::move(int &actinUsed, const Coordinates &cellCenter)
{
    if (!remeshed()) {
        if (retracting()) {
            if (retractionFinished())
                startDisablingCurrentFilopodia();
        } else {
            bool extended = attemptExtension(actinUsed, cellCenter);
            if (!extended)
                incrementRetractionTimer(actinUsed);
        }
    } else {
        for (auto &spring : springs)
            spring.move(actinUsed, cellCenter);
    }
}

void Spring::signalling()
{
    std::array<double, Proteins::N_PROTEINS> values {0};
    values[Proteins::Index::VEGFR_ACTIVE] = getReceptorActive(Proteins::MembraneReceptors::VEGFR);
    values[Proteins::Index::VEGFR_UNBOUND] = proteins[Proteins::MembraneReceptors::VEGFR].getUnbound();

    grid->vegfrVegfBinding(values, start->getCoordinates(), end->getCoordinates(), coarsenessLevel);

    auto increase = values[Proteins::Index::VEGFR_ACTIVE] - getReceptorActive(Proteins::MembraneReceptors::VEGFR);
    increaseReceptorActive(Proteins::MembraneReceptors::VEGFR, increase);
}

#ifdef UNITTEST
void Spring::signallingWithRemeshCheck()
{
    if (!remeshed()) {
        std::array<double, Proteins::N_PROTEINS> values {0};
        values[Proteins::Index::VEGFR_ACTIVE] = getReceptorActive(Proteins::MembraneReceptors::VEGFR);
        values[Proteins::Index::VEGFR_UNBOUND] = proteins[Proteins::MembraneReceptors::VEGFR].getUnbound();

        grid->vegfrVegfBindingNoProteinRemoval(values, start->getCoordinates(), end->getCoordinates(), coarsenessLevel);

        auto increase = values[Proteins::Index::VEGFR_ACTIVE] - getReceptorActive(Proteins::MembraneReceptors::VEGFR);
        increaseReceptorActive(Proteins::MembraneReceptors::VEGFR, increase);
    } else {
        for (auto &spring : springs) {
            spring.signallingWithRemeshCheck();
        }
    }
}
#endif

void Spring::hookesLaw()
{
    if (!remeshed()) {
        /* TODO:
         * calc spring forces according to hookes law if start and/or end node are not focal adhesions.
         * apply this force to move node non adhered node(s).
         * */
    } else {
        for (auto &spring : springs)
            spring.hookesLaw();
    }
}

void Spring::checkRemeshing()
{
    double activation = proteins.getReceptorActivation(Proteins::MembraneReceptors::VEGFR);
    if (activation > static_cast<double>(AceParameters::Instance().remeshActivation) &&
        proteins[Proteins::MembraneReceptors::VEGFR].getActive() >
            static_cast<double>(AceParameters::Instance().remeshVegfrMin)) {
        ++remeshTimer;
    } else {
        remeshTimer = 0;
    }

    if (canRemesh())
        remesh();
}

bool Spring::canRemesh() const
{
    return !remeshed() && coarsenessLevel > Environment::Grid::coarsenessMin &&
           remeshTimer > Parameters::Defaults::remeshTime;
}

void Spring::remesh()
{
    remeshTimer = 0;
    unmeshTimer = 0;

    // Add extra space needed for Moore Neighbourhood around Nodes
    float extraSpace = Environment::Grid::rangeAtCoarseness[coarsenessLevel - 1];
    grid->createRefinedGrid(start->coordinates, end->coordinates, coarsenessLevel - 1, extraSpace);

    auto midpointNode = createMidpointNode();

    createRemeshedSpring(start, midpointNode, start);
    createRemeshedSpring(midpointNode, end, end);

    auto proteinSplit = proteins.getSplit();
    for (auto &spring : springs)
        spring.proteins = proteinSplit;

    disconnectFromNodes();
}

void Spring::createRemeshedSpring(const std::shared_ptr<Node> &startNode, const std::shared_ptr<Node> &endNode,
                                  const std::shared_ptr<Node> &nonSharedNode)
{
    springs.emplace_back(startNode, endNode, this, grid, coarsenessLevel - 1);
    setRemeshedSpringsFilStates(springs.back(), nonSharedNode);
}

void Spring::setRemeshedSpringsFilStates(Spring &spring, const std::shared_ptr<Node> &nonSharedNode) const
{
    if (isFilopodiaStem() || notInFilopodia()) {
        spring.filState = filState;
    } else {
        auto *oppositeSpring = nonSharedNode->getOpposteSpring2dHack(&spring);
        if (filState == Filopodia::State::BASE) {
            if (oppositeSpring->isFilopodiaStem() || oppositeSpring->isFilopodiaTip()) {
                spring.filState = Filopodia::State::BASE;
                spring.nFilopodiaBase = 1;
            } else {
                spring.filState = Filopodia::State::NONE;
            }
        } else if (filState == Filopodia::State::TIP) {
            if (oppositeSpring->isFilopodiaTip()) {
                spring.filState = Filopodia::State::TIP;
                spring.filTipRetractionTimer = filTipRetractionTimer;
            } else {
                spring.filState = Filopodia::State::STEM;
            }
        }
    }
}

void Spring::checkUnmeshing()
{
    if (remeshed()) {
        if (canUnmesh())
            unmesh();
    } else {
        if (remeshTimer == 0 && coarsenessLevel < Environment::Grid::coarsenessMax)
            unmeshTimer++;
        else
            unmeshTimer = 0;
    }
}

bool Spring::canUnmesh() const
{
    if (!remeshed()) {
        return unmeshTimer >= AceParameters::Instance().unmeshTime &&
               coarsenessLevel < Environment::Grid::coarsenessMax;
    }

    int nTip = 0;
    int nBase = 0;
    bool allCanUnmesh = true;
    for (const auto &spring : springs) {
        if (spring.isFilopodiaBase()) {
            ++nBase;
        } else if (spring.isFilopodiaTip()) {
            ++nTip;
        }

        if (!spring.canUnmesh())
            allCanUnmesh = false;
    }

    /* Cannot unmesh when all child springs are tip springs and child springs > 1. Filopodia must have two tip edges and
     * unmeshing a parent spring that owns both child springs will result in the parent being the only tip edge. */
    if (nTip > 1)
        return false;

    /* When unmeshing, a parent spring must take on a filopodia state from child springs. Neither base or tip states can
     * take priority over each other therefore parent cannot unmesh whilst it holds child springs with both these
     * states. */
    if (nBase && nTip)
        return false;

    return allCanUnmesh;
}

void Spring::unmesh()
{
    bool hasTip = std::any_of(springs.begin(), springs.end(),
                              [](const auto &spring) { return spring.filState == Filopodia::State::TIP; });

    int nBase = std::accumulate(springs.begin(), springs.end(), 0,
                                [](int count, const auto &spring) { return count + spring.nFilopodiaBase; });

    nFilopodiaBase = nBase;
    /* cannot unmesh if child springs contain both BASE and TIP states */
    if (!((nBase > 0) && hasTip)) {
        changeNode(springs.front().start, SpringConnectionType::Start);
        changeNode(springs.back().end, SpringConnectionType::End);

        for (auto receptor : Proteins::MembraneReceptorsArray)
            proteins[receptor].setActive(getReceptorActive(receptor));

        for (auto ligand : Proteins::MembraneLigandsArray)
            proteins[ligand].setTotal(getProteinTotal(ligand));

        auto firstState = springs.begin()->filState;
        if (std::all_of(springs.begin(), springs.end(),
                        [firstState](const auto &spring) { return spring.filState == firstState; })) {
            filState = firstState;
        } else {
            if (hasTip) {
                filState = Filopodia::State::TIP;
            } else if (nBase > 0) {
                filState = Filopodia::State::BASE;
            } else {
                /* should not reach this point, getting here would mean that child springs would contain a mix of fil
                 * state NONE and STEM */
                exit(-1);
            }
        }

        if (isFilopodiaTip()) {
            for (const auto &spring : springs) {
                if (spring.isFilopodiaTip()) {
                    filTipRetractionTimer = spring.filTipRetractionTimer;
                    break;
                }
            }
        }

        springs.clear();
        unmeshTimer = 0;
    }
}

bool Spring::attemptExtension(int &cellActinUsed, const Coordinates &cellCenter)
{
    if (cellActinUsed < AceParameters::Instance().cellActinMax) {
        // TODO: maybe have rule cannot share node with a spring that is in stem? would spread filopodia out more and og
        // base stem would only be able to be stem for one filopodia at a time
        if (maxTipNodes && nTipNodes >= maxTipNodes && filState != Filopodia::State::TIP)
            return false;

        if (isFilopodiaTip() || notInFilopodia()) {
            std::shared_ptr<Node> tipNode = getNodeToMove();
            if (tipNode) {
                if (activationMeetsExtensionRequirements()) {
                    auto [angle, coordinates] = getExtensionAngleAndCoordinates(tipNode, cellCenter);
                    /* stop from deleting tip due to being too small and then giving tip state to neighbouring edge
                     * which is fil base */
                    if (Grid::withinExtensionBounds(coordinates)) {
                        /* If starting filopodia, set states of self and neighbours */
                        if (filState == Filopodia::State::NONE) {
                            auto *oppositeFilTop = tipNode->getOpposteSpring2dHack(this);
                            /* had trouble before where tip was started and immediately deleted here because its length
                             * is less than min. This is a problem from elsewhere in the code where min / max lengths
                             * need to be enforced. this hack takes care of it for now though */
                            if (getLength() < Environment::Grid::getRangeAtCoarseness(coarsenessLevel) ||
                                oppositeFilTop->getLength() <
                                    Environment::Grid::getRangeAtCoarseness(oppositeFilTop->coarsenessLevel)) {
                                return false;
                            }

                            /* self */
                            filState = Filopodia::State::TIP;
                            auto oppositeNode = getOppositeNode(tipNode);
                            auto baseSpring = oppositeNode->getOpposteSpring2dHack(this);
                            baseSpring->filState = Filopodia::State::BASE;
                            ++baseSpring->nFilopodiaBase;

                            /* spring sharing tip node */
                            oppositeFilTop->filState = Filopodia::State::TIP;
                            baseSpring =
                                oppositeFilTop->getOppositeNode(tipNode)->getOpposteSpring2dHack(oppositeFilTop);
                            baseSpring->filState = Filopodia::State::BASE;
                            ++baseSpring->nFilopodiaBase;

                            ++nTipNodes;
                            /* Need to call extend here to save start position in list of extension. */
                            tipNode->extend(tipNode->getCoordinates(), cellActinUsed);
                        }

                        /* now can finally extend */
                        prevProtrusionAngle = angle;
                        tipNode->extend(coordinates, cellActinUsed);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Spring::activationMeetsExtensionRequirements() const
{
    auto total = proteins[Proteins::MembraneReceptors::VEGFR].getTotal();
    auto activation = proteins.getReceptorActivation(Proteins::MembraneReceptors::VEGFR);

    return activation > static_cast<double>(AceParameters::Instance().extensionActivation) &&
           total > static_cast<double>(AceParameters::Instance().extensionActivationMin);
}

std::pair<float, Coordinates> Spring::getExtensionAngleAndCoordinates(const std::shared_ptr<Node> &nodeToMove,
                                                                      const Coordinates &cellCenter)
{
    /* if not yet protruded then get angle perpendicular from current point and line between opposite nodes to tip node
     */
    if (prevProtrusionAngle == Filopodia::NO_PREV_PROTRUSION_ANGLE) {
        auto oppositeNodeCoords = getOppositeNode(nodeToMove)->getCoordinates();
        auto *oppositeFilTop = nodeToMove->getOpposteSpring2dHack(this);
        auto oppositeFilTopOppositeNodeCoords = oppositeFilTop->getOppositeNode(nodeToMove)->getCoordinates();
        auto nodeCoordinates = nodeToMove->getCoordinates();

        auto perpPoint = Geometry::calcPointOnLineThroughExternalPointGivingPerpendicularLine(
            nodeCoordinates, oppositeNodeCoords, oppositeFilTopOppositeNodeCoords);

        auto angle = std::atan2(nodeCoordinates.y - perpPoint.y, nodeCoordinates.x - perpPoint.x);
        auto coordinates = Spring::getExtensionCoordinates(nodeCoordinates, angle);

        auto reversedAngle = Geometry::reverseAngle(angle);
        auto reversedCoordinates = Spring::getExtensionCoordinates(nodeCoordinates, reversedAngle);

        /* check if reverse / normal is closer to cell center. Protruding outward so want angle which is going away from
         * cell center */
        if (Geometry::getLength(coordinates, cellCenter) > Geometry::getLength(reversedCoordinates, cellCenter)) {
            return {angle, coordinates};
        } else {
            return {reversedAngle, reversedCoordinates};
        }
    }

    /* else return random variation on previous protrusion angle */
    auto var = Random::generateNumberBetween(-AceParameters::Instance().filopodiaExtensionAngleVariation,
                                             AceParameters::Instance().filopodiaExtensionAngleVariation);

    auto angle = prevProtrusionAngle + var;
    auto coordinates = Spring::getExtensionCoordinates(nodeToMove->getCoordinates(), angle);

    return {angle, coordinates};
}

Coordinates Spring::getExtensionCoordinates(const Coordinates &start, float angle)
{
    auto x = start.x + Filopodia::EXTENSION_DISTANCE * std::cos(angle);
    auto y = start.y + Filopodia::EXTENSION_DISTANCE * std::sin(angle);
    return {x, y, start.z};
}

bool Spring::extensionWillCauseBaseStemStateChange(const std::shared_ptr<Node> &tipNode, const Coordinates &coordinates)
{
    const auto &otherNode = getOppositeNode(tipNode);
    auto adjacentBase = otherNode->getOpposteSpring2dHack(this)->isFilopodiaBase();

    if (adjacentBase) {
        auto newLength = Geometry::getLength(otherNode->getCoordinates(), coordinates);
        if (newLength < static_cast<float>(Environment::Grid::getRangeAtCoarseness(coarsenessLevel)))
            return true;
    }

    /* also check other spring sharing tip node */
    const auto &otherTipSpring = tipNode->getOpposteSpring2dHack(this);
    const auto &otherTipSpringOtherNode = otherTipSpring->getOppositeNode(tipNode);
    adjacentBase = otherTipSpringOtherNode->getOpposteSpring2dHack(otherTipSpring)->isFilopodiaBase();

    if (adjacentBase) {
        auto newLength = Geometry::getLength(otherTipSpringOtherNode->getCoordinates(), coordinates);
        if (newLength < static_cast<float>(Environment::Grid::getRangeAtCoarseness(otherTipSpring->coarsenessLevel)))
            return true;
    }

    return false;
}

std::shared_ptr<Node> Spring::getTipNode()
{
    if (isFilopodiaTip()) {
        if (start->getOpposteSpring2dHack(this)->isFilopodiaTip())
            return start;
        else
            return end;
    }

    return nullptr;
}

std::shared_ptr<Node> Spring::getNodeToMove()
{
    /* if tip already then node to move is one shared with other tip spring */
    auto tipNode = getTipNode();
    if (tipNode)
        return tipNode;

    /* else is not in a filopodia yet */

    auto *startOppositeSpring = start->getOpposteSpring2dHack(this);
    auto *endOppositeSpring = end->getOpposteSpring2dHack(this);

    /* if both neighbours are already in filopodia then cannot extend either node */
    if (startOppositeSpring->isInFilopodiaStem() && endOppositeSpring->isInFilopodiaStem())
        return nullptr;

    /* check if only one neighbour is not in filopodia. Node shared with this neighbour would then be only one that can
     * be moved. */
    if (startOppositeSpring->isInFilopodiaStem() && endOppositeSpring->notInFilopodia())
        return end;

    if (endOppositeSpring->isInFilopodiaStem() && startOppositeSpring->notInFilopodia())
        return start;

    /* If neither neighbour are already in filopodia then choose node with highest VEGF sum in its moore neighbourhood
     */
    auto startMooreVegfSum = grid->getVegfSumOfMooreNeighbourhood(start->getCoordinates(), coarsenessLevel);
    auto endMooreVegfSum = grid->getVegfSumOfMooreNeighbourhood(end->getCoordinates(), coarsenessLevel);

    return (startMooreVegfSum > endMooreVegfSum) ? start : end;
}

bool Spring::retracting() const
{
    return isFilopodiaTip() && filTipRetractionTimer > AceParameters::Instance().retractionTime;
}

bool Spring::retractionInactive() const { return retractionInactivityTimer > Filopodia::RETRACTION_INACTIVITY_TIMER; }

void Spring::disableRestOfFilopodia(const std::shared_ptr<Node> &sharedNode)
{
    if (isFilopodiaBase()) {
        if (--nFilopodiaBase == 0)
            resetFilopodiaStates();
    } else {
        resetFilopodiaStates();
        auto otherNode = getOppositeNode(sharedNode);
        auto oppositeSpring = otherNode->getOpposteSpring2dHack(this);
        if (oppositeSpring->inFilopodia())
            oppositeSpring->disableRestOfFilopodia(otherNode);
    }
}

void Spring::resetFilopodiaStates()
{
    prevProtrusionAngle = Filopodia::NO_PREV_PROTRUSION_ANGLE;
    filTipRetractionTimer = 0;
    retractionInactivityTimer = 0;
    filState = Filopodia::State::NONE;
}

void Spring::incrementRetractionTimer(int &cellActinUsed)
{
    if (isFilopodiaTip()) {
        ++filTipRetractionTimer;
        /* When retraction requirements first met, also set other fil tip to be retracting */
        if (filTipRetractionTimer > AceParameters::Instance().retractionTime) {
            auto tipNode = getTipNode();
            tipNode->adhered = false;
            cellActinUsed -= tipNode->actinUsedByNode;
            tipNode->actinUsedByNode = 0;
            auto *oppositeFilTip = tipNode->getOpposteSpring2dHack(this);
            oppositeFilTip->filTipRetractionTimer = filTipRetractionTimer;
        }
    }
}

void Spring::changeNode(std::shared_ptr<Node> node, SpringConnectionType connectionType)
{
    auto &n = (connectionType == SpringConnectionType::Start) ? start : end;
    if (n)
        n->removeSpring(this);

    n = node;
    if (n)
        n->addSpring(this, connectionType);
}

bool Spring::deleteSpringFromList(decltype(springs)::iterator &it, const CellProteins & /*parentProteins*/,
                                  Grid * /*grid*/)
{
    if (springs.size() > 1)
        return ISpringOwner::deleteSpringFromList(it, proteins, grid);

    /* last child spring being deleted */
    takeNodeConnections(&*it);  // TODO: make take reference
    proteins = it->proteins;
    filState = it->filState;
    filTipRetractionTimer = it->filTipRetractionTimer;
    it = springs.erase(it);

    return true;
}

float Spring::length() const { return Geometry::getLength(start->coordinates, end->coordinates); }

std::shared_ptr<Node> Spring::getOppositeNode(SpringConnectionType otherConnectionType) const
{
    return (otherConnectionType == SpringConnectionType::Start) ? end : start;
}

SpringConnectionType Spring::getNodeConnectionType(const std::shared_ptr<Node> &node) const
{
    if (node != end && node != start)
        exit(-1);
    return (node == start) ? SpringConnectionType::Start : SpringConnectionType::End;
}

std::shared_ptr<Node> Spring::getOppositeNode(const std::shared_ptr<Node> &node) const
{
    if (node != end && node != start)
        return nullptr;
    return (node == start) ? end : start;
}

Coordinates Spring::getMidpoint() const
{
    return {(start->coordinates.x + end->coordinates.x) / 2, (start->coordinates.y + end->coordinates.y) / 2,
            (start->coordinates.z + end->coordinates.z) / 2};
}

float Spring::getLength() const
{
    if (remeshed())
        return std::accumulate(springs.begin(), springs.end(), 0.F,
                               [](auto count, const auto &spring) { return count + spring.getLength(); });

    return Geometry::getLength(start->coordinates, end->coordinates);
}

std::shared_ptr<Node> Spring::getSharedNode(const Spring &other)
{
    if (start->getOppositeSpring(this) == &other)
        return start;

    if (end->getOppositeSpring(this) == &other)
        return end;

    return nullptr;
}

void Spring::setFilStatesAfterSplit(Spring &originalSpring, Spring &newSpring)
{
    if (originalSpring.isFilopodiaTip()) {
        /* find which one is connected to a tip on other side and set this one to tip and the other to stem */
        newSpring.filState = Filopodia::State::STEM;

        auto sharedNode = newSpring.getSharedNode(originalSpring);
        auto *opposite = newSpring.getOppositeNode(sharedNode)->getOppositeSpring(&newSpring);

        if (opposite != &originalSpring && opposite->isFilopodiaTip()) {
            std::swap(newSpring.filState, originalSpring.filState);
            newSpring.filTipRetractionTimer = originalSpring.filTipRetractionTimer;
            originalSpring.filTipRetractionTimer = 0;
        }
    } else if (originalSpring.isFilopodiaBase()) {
        auto sharedNode = newSpring.getSharedNode(originalSpring);
        auto *newSpringOppositeSpring = newSpring.getOppositeNode(sharedNode)->getOppositeSpring(&newSpring);
        auto *originalSpringOppositeSpring =
            originalSpring.getOppositeNode(sharedNode)->getOppositeSpring(&originalSpring);

        if ((newSpringOppositeSpring->isFilopodiaStem() || newSpringOppositeSpring->isFilopodiaTip()) &&
            (originalSpringOppositeSpring->isFilopodiaStem() || originalSpringOppositeSpring->isFilopodiaTip())) {
            /* Filopodia either side so both become bases */
            newSpring.filState = Filopodia::State::BASE;
            newSpring.nFilopodiaBase++;
            originalSpring.nFilopodiaBase--;
        } else if ((newSpringOppositeSpring->isFilopodiaStem() || newSpringOppositeSpring->isFilopodiaTip()) &&
                   !(originalSpringOppositeSpring->isFilopodiaStem() ||
                     originalSpringOppositeSpring->isFilopodiaTip())) {
            /* original is no longer connected to a filopodia so new spring becomes base */
            newSpring.filState = Filopodia::State::BASE;
            newSpring.nFilopodiaBase = 1;
            originalSpring.nFilopodiaBase = 0;
            originalSpring.filState = Filopodia::State::NONE;
        } else if (!(newSpringOppositeSpring->isFilopodiaStem() || newSpringOppositeSpring->isFilopodiaTip()) &&
                   (originalSpringOppositeSpring->isFilopodiaStem() ||
                    originalSpringOppositeSpring->isFilopodiaTip())) {
            /* original is still connectedd to filopodia and new is not so new is not part of a filopodia */
            newSpring.filState = Filopodia::State::NONE;
        } else {
            /* this would mean there is no tip or stalk either side of the base. somethings gone wrong. */
            assert(false);
        }
    } else {
        /* filopodia stem or none */
        newSpring.filState = originalSpring.filState;
    }
}

void Spring::swapNode(const std::shared_ptr<Node> &originalNode, const std::shared_ptr<Node> &newNode)
{
    assert(originalNode == start || originalNode == end);
    if (originalNode == start) {
        changeNode(newNode, SpringConnectionType::Start);
    } else {
        changeNode(newNode, SpringConnectionType::End);
    }
}

int Spring::getCellId() const { return start->cellId; }

std::shared_ptr<Node> Spring::createMidpointNode() const
{
    std::cout << " Need to implement createMidpointNode() for spring new node ID " << this << std::endl;
    return std::make_shared<Node>(getCellId(), 0, grid, getMidpoint());
}

double Spring::getProteinTotal(Proteins::MembraneReceptors receptor) const { return proteins[receptor].getTotal(); }

double Spring::getProteinTotal(Proteins::MembraneLigands ligand) const
{
    if (remeshed())
        return std::accumulate(springs.begin(), springs.end(), 0., [&ligand](auto count, const auto &spring) {
            return count + spring.getProteinTotal(ligand);
        });
    return proteins[ligand].getTotal();
}

void Spring::increaseLigandTotal(Proteins::MembraneLigands ligand, double n)
{
    proteins[ligand].increaseTotal(n);
    if (remeshed())
        increaseSpringsLigandTotal(ligand, n);
}

void Spring::decreaseLigandTotal(Proteins::MembraneLigands ligand, double n)
{
    proteins[ligand].decreaseTotal(n);
    if (remeshed())
        decreaseSpringsLigandTotal(ligand, n);
}

void Spring::setReceptorTotal(Proteins::MembraneReceptors receptor, double n)
{
    proteins[receptor].setTotal(n);
    if (remeshed())
        updateSpringsReceptorTotal(receptor, n);
}

double Spring::getReceptorActive(Proteins::MembraneReceptors receptor) const
{
    if (remeshed())
        return std::accumulate(springs.begin(), springs.end(), 0., [&receptor](auto count, const auto &spring) {
            return count + spring.getReceptorActive(receptor);
        });
    return proteins[receptor].getActive();
}

double Spring::decreaseReceptorActive(Proteins::MembraneReceptors receptor, double n)
{
    auto active = getReceptorActive(receptor);
    auto decrease = active - std::max(0., active - n);
    auto leftover = decrease;

    if (remeshed()) {
        for (auto &spring : springs)
            leftover -= spring.decreaseReceptorActive(receptor, leftover);
    } else {
        leftover -= proteins[receptor].decreaseActive(leftover);
    }
    // assert(leftover == 0);

    return decrease;
}

double Spring::increaseReceptorActive(Proteins::MembraneReceptors receptor, double n)
{
    auto active = getReceptorActive(receptor);
    auto increase = std::min(proteins[receptor].getTotal(), active + n) - active;
    auto leftover = increase;

    if (remeshed()) {
        for (auto &spring : springs)
            leftover -= spring.increaseReceptorActive(receptor, leftover);
    } else {
        leftover -= proteins[receptor].increaseActive(leftover);
    }
    // assert(leftover == 0);

    return increase;
}

double Spring::getReceptorUnbound(Proteins::MembraneReceptors receptor) const
{
    if (remeshed())
        return std::accumulate(springs.begin(), springs.end(), 0., [&receptor](auto count, const auto &spring) {
            return count + spring.getReceptorUnbound(receptor);
        });
    return proteins[receptor].getUnbound();
}

void Spring::scaleReceptorActiveByFactor(Proteins::MembraneReceptors receptor, double factor)
{
    proteins[receptor].setActive(proteins[receptor].getActive() * factor);
    for (auto &spring : springs)
        spring.scaleReceptorActiveByFactor(receptor, factor);
}

void Spring::getGraphics(std::vector<Coordinates> &coordinates, std::vector<Colour> &colours, unsigned int &nSprings,
                         std::vector<float> &thickness, int highlightedEdgeID,
                         const std::array<bool, Proteins::N_PROTEINS> &showProteins,
                         const std::array<bool, Filopodia::State::N> &showFilStates) const
{
    if (!remeshed()) {
        // auto colour = COARSENESS_COLOUR_MAP_SPRINGS[coarsenessLevel];
        auto colour = COARSENESS_COLOUR_MAP_SPRINGS[Environment::Grid::coarsenessMax];

        if (showProteins[Proteins::Index::VEGFR_ACTIVE]) {
            auto vegfr = proteins[Proteins::MembraneReceptors::VEGFR];
            auto activeRatio = static_cast<float>(vegfr.getActive() / vegfr.getTotal());
            if (activeRatio > 0) {
                colour.red -= activeRatio;
                colour.green += activeRatio;
                colour.blue -= activeRatio;
            }
        }

        if (showFilStates[Filopodia::State::TIP] && isFilopodiaTip())
            colour = {0, 1, 0, 1};

        if (showFilStates[Filopodia::State::STEM] && isFilopodiaStem())
            colour = {0, 0, 1, 1};

        if (showFilStates[Filopodia::State::BASE] && isFilopodiaBase())
            colour = {1, 0, 0, 1};

        if (mID == highlightedEdgeID)
            colour = Graphics::highlightedEdgeColour;

        coordinates.push_back(start->coordinates);
        coordinates.push_back(end->coordinates);
        thickness.push_back(COARSENESS_SPRING_THICKNESS_MAP[coarsenessLevel]);
        colours.push_back(colour);
        nSprings++;
    } else {
        for (const auto &spring : springs)
            spring.getGraphics(coordinates, colours, nSprings, thickness, highlightedEdgeID, showProteins,
                               showFilStates);
    }
}

Node *Spring::getStartNode() const
{
    if (remeshed())
        return springs.front().getStartNode();
    return start.get();
}

Spring *Spring::getSpring()
{
    if (remeshed())
        return springs.front().getSpring();

    return this;
}

void Spring::getNodeGraphics(std::vector<Coordinates> &coordinates, std::vector<Colour> &colours, unsigned int &nNodes,
                             std::vector<float> &sideLengths) const
{
    if (!remeshed()) {
        coordinates.push_back(start->coordinates);
        coordinates.push_back(end->coordinates);
        colours.push_back(COARSENESS_COLOUR_MAP_NODES[coarsenessLevel]);
        colours.push_back(COARSENESS_COLOUR_MAP_NODES[coarsenessLevel]);
        sideLengths.push_back(COARSENESS_CUBE_SIZE_MAP[coarsenessLevel]);
        sideLengths.push_back(COARSENESS_CUBE_SIZE_MAP[coarsenessLevel]);
        nNodes += 2;

    } else {
        for (const auto &spring : springs)
            spring.getNodeGraphics(coordinates, colours, nNodes, sideLengths);
    }
}

void Spring::getVisibleGridGraphics(std::vector<Coordinates> &coordinates, std::vector<Colour> &colours,
                                    std::vector<int> &sideLengths, unsigned int &n) const
{
    if (springs.empty()) {
        const auto startVisibleGrid = start->getVisibleGrid(coarsenessLevel);
        const auto endVisibleGrid = end->getVisibleGrid(coarsenessLevel);

        coordinates.insert(coordinates.end(), startVisibleGrid.begin(), startVisibleGrid.end());
        coordinates.insert(coordinates.end(), endVisibleGrid.begin(), endVisibleGrid.end());

        for (size_t i = 0; i < startVisibleGrid.size() + endVisibleGrid.size(); i++) {
            colours.push_back(COARSENESS_COLOUR_MAP[coarsenessLevel]);
            sideLengths.push_back(Environment::Grid::getRangeAtCoarseness(coarsenessLevel));
        }

        n += startVisibleGrid.size() + endVisibleGrid.size();
    } else {
        for (const auto &spring : springs)
            spring.getVisibleGridGraphics(coordinates, colours, sideLengths, n);
    }
}

void Spring::resetSurroundingDeallocationTimers() const
{
    if (!remeshed()) {
        start->resetVisibleGridSiteDeallocationTimers(coarsenessLevel);
        end->resetVisibleGridSiteDeallocationTimers(coarsenessLevel);
    } else {
        for (const auto &spring : springs)
            spring.resetSurroundingDeallocationTimers();
    }
}

int Spring::sumNodeCoordinates(Coordinates &sum) const
{
    int n = 0;
    if (remeshed()) {
        for (auto &spring : springs)
            n += spring.sumNodeCoordinates(sum);
    } else {
        sum += start->getCoordinates();
        sum += end->getCoordinates();
        n += 2;
    }
    return n;
}

// Jess wrote this
int Spring::GetSpringId() { return mID; }

#if GRAPHICS
void Spring::getInfoForGui(std::vector<SpringGuiInfo> &retval) const
{
    if (remeshed()) {
        for (const auto &spring : springs)
            spring.getInfoForGui(retval);
    } else {
        SpringGuiInfo info;
        info.id = mID;
        info.coarseness = coarsenessLevel;
        std::ostringstream startAddress;
        startAddress << (void const *)start.get();
        info.startNodeAddr = startAddress.str();
        std::ostringstream endAddress;
        endAddress << (void const *)end.get();
        info.endNodeAddr = endAddress.str();

        info.proteins[Proteins::Index::VEGFR_TOTAL] = proteins[Proteins::MembraneReceptors::VEGFR].getTotal();
        info.proteins[Proteins::Index::VEGFR_ACTIVE] = proteins[Proteins::MembraneReceptors::VEGFR].getActive();
        info.proteins[Proteins::Index::VEGFR_UNBOUND] = proteins[Proteins::MembraneReceptors::VEGFR].getUnbound();

        info.nFilopodiaBase = nFilopodiaBase;

        retval.push_back(info);
    }
}
#endif  // GRAPHICS
