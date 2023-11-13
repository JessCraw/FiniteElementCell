#ifndef SPRING_H
#define SPRING_H

#include "Constants.h"
#include "ISpringOwner.h"
#include "MathsFunctions.hpp"
#include "SpringConnectionType.h"
#include "proteins/CellProteins.h"
#include "proteins/Receptor.h"
#include "utility/Coordinates.h"
#include "utility/Geometry.h"

#include <iostream>
#include <list>
#include <memory>
#include <vector>

#if GRAPHICS
#include "graphics/utility/SpringGuiInfo.h"
#endif  // GRAPHICS

class NodeOLD;
class Grid;
class Cell;
struct Colour;

class Spring : public ISpringOwner
{
public:
    static int nTipNodes;
    static int maxTipNodes;

    //  Constructor
    Spring(std::shared_ptr<Node> start, std::shared_ptr<Node> end, ISpringOwner *owner, Grid *grid,
           int coarsenessLevel = Environment::Grid::nCoarsenessLevels - 1, std::list<Spring> springs = {});

    //  Destructor
    ~Spring() override;

    //  Overloaded constructor
    Spring(Spring &&other) noexcept;

    Spring &operator=(Spring &&other) noexcept;
    Spring(const Spring &other) = delete;
    Spring &operator=(const Spring &) = delete;

    bool operator==(const Spring &other);

    /*
     * Range of sensors at each coarseness level. This ensures that springs at different coarsness levels perceive the
     * same amount of VEGF.
     */
    static constexpr int vegfrSensorRange[Environment::Grid::nCoarsenessLevels] = {
        Environment::gridSquareVolumes[Environment::Grid::coarsenessMin] /
            Environment::Grid::getRangeAtCoarseness(Environment::Grid::coarsenessMin),
        Environment::gridSquareVolumes[Environment::Grid::coarsenessMax - 1] /
            Environment::Grid::getRangeAtCoarseness(Environment::Grid::coarsenessMax - 1),
        Environment::gridSquareVolumes[Environment::Grid::coarsenessMax] /
            Environment::Grid::getRangeAtCoarseness(Environment::Grid::coarsenessMax)};

    /* Ideal spring length is halfway between min (1x) at max (2x) at range */
    static constexpr std::array<float, Environment::Grid::nCoarsenessLevels> idealLength = [] {
        auto retval = decltype(Environment::Grid::rangeAtCoarseness) {};
        for (decltype(retval.size()) i = 0; i < Environment::Grid::nCoarsenessLevels; ++i) {
            retval[i] = Environment::Grid::rangeAtCoarseness[i] * 0.5;  // I cant seem to fix this so oh well
            // retval[i] = static_cast<float>(Environment::Grid::rangeAtCoarseness[i] * 0.5);
        }
        return retval;
    }();

    void startDisablingCurrentFilopodia();
    bool retractionFinished();

    void update();
    void move(int &actinUsed, const Coordinates &cellCenter);
    void signalling();
    void hookesLaw();

#ifdef UNITTEST
    void signallingWithRemeshCheck();
#endif

    /**
     * Attempt to start a filopodia extension, or continue extending if already at tip.
     *
     * @param     Cell's amount of used actin. Increased by extension distance if extends.
     * @return    Whether successfully extended.
     */
    bool attemptExtension(int &actinUsed, const Coordinates &cellCenter);

    Coordinates getMidpoint() const;
    float getLength() const;

    std::shared_ptr<Node> getOppositeNode(SpringConnectionType otherConnectionType) const;
    std::shared_ptr<Node> getOppositeNode(const std::shared_ptr<Node> &node) const;
    SpringConnectionType getNodeConnectionType(const std::shared_ptr<Node> &node) const;

    void checkRemeshing();
    void checkUnmeshing();
    void remesh();
    void unmesh();
    bool canUnmesh() const;
    bool canRemesh() const;
    inline bool remeshed() const { return !springs.empty(); }
    Node *getStartNode() const;
    Spring *getSpring();

    int unmeshTimer = 0;
    int remeshTimer = 0;

    void disconnectFromNodes();
    void changeNode(std::shared_ptr<Node> node, SpringConnectionType connectionType);
    void resetSurroundingDeallocationTimers() const;

