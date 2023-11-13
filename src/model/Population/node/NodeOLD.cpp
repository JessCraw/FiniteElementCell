#include "Population/node/NodeOLD.h"

#include "Constants.h"
#include "Parameters.h"
#include "Population/spring/Spring.h"
#include "Population/spring/SpringConnectionType.h"
#include "environment/Grid.h"
#include "environment/LocationsContainer.h"
#include "environment/LocationsContainerTree.h"
#include "utility/Coordinates.h"
#include "utility/Geometry.h"
#include "utility/Neighbourhood.h"
#include "utility/Random.h"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <memory>
#include <utility>
#include <vector>

NodeOLD::NodeOLD(int cellId, int nodeId, Grid *grid, const Coordinates &coordinates)
    : cellId(cellId),
      coordinates(coordinates),
      grid(grid)
{
    mNodeId = nodeId;
    // std::cout << mNodeId << std::endl;
}

NodeOLD::~NodeOLD()
{
    for (auto &connection : springs)
        connection.spring->changeNode(nullptr, connection.connectionType);
}

NodeOLD::NodeOLD(Node &&other) noexcept
    : cellId(other.cellId),
      coordinates(other.coordinates),
      springs(std::move(other.springs)),
      actinUsedByNode(other.actinUsedByNode),
      adhered(other.adhered),
      grid(other.grid)
{
    for (auto &springConnection : springs)
        if (springConnection.connectionType == SpringConnectionType::Start)
            springConnection.spring->start = shared_from_this();
        else
            springConnection.spring->end = shared_from_this();
}

Node &NodeOLD::operator=(Node &&other) noexcept
{
    if (&other != this) {
        coordinates = other.coordinates;
        cellId = other.cellId;
        springs = std::move(other.springs);
        grid = other.grid;
        adhered = other.adhered;
        actinUsedByNode = other.actinUsedByNode;

        for (auto &springConnection : springs)
            if (springConnection.connectionType == SpringConnectionType::Start)
                springConnection.spring->start = shared_from_this();
            else
                springConnection.spring->end = shared_from_this();
    }

    return *this;
}

int NodeOLD::GetNodeId() { return mNodeId; }

void NodeOLD::resetVisibleGridSiteDeallocationTimers(int coarsenessLevel) const
{
    auto *lct = grid->getGridAtPositionAndDepth(coordinates, coarsenessLevel);
    auto arrayIndex = lct->getValue().arrayPositionsFromCoords(coordinates);

    for (const auto &mooreIndex : Neighbourhood::Moore::get(arrayIndex))
        lct->resetDeallocationTimerAt(mooreIndex);
}

std::vector<Coordinates> NodeOLD::getVisibleGrid(int coarsenessLevel)
{
    return grid->getVisibleGridSiteCoordinates(this->coordinates, coarsenessLevel);
}

void NodeOLD::updateCoordinates(const Coordinates &newCoordinates)
{
    auto oldCoordinates = coordinates;
    coordinates = newCoordinates;
    /* expand grids at connected springs' coarseness levels to new coordinates if not present already */
    for (const auto &connected : springs)
        grid->onSpringPositionUpdated(oldCoordinates, newCoordinates, connected.spring->getCoarsenessLevel());

    grid->onNodePositionUpdated(oldCoordinates, newCoordinates, shared_from_this());
}

void NodeOLD::addSpring(Spring *spring, SpringConnectionType connectionType)
{
    springs.emplace_back(spring, connectionType);
}

void NodeOLD::removeSpring(Spring *spring)
{
    springs.erase(std::remove_if(springs.begin(), springs.end(),
                                 [spring](auto &connection) { return connection.spring == spring; }),
                  springs.end());
}

void NodeOLD::extend(const Coordinates &coordinates, int &cellActinUsed)
{
    ++cellActinUsed;
    ++actinUsedByNode;
    updateCoordinates(coordinates);
}

