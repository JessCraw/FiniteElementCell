#ifndef CONSTANTS_H
#define CONSTANTS_H

// #include "utility/AceChecks.h"
// #include "utility/Colour.h"
// #include "utility/Coordinates.h"

#include <array>
#include <cmath>
// #include <gcem.hpp>
#include <string>

extern int timestep;

namespace Graphics
{
    namespace Screen
    {
        static constexpr int Width = 800;
        static constexpr int Height = 800;
    }  // namespace Screen

    static const Colour highlightedEdgeColour = {0.1F, 1, 0.1F, 1};
    static const Colour highlightedGridSquareColour = highlightedEdgeColour;
}  // namespace Graphics
//

namespace Proteins
{
    /*
     * Receptors found in membrane.
     */
    enum class MembraneReceptors
    {
        VEGFR,
        NOTCH,
        N
    };

    /*
     * Ligands found in membrane.
     */
    enum class MembraneLigands
    {
        DLL4,
        N
    };

    /*
     * Ligands found in environment.
     */
    enum class EnvironmentLigands
    {
        VEGF,
        N
    };

    enum class ReceptorAttributes
    {
        TOTAL,
        ACTIVE,
        UNBOUND,
        N
    };

    enum class LigandAttributes
    {
        TOTAL,
        N
    };

    static constexpr int N_CELL_RECEPTORS =
        static_cast<int>(MembraneReceptors::N) * static_cast<int>(ReceptorAttributes::N);

    static constexpr int N_CELL_LIGANDS = static_cast<int>(MembraneLigands::N) * static_cast<int>(LigandAttributes::N);

    static constexpr int N_CELL_PROTEINS = N_CELL_RECEPTORS + N_CELL_LIGANDS;

    static constexpr int N_ENVIRONMENT_PROTEINS =
        static_cast<int>(EnvironmentLigands::N) * static_cast<int>(LigandAttributes::N);

    static constexpr int N_PROTEINS = N_CELL_PROTEINS + N_ENVIRONMENT_PROTEINS;

    /* Index utility functions to index a container of size N_PROTEINS. E.g: a vector holding doubles for each proteins
     * total / conc */
    static constexpr int getProteinIndex(MembraneReceptors receptor, ReceptorAttributes attribute)
    {
        return static_cast<int>(static_cast<unsigned long>(receptor) *
                                    static_cast<unsigned long>(ReceptorAttributes::N) +
                                static_cast<unsigned long>(attribute));
    }

    static constexpr int getProteinIndex(MembraneLigands ligand, LigandAttributes attribute)
    {
        return static_cast<int>(N_CELL_RECEPTORS +
                                static_cast<unsigned long>(ligand) * static_cast<unsigned long>(LigandAttributes::N) +
                                static_cast<unsigned long>(attribute));
    }

    static constexpr int getProteinIndex(EnvironmentLigands ligand, LigandAttributes attribute)
    {
        return static_cast<int>(N_CELL_RECEPTORS + N_ENVIRONMENT_PROTEINS +
                                static_cast<unsigned long>(ligand) * static_cast<unsigned long>(LigandAttributes::N) +
                                static_cast<unsigned long>(attribute));
    }

    enum Index
    {
        VEGFR_TOTAL = getProteinIndex(MembraneReceptors::VEGFR, ReceptorAttributes::TOTAL),
        VEGFR_UNBOUND = getProteinIndex(MembraneReceptors::VEGFR, ReceptorAttributes::UNBOUND),
        VEGFR_ACTIVE = getProteinIndex(MembraneReceptors::VEGFR, ReceptorAttributes::ACTIVE),
        VEGF_TOTAL = getProteinIndex(EnvironmentLigands::VEGF, LigandAttributes::TOTAL),
        NOTCH_TOTAL = getProteinIndex(MembraneReceptors::NOTCH, ReceptorAttributes::TOTAL),
        NOTCH_UNBOUND = getProteinIndex(MembraneReceptors::NOTCH, ReceptorAttributes::UNBOUND),
        NOTCH_ACTIVE = getProteinIndex(MembraneReceptors::NOTCH, ReceptorAttributes::ACTIVE),
        DLL4_TOTAL = getProteinIndex(MembraneLigands::DLL4, LigandAttributes::TOTAL),
    };