    void getGraphics(std::vector<Coordinates> &coordinates, std::vector<Colour> &colours, unsigned int &nSprings,
                     std::vector<float> &thickness, int highlightedEdgeID,
                     const std::array<bool, Proteins::N_PROTEINS> &showProteins,
                     const std::array<bool, Filopodia::State::N> &showFilStates) const;
    void getNodeGraphics(std::vector<Coordinates> &coordinates, std::vector<Colour> &colours, unsigned int &nNodes,
                         std::vector<float> &sideLengths) const;
    void getVisibleGridGraphics(std::vector<Coordinates> &coordinates, std::vector<Colour> &colours,
                                std::vector<int> &sideLengths, unsigned int &n) const;

    void setRemeshedSpringsFilStates(Spring &spring, const std::shared_ptr<Node> &nonSharedNode) const;
#if GRAPHICS
    void getInfoForGui(std::vector<SpringGuiInfo> &retval) const;
#endif  // GRAPHICS

    int getCellId() const;

    inline bool isInFilopodiaStem() const
    {
        return (filState == Filopodia::State::STEM || filState == Filopodia::State::BASE);
    }
    inline bool isFilopodiaStem() const { return filState == Filopodia::State::STEM; }
    inline bool notInFilopodia() const { return filState == Filopodia::State::NONE; }
    inline bool inFilopodia() const { return !notInFilopodia(); }
    inline bool isFilopodiaTip() const { return filState == Filopodia::State::TIP; }
    inline bool isFilopodiaBase() const { return filState == Filopodia::State::BASE; }

    Filopodia::State filState = Filopodia::State::NONE;
    /**
     * Counter for how many filopodia this spring is a base for.
     */
    int nFilopodiaBase = 0;

    bool retracting() const;
    void incrementRetractionTimer(int &cellActinUsed);

    float length() const;

    [[nodiscard]] inline float maxLength() const
    {
        return Environment::maxEdgeLengths[static_cast<unsigned>(coarsenessLevel)];
    }

    [[nodiscard]] inline float minLength() const
    {
        return Environment::Grid::rangeAtCoarseness[static_cast<unsigned>(coarsenessLevel)];
    }

    void swapNode(const std::shared_ptr<Node> &originalNode, const std::shared_ptr<Node> &newNode);

    /**
     * Create a new Node with coordinates at the center of the Spring. Passing pointer to grid and ID of the Cell the
     * Spring is within.
     *
     * @return    shared_ptr to new Node with coordinates at midpoint of Spring.
     */
    std::shared_ptr<Node> createMidpointNode() const;

    std::shared_ptr<Node> start, end;
    ISpringOwner *owner;
    Grid *grid;
    int mID;
    // int id;

    // Jess wrote these
    int GetSpringId();
    static int idCounter;

    int retractionInactivityTimer = 0;
    int filTipRetractionTimer = 0;

    static void setFilStatesAfterSplit(Spring &originalSpring, Spring &newSpring);

    std::shared_ptr<Node> getSharedNode(const Spring &other);

    /*
     * Get shared_ptr to either or start or end if tip node. Nullptr if neither.
     *
     * @return    shared_ptr to tip node. Nullptr if no tip node.
     */
    std::shared_ptr<Node> getTipNode();

    /*
     * Get sum of coordinates of nodes covered by spring. If remeshed then call recursively.
     *
     * @param sum     Reference to Coordinates varialbe to add sum to.
     * @return        Number of nodes
     */
    int sumNodeCoordinates(Coordinates &sum) const;

    bool deleteSpringFromList(decltype(springs)::iterator &it, const CellProteins & /*parentProteins*/,
                              Grid * /*grid*/) override;
    /*
     * Get total of given Receptor
     *
     * @param receptor    Receptor to get total of
     * @return            Receptor total
     */
    double getProteinTotal(Proteins::MembraneReceptors receptor) const;

    /*
     * Get total of given Ligand.
     * If remeshed then accumulates from child springs recursively.
     *
     * @param ligand    Ligand to get total of
     * @return          Ligand total
     */
    double getProteinTotal(Proteins::MembraneLigands ligand) const;

