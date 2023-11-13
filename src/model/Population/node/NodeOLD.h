#ifndef NODEOLD_H
#define NODEOLD_H

#include "Population/spring/SpringConnectionType.h"
#include "utility/Coordinates.h"

#include <iostream>
#include <memory>
#include <vector>

// TODO: look into focal adhesion "FA" bool in og msm
// TODO: look at inlining some of these functions
class Spring;
class Grid;

class NodeOLD : public std::enable_shared_from_this<NodeOLD>
{
public:
    struct ConnectedSpring
    {
        ConnectedSpring(Spring *spring, SpringConnectionType connectionType)
            : spring(spring),
              connectionType(connectionType)
        {
        }
        Spring *spring;
        SpringConnectionType connectionType;
        SpringConnectionType getOppositeConnectionType() const
        {
            return (connectionType == SpringConnectionType::Start) ? SpringConnectionType::End
                                                                   : SpringConnectionType::Start;
        }
    };

    NodeOLD(int cellId, int nodeId, Grid *grid, const Coordinates &coordinates);
    ~NodeOLD();

    int mNodeId;  // Jess wrote this
    int GetNodeId();

    NodeOLD(Node &&) noexcept;  // TODO: will need to update these if nodes are changed to be stack allocated
    Node &operator=(Node &&) noexcept;

    NodeOLD(const Node &node) = delete;
    Node &operator=(const Node &) = delete;

    void addSpring(Spring *spring, SpringConnectionType connectionType);
    void removeSpring(Spring *spring);

    void hookesLaw();

    /**
     * Replace a connected Spring with another Spring.
     *
     * @param oldSpring     Spring to be replaced.
     * @param newSpring     Spring to be added.
     */
    void replaceSpring(const Spring *oldSpring, Spring *newSpring);

    bool isAtJunction() const;

    void updateCoordinates(const Coordinates &coordinates);
    inline Coordinates getCoordinates() const { return coordinates; };

    // TODO: below should be private?
    int cellId;
    Coordinates coordinates;

    void resetVisibleGridSiteDeallocationTimers(int coarsenessLevel) const;

    std::vector<ConnectedSpring> springs;
    std::vector<Coordinates> getVisibleGrid(int coarsenessLevel);
    Spring *getOpposteSpring2dHack(Spring *spring);
    Spring *getOppositeSpring(const Spring *spring);
    Spring *getOppositeSpringHookesLaw(const Spring *spring);

    void extend(const Coordinates &coordinates, int &cellActinUsed);

    int actinUsedByNode = 0;

    bool adhered = true;

private:
    Grid *grid;
};

#endif  // NODE_H