    // static std::array<std::string, N_CELL_PROTEINS> GetCellProteinNames() // Jess turned this off because it is
    // throwing a warning because it is unused
    // {
    //     std::array<std::string, N_CELL_PROTEINS> proteinNames;
    //     proteinNames[Index::VEGFR_TOTAL] = "VEGFR Total";
    //     proteinNames[Index::VEGFR_UNBOUND] = "VEGFR Unbound";
    //     proteinNames[Index::VEGFR_ACTIVE] = "VEGFR Active";
    //     proteinNames[Index::NOTCH_TOTAL] = "Notch Total";
    //     proteinNames[Index::NOTCH_UNBOUND] = "Notch Unbound";
    //     proteinNames[Index::NOTCH_ACTIVE] = "Notch Active";
    //     proteinNames[Index::DLL4_TOTAL] = "DLL4 Total";
    //     return proteinNames;
    // }

    /* Generate array of membrane receptors at compile time. Needed for range based for loop. */
    static constexpr std::array<MembraneReceptors, static_cast<unsigned>(MembraneReceptors::N)> MembraneReceptorsArray =
        [] {
            auto retval = decltype(MembraneReceptorsArray) {};
            for (decltype(retval.size()) i = 0; i < static_cast<unsigned>(MembraneReceptors::N); ++i) {
                retval[i] = static_cast<enum MembraneReceptors>(i);
            }
            return retval;
        }();

    /* Generate array of membrane receptors at compile time. Needed for range based for loop. */
    static constexpr std::array<MembraneLigands, static_cast<unsigned>(MembraneLigands::N)> MembraneLigandsArray = [] {
        auto retval = decltype(MembraneLigandsArray) {};
        for (decltype(retval.size()) i = 0; i < static_cast<unsigned>(MembraneLigands::N); ++i) {
            retval[i] = static_cast<enum MembraneLigands>(i);
        }
        return retval;
    }();

    /*
     * Generate array of translocation times at compile time. Allows translocation times to be set explicitly using
     * enum, instead of having to keep correct enum order in array declaration.
     */
    static constexpr std::array<int, Proteins::MembraneReceptorsArray.size()> ReceptorTranslocationTimes = [] {
        auto retval = decltype(ReceptorTranslocationTimes) {0};
        retval[static_cast<unsigned long>(Proteins::MembraneReceptors::VEGFR)] = 0;
        retval[static_cast<unsigned long>(Proteins::MembraneReceptors::NOTCH)] = 0;
        return retval;
    }();

}  // namespace Proteins
//

namespace Environment
{
    namespace Grid
    {
        static constexpr int nCoarsenessLevels = 3;
        static constexpr int coarsenessMax = nCoarsenessLevels - 1;
        static constexpr int coarsenessMin = 0;
        static constexpr int initialScale = 1;
        static constexpr int coarsenessScale = 2;
        static constexpr int coarsenessFineUnmeshTime = 150;
        static constexpr int coarsenessMediumUnmeshTime = 400;
        static constexpr int unmeshTime = 150;

        // The width of the boundary/ghost cell layer.
        static constexpr int widthOfBoundaryLayer = 1;
        // The number of boundary/ghost positions in a LocationsContainer per dimension (One at each end).
        static constexpr int numBoundaryPositionsPerDim = 2 * widthOfBoundaryLayer;

        static constexpr int getRangeAtCoarseness(int coarsenessLevel)
        {
            return initialScale * static_cast<int>(gcem::pow(coarsenessScale, coarsenessLevel));
        }