    /*
     * Get active amount of given Receptor.
     * If remeshed then accumulates from child springs recursively.
     *
     * @param receptor    Receptor to get active of
     * @return            Receptor active amount
     */
    double getReceptorActive(Proteins::MembraneReceptors receptor) const;

    /*
     * Set total amount of given Receptor.
     * If remeshed this will update the total of all child springs with even split of new total via
     * ISpringOwner::updateSpringsReceptorTotal.
     *
     * @param receptor    Receptor to get total of.
     * @param n           Value to set Receptor total to.
     */
    void setReceptorTotal(Proteins::MembraneReceptors receptor, double n);

    /*
     * Increase Ligand total by given amount.
     * If remeshed then even split of increase is applied to all child springs recursively via
     * ISpringOwner::increaseSpringsLigandTotal
     *
     * @param ligand    Ligand to increase total of.
     * @param n         Value to set Receptor total to.
     */
    void increaseLigandTotal(Proteins::MembraneLigands ligand, double n);

    /*
     * Same as increase but decrease.
     *
     * @param ligand    Ligand to increase total of.
     * @param n         Value to set Receptor total to.
     */
    void decreaseLigandTotal(Proteins::MembraneLigands ligand, double n);

    /*
     * Increase Receptor active by given amount.
     *
     * Returns the difference between the requested increase value n and the actual amount
     * increased by. This difference can occur when the Receptor does not have enough unbound receptors left.
     *
     * If remeshed applies increase to child springs recursively. This works by looping through each spring, allocating
     * as high proportion of the total increase that is still unallocated that the child spring can hold. This is the
     * most simple method but means there will likely be an uneven distribution of increase.
     *
     * @param receptor    Receptor to increase active amount of.
     * @param n           Value to increase Receptor active by.
     * @return            Amount active actually increased by.
     */
    double increaseReceptorActive(Proteins::MembraneReceptors receptor, double n);

    /**
     * Same as increase but decreases. Floor at 0, active cannot go negative.
     *
     * @param receptor    Receptor to decrease active amount of.
     * @param n           Value to decrease Receptor active by.
     * @return            Amount active actually decreased by.
     */
    double decreaseReceptorActive(Proteins::MembraneReceptors receptor, double n);

    /**
     * Scale given receptors active amount by given factor. If remeshed applies recursively.
     *
     * @param receptor    Receptor to change active amount of.
     * @param factor      Factor by which to scale receptor's active amount.
     */
    void scaleReceptorActiveByFactor(Proteins::MembraneReceptors receptor, double factor);

    /*
     * Get unbound amount of given Receptor.
     * If remeshed then accumulates from child springs recursively.
     *
     * @param receptor    Receptor to get unbound of
     * @return            Receptor unbound amount
     */
    double getReceptorUnbound(Proteins::MembraneReceptors receptor) const;

    /*
     * Calculate angle that node should be moved at. If not yet protruded then get angle perpendicular from current
     * point and line between opposite nodes to tip node. Else return random variation on previous protrusion angle
     *
     * @param nodeToMove    Node that is going to be moved.
     * @return              Angle that node should be moved in.
     */
    float getProtrusionAngle(const std::shared_ptr<Node> &nodeToMove);

    inline int getCoarsenessLevel() const { return coarsenessLevel; }

private:
    /**
     * Calculates coordinates tip node will extend to.
     *
     * If not yet protruded, angle of extension is calculated as being perpendicular to line between the moving node's
     * two connected springs' nodes opposite the moving node. This can sometimes result in the protrusion going back
     * into the cell instead of outward. The coordinates of the cell's center are passed to the function to check if the
     * coordinates obtained by using the generated angle will cause the node to move closer towards the center compared
     * to its current position. If this is the case, then the node would be protruding inwards and so the angle is
     * reversed.
     *
     * If already protruded, angle is a randomly generated variation on the previous protrusion angle within the range
     * of +/- AceParameters::filopodiaExtensionAngleVariation.
     *
     * Coordinates are calculated by passing angle to Spring::getExtensionCoordinates.
     *
     * @param nodeToMove    The node extending.
     * @param cellCenter    Coordinates of cell center
     * @return              angle extending at and coordinates to extend to.
     */
    std::pair<float, Coordinates> getExtensionAngleAndCoordinates(const std::shared_ptr<Node> &nodeToMove,
                                                                  const Coordinates &cellCenter);