void NodeOLD::replaceSpring(const Spring *oldSpring, Spring *newSpring)
{
    const auto it = std::find_if(springs.begin(), springs.end(), [&oldSpring](const auto &connectedSpring) {
        return connectedSpring.spring == oldSpring;
    });
    it->spring = newSpring;
}

bool NodeOLD::isAtJunction() const
{
    return std::any_of(springs.begin(), springs.end(), [this](const auto &connection) {
        return connection.spring->getOppositeNode(connection.connectionType)->cellId != this->cellId;
    });
}

void NodeOLD::hookesLaw()
{
    static constexpr float springConstant = 0.01;

    if (!adhered) {
        /* Refactored version of MemAgent::calcForce function from MemAgent-Spring Model. */

        /* Calculate new force by summing neighbour vectors minused from current point (PN). Then calculate the
         * projection of S onto spring direction and get the difference. Then sum the elongated regions of each times by
         * k constant. */
        std::array<float, Dimension::nDimensions> sumDN {0};
        for (auto &connection : springs) {
            std::shared_ptr<Node> oppositeNode = connection.spring->getOppositeNode(shared_from_this());
            Coordinates PN = getCoordinates() - oppositeNode->getCoordinates();

            float length = Geometry::getLength(getCoordinates(), oppositeNode->getCoordinates());

            float denom = sqrt((PN.x * PN.x) + (PN.y * PN.y) + (PN.z * PN.z));

            /* Only apply force when spring is longer than it should be, not smaller. Membranes don't ping outwards,
             * they ruffle - should avoid 'sagging of membrane'. */
            float idealSpringLength = Spring::idealLength[static_cast<size_t>(connection.spring->getCoarsenessLevel())];
            if (length > idealSpringLength) {
                std::array<float, Dimension::nDimensions> SN {0};
                for (const auto dimension : Dimensions) {
                    SN[dimension] = idealSpringLength * (PN[dimension] / denom);
                }

                std::array<float, Dimension::nDimensions> DN {0};
                for (const auto dimension : Dimensions) {
                    DN[dimension] = PN[dimension] - SN[dimension];
                }

                for (const auto dimension : Dimensions) {
                    sumDN[dimension] += (springConstant * DN[dimension]);
                }
            }
        }

        Coordinates newCoordinates = getCoordinates();
        newCoordinates.x -= sumDN[Dimension::x];
        newCoordinates.y -= sumDN[Dimension::y];
        newCoordinates.z -= sumDN[Dimension::z];

        if (newCoordinates != getCoordinates())
            updateCoordinates(newCoordinates);
    }
}

Spring *NodeOLD::getOppositeSpringHookesLaw(const Spring *spring)
{
    assert((spring == springs[0].spring || spring == springs[1].spring) &&
           !(spring == springs[0].spring && spring == springs[1].spring));
    if (spring == springs[0].spring)
        return springs[1].spring;

    if (spring == springs[1].spring)
        return springs[0].spring;
}

Spring *NodeOLD::getOppositeSpring(const Spring *spring)
{
    const auto it = std::find_if(springs.begin(), springs.end(),
                                 [spring](const auto &connected) { return connected.spring == spring; });

    const auto retval = std::find_if(springs.begin(), springs.end(), [&it](const auto &connected) {
        return connected.connectionType == it->getOppositeConnectionType();
    });

    return (retval == springs.end()) ? nullptr : retval->spring;
}

Spring *NodeOLD::getOpposteSpring2dHack(
    Spring *spring)  // TODO: this wont work outside of 2d version as more than 2 springs can be connected in 3d version
{
    const auto it = std::find_if(springs.begin(), springs.end(),
                                 [&spring](const auto &connectedSpring) { return connectedSpring.spring == spring; });
    const auto retval = std::find_if(springs.begin(), springs.end(), [&it](const auto &connectedSpring) {
        return connectedSpring.connectionType == it->getOppositeConnectionType();
    });

    if (retval == springs.end())
        return nullptr;

    return retval->spring;
}