        /* Generate ranges at all coarseness scales at compile time */
        static constexpr std::array<float, nCoarsenessLevels> rangeAtCoarseness = [] {
            auto retval = decltype(rangeAtCoarseness) {};
            for (decltype(retval.size()) i = 0; i < nCoarsenessLevels; ++i) {
                retval[i] = initialScale * static_cast<float>(gcem::pow(coarsenessScale, static_cast<int>(i)));
            }
            return retval;
        }();

        static constexpr float rangeAtMaxCoarseness = rangeAtCoarseness[coarsenessMax];

        constexpr bool coarsenessScaleIsPowerOfTwo()
        {
            static_assert(coarsenessScale > 1, "Coarseness Scale must be greater than one.");
            return Checks::IsPowerOfTwo(coarsenessScale);
        }
    }  // namespace Grid

    /* Generate max edge lengths at all coarseness scales at compile time */
    static constexpr float maxLengthMultiplier = 2;
    static constexpr std::array<float, Grid::nCoarsenessLevels> maxEdgeLengths = [] {
        auto retval = decltype(maxEdgeLengths) {};
        for (decltype(retval.size()) i = 0; i < Grid::nCoarsenessLevels; ++i) {
            retval[i] = Grid::rangeAtCoarseness[i] * maxLengthMultiplier;
        }
        return retval;
    }();

    static constexpr unsigned nXLayers = 34;
    static constexpr unsigned nYLayers = 32;
    static constexpr unsigned nZLayers = 6;

    static constexpr int zMiddle = (Grid::getRangeAtCoarseness(Grid::coarsenessMax) * nZLayers) / 2;

    static constexpr int Width = static_cast<int>(Grid::rangeAtCoarseness[Grid::coarsenessMax]) * nXLayers;
    static constexpr int Height = static_cast<int>(Grid::rangeAtCoarseness[Grid::coarsenessMax]) * nYLayers;
    static constexpr int Depth = static_cast<int>(Grid::rangeAtCoarseness[Grid::coarsenessMax]) * nZLayers;

    static constexpr int gridSquareVolumes[Grid::nCoarsenessLevels] = {
        static_cast<int>(gcem::pow(Environment::Grid::getRangeAtCoarseness(Environment::Grid::coarsenessMin), 3)),
        static_cast<int>(gcem::pow(Environment::Grid::getRangeAtCoarseness(Environment::Grid::coarsenessMax - 1), 3)),
        static_cast<int>(gcem::pow(Environment::Grid::getRangeAtCoarseness(Environment::Grid::coarsenessMax), 3))};

    enum GradientType
    {
        Steady,
        Linear
    };

    static constexpr float VConcST = 0.04;

    namespace Diffusion
    {

        static constexpr float diffusionRate = 1.02;  // the diffusion rate constant for the environment - this needs to
                                                      // be set so it is accurate for the actual grid spacing of the sim
                                                      // for finest coarsenessLevel

        constexpr float numberOfSubcycles(int coarsenessLevel)
        {
            return static_cast<float>(gcem::pow(Grid::coarsenessScale, Grid::coarsenessMax - coarsenessLevel));
        }

        constexpr float getMaxTimestep()
        {
            float minDeltaX = Grid::getRangeAtCoarseness(Grid::coarsenessMin);
            float maxNumberOfSubCycles = numberOfSubcycles(Grid::coarsenessMin);

            return maxNumberOfSubCycles * (minDeltaX * minDeltaX) / (8 * diffusionRate);
        }

        inline bool checkStability(float deltaX, float deltaT, float diffusionRate)
        {
            return 8 * diffusionRate * deltaT / (deltaX * deltaX) <= 1;
        }

        constexpr float simulationDeltaT = getMaxTimestep();
    }  // namespace Diffusion

}  // namespace Environment

namespace Filopodia
{
    enum State
    {
        TIP,
        STEM,
        BASE,
        NONE,
        N
    };

    static constexpr float NO_PREV_PROTRUSION_ANGLE = 361;

    static constexpr float EXTENSION_DISTANCE = 0.1;