    void connectToNodes();

    /**
     * Move the Node connections from the other Spring.
     *
     * Alters the Nodes connected to the other Spring to point to this Spring.
     * @param other     The Spring to move the Node connections from.
     */
    void moveNodeConnections(const Spring &other);

    /**
     * Removes node connections from other spring and connect to this.
     *
     * @param other     The Spring to take the Node connections from.
     */
    void takeNodeConnections(Spring *other);
    /**
     * Creates and adds to back of springs container a new child spring. Also calls checkForMooreNeighbourhood on the
     * new spring so that deallocation timers of its surrounding environment are reset.
     *
     * @param start               Start node of new spring
     * @param end                 End node of new spring
     * @param nonSharedNode       Node that is not shared between the newly created remeshed spring and the other
     *                            remeshed spring that has / will be created.
     */
    void createRemeshedSpring(const std::shared_ptr<Node> &start, const std::shared_ptr<Node> &end,
                              const std::shared_ptr<Node> &nonSharedNode);
    /*
     * Decide which node can be moved when attempting to extend.
     * If in tip state, then determine tip node by checking whether opposite spring connected to start or end is also in
     * tip state.
     *
     * If start/end opposite spring is in STEM or OG_BASE filopodia and other is not then return other.
     * If both are not connected to springs currently in filopodia then return the one in highest VEGF environment.
     * If both are connected to springs in filopodia then cannot start a new filopodia so nullptr is returned.
     *
     * @return    shared_ptr to node to move.
     */
    std::shared_ptr<Node> getNodeToMove();

    /*
     * If has already protruded, then angle is stored here.
     */
    float prevProtrusionAngle = Filopodia::NO_PREV_PROTRUSION_ANGLE;

    /**
     * Check if VEGFR activation and total meet minimum requirements to be able to attempt extension.
     * Checks against values in AceParameters.
     *
     * @return    Whether VEGFR activation and total meet minimum requirements to be able to attempt extension.
     */
    bool activationMeetsExtensionRequirements() const;

    /**
     * Given a tip node's current coordinates and the angle it should move in, calculate the tip node's new coordinates.
     * Movement distance in angle restricted by Filopodia extension distance constant.
     *
     * @param start     Tip node's current coordinates.
     * @return          The coordinates the tip node's positions should be update to.
     */
    static Coordinates getExtensionCoordinates(const Coordinates &start, float angle);

    /* Determines whether moving tip node to given coordinates will cause a neighbouring spring with filopodia state
     * OG_BASE to delete. We want to check and prevent this because OG_BASE filopodia states need to be preserved as
     * indicators of where a filopodia started.
     *
     * @param tipNode         The tip node which is being moved.
     * @param coordinates     The coordinates the tip node is going to be moved to.
     * @return                True if moving tip node to given coordinates will cause neighbour spring of OG_BASE to
     *                        delete.
     */
    bool extensionWillCauseBaseStemStateChange(const std::shared_ptr<Node> &tipNode, const Coordinates &coordinates);

    /**
     * Resets filopodia related member variables to original states.
     */
    void resetFilopodiaStates();

    /**
     * Travels along filopodia via connections in opposite direction to shared node, resetting filopodia related member
     * variables.
     *
     * @param sharedNode    Node shared between current spring and next spring to have states disabled.
     * */
    void disableRestOfFilopodia(const std::shared_ptr<Node> &sharedNode);

    /**
     * Checks if retractionInactivityTimer is over Filopodia::RETRACTION_INACTIVITY_TIMER
     *
     * @returns     Whether inactivity timer is over threshold.
     */
    bool retractionInactive() const;
    /*
     * Container holding Spring instance's protein values.
     */
    friend Cell;
    friend ISpringOwner;  // TODO: move proteins to spring owner?
    CellProteins proteins;
    int coarsenessLevel = Environment::Grid::nCoarsenessLevels - 1;
};

#endif  // SPRING_H