    // static const Coordinates NO_EXTENSION_START = {-1, -1, -1};

    static constexpr double RETRACTION_DISTANCE = 0.2;

    static constexpr float RETRACTION_INACTIVITY_TIMER = 50;
}  // namespace Filopodia

enum Dimension
{
    x,
    y,
    z,
    nDimensions
};

static constexpr std::array<unsigned, Dimension::nDimensions> Dimensions = {x, y, z};

enum Colors
{
    r,
    g,
    b,
    a,
    nColorVals
};

namespace Shapes
{
    namespace Line
    {
        static constexpr unsigned int nVertices = 2 * (Dimension::nDimensions + Colors::nColorVals);
    }

    namespace Triangle
    {
        static constexpr unsigned int points = 3;
    }

    namespace Cube
    {
        enum Sides
        {
            Top,
            Bottom,
            Left,
            Right,
            Front,
            Back,
            nSides
        };

        enum Corners
        {
            frontBottomLeft,
            frontBottomRight,
            frontTopRight,
            frontTopLeft,

            backBottomLeft,
            backBottomRight,
            backTopRight,
            backTopLeft,

            nCorners
        };
        inline Shapes::Cube::Corners operator++(Shapes::Cube::Corners &corner, int)
        {
            if (corner == Shapes::Cube::Corners::nCorners)
                return corner = Shapes::Cube::Corners::frontBottomLeft;

            int cornerInt = static_cast<int>(corner);
            return corner = static_cast<Shapes::Cube::Corners>(++cornerInt);
        }

        namespace Normals
        {
            enum Corners
            {
                frontBottomLeftFront,
                frontBottomLeftBottom,
                frontBottomLeftLeft,

                frontBottomRightFront,
                frontBottomRightBottom,
                frontBottomRightRight,

                frontTopRightFront,
                frontTopRightTop,
                frontTopRightRight,

                frontTopLeftFront,
                frontTopLeftTop,
                frontTopLeftLeft,

                backBottomLeftBack,
                backBottomLeftBottom,
                backBottomLeftLeft,

                backBottomRightBack,
                backBottomRightBottom,
                backBottomRightRight,

                backTopRightBack,
                backTopRightTop,
                backTopRightRight,

                backTopLeftBack,
                backTopLeftTop,
                backTopLeftLeft,

                nCornersWithNormals
            };
            inline Shapes::Cube::Normals::Corners operator++(Shapes::Cube::Normals::Corners &corner, int)
            {
                if (corner == Shapes::Cube::Normals::Corners::nCornersWithNormals)
                    return corner = Shapes::Cube::Normals::Corners::frontBottomLeftFront;

                int cornerInt = static_cast<int>(corner);
                return corner = static_cast<Shapes::Cube::Normals::Corners>(++cornerInt);
            }

            static constexpr unsigned int nVertices =
                Normals::Corners::nCornersWithNormals *
                (Dimension::nDimensions + Colors::nColorVals + Dimension::nDimensions);
        }  // namespace Normals

        static constexpr unsigned int faces = 6;
        static constexpr unsigned int pointsPerFace = Triangle::points * 2;

        static constexpr unsigned int nVertices = Corners::nCorners * (Dimension::nDimensions + Colors::nColorVals);
        static constexpr unsigned int nIndices = faces * pointsPerFace;

        namespace WireFrame
        {
            static constexpr unsigned int pointsPerFace = 4;
            static constexpr unsigned int nIndices = faces * pointsPerFace * 2;
        }  // namespace WireFrame

    }  // namespace Cube
}  // namespace Shapes

static constexpr Shapes::Cube::Corners
    CUBE_NORMALS_CORNER_TO_CUBE_CORNER[Shapes::Cube::Normals::Corners::nCornersWithNormals] = {
        Shapes::Cube::Corners::frontBottomLeft,  Shapes::Cube::Corners::frontBottomLeft,
        Shapes::Cube::Corners::frontBottomLeft,

        Shapes::Cube::Corners::frontBottomRight, Shapes::Cube::Corners::frontBottomRight,
        Shapes::Cube::Corners::frontBottomRight,

        Shapes::Cube::Corners::frontTopRight,    Shapes::Cube::Corners::frontTopRight,
        Shapes::Cube::Corners::frontTopRight,

        Shapes::Cube::Corners::frontTopLeft,     Shapes::Cube::Corners::frontTopLeft,
        Shapes::Cube::Corners::frontTopLeft,

        Shapes::Cube::Corners::backBottomLeft,   Shapes::Cube::Corners::backBottomLeft,
        Shapes::Cube::Corners::backBottomLeft,

        Shapes::Cube::Corners::backBottomRight,  Shapes::Cube::Corners::backBottomRight,
        Shapes::Cube::Corners::backBottomRight,

        Shapes::Cube::Corners::backTopRight,     Shapes::Cube::Corners::backTopRight,
        Shapes::Cube::Corners::backTopRight,

        Shapes::Cube::Corners::backTopLeft,      Shapes::Cube::Corners::backTopLeft,
        Shapes::Cube::Corners::backTopLeft,
};

static constexpr Shapes::Cube::Sides CUBE_CORNERS_TO_NORMALS_MAP[Shapes::Cube::Normals::Corners::nCornersWithNormals] =
    {
        Shapes::Cube::Sides::Front, Shapes::Cube::Sides::Bottom, Shapes::Cube::Sides::Left,

        Shapes::Cube::Sides::Front, Shapes::Cube::Sides::Bottom, Shapes::Cube::Sides::Right,

        Shapes::Cube::Sides::Front, Shapes::Cube::Sides::Top,    Shapes::Cube::Sides::Right,

        Shapes::Cube::Sides::Front, Shapes::Cube::Sides::Top,    Shapes::Cube::Sides::Left,

        Shapes::Cube::Sides::Back,  Shapes::Cube::Sides::Bottom, Shapes::Cube::Sides::Left,

        Shapes::Cube::Sides::Back,  Shapes::Cube::Sides::Bottom, Shapes::Cube::Sides::Right,

        Shapes::Cube::Sides::Back,  Shapes::Cube::Sides::Top,    Shapes::Cube::Sides::Right,

        Shapes::Cube::Sides::Back,  Shapes::Cube::Sides::Top,    Shapes::Cube::Sides::Left,
};

// TODO: these need to adapt to n coarseness levels
static const Colour COARSENESS_COLOUR_MAP_SPRINGS[3] = {
    {0.4f, 0.85f, 0.11f, 1.0f}, {1.0f, 0.88f, 0.21f, 1.0f}, {0.98f, 0.2f, 0.96f, 1.0f}};

static const Colour COARSENESS_COLOUR_MAP[3] = {
    {0.4f, 0.55f, 0.11f, 1.0f}, {0.8f, 0.78f, 0.21f, 1.0f}, {0.88f, 0.6f, 0.96f, 1.0f}};

static const double COARSENESS_CUBE_SIZE_MAP[3] = {0.4, 0.75, 1};

static const Colour COARSENESS_COLOUR_MAP_NODES[3] = {
    {0.4, 0.96f, 0.3f, 1.0f}, {0.18f, 0.96f, 0.6f, 1.0f}, {0.18f, 0.6f, 0.96f, 1.0f}};

static const double COARSENESS_SPRING_THICKNESS_MAP[3] = {0.09, 0.18, 0.3};

static const Colour VEGF_COLOUR = {1.f, 0.5, 0.f, 1.0f};

// original:
// static const Colour COARSENESS_COLOUR_MAP[3] = {
//     { 0.4f, 0.85f, 0.11f, 1.0f },
//     { 1.0f, 0.88f, 0.21f, 1.0f },
//     { 0.88f, 0.6f, 0.96f, 1.0f }
// };

#endif  // CONSTANTS_H
