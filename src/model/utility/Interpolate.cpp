#include "Interpolate.h"

#include "Constants.h"
#include "environment/Location.h"
#include "environment/LocationsContainer.h"
#include "gcem_incl/abs.hpp"
#include "utility/vec3.h"

#include <array>
#include <utility>

namespace Interpolate
{
    constexpr int coefficientArraySize = Environment::Grid::coarsenessScale * Environment::Grid::coarsenessScale *
                                         Environment::Grid::coarsenessScale * 8;
    void interpolateAndSetChildRefinedLocations(const LocationsContainer &parent, LocationsContainer &child,
                                                const vec3 &parentIndex, const vec3 &pos);
    double getRefinedChildValue(const LocationsContainer &parent, const vec3 &parentIndex,
                                const vec3 &refinedChildIndex);
    void interpolateAllBoundaries(const LocationsContainer &parent, LocationsContainer &child,
                                  const std::array<std::pair<int, int>, Dimension::nDimensions> &boundaries,
                                  int boundary);
    void interpolateBoundariesAndSetChildBoundayLocations(const LocationsContainer &parent, LocationsContainer &child,
                                                          const vec3 &parentIndex, const vec3 &pos, int boundaryAxis,
                                                          int boundaryLocation);

    inline constexpr int linearIndexInterpolatedArray(int x, int y, int z)
    {
        return x * Environment::Grid::coarsenessScale * Environment::Grid::coarsenessScale +
               y * Environment::Grid::coarsenessScale + z;
    }

    inline constexpr int linearIndexCoefficientArray(int x, int y, int z, int l)
    {
        return x * Environment::Grid::coarsenessScale * Environment::Grid::coarsenessScale * 8 +
               y * Environment::Grid::coarsenessScale * 8 + z * 8 + l;
    }

    inline constexpr int linearIndexDataArray(int i, int j, int k) { return i * 4 + j * 2 + k; }

    static constexpr std::array<double, coefficientArraySize> getCoefficients()
    {
        std::array<double, coefficientArraySize> result {0};

        for (int x = 0; x < Environment::Grid::coarsenessScale; x++) {
            for (int y = 0; y < Environment::Grid::coarsenessScale; y++) {
                for (int z = 0; z < Environment::Grid::coarsenessScale; z++) {
                    double xx =
                        ((double)x) /
                        Environment::Grid::coarsenessScale;  // this is the offset from the center point of the
                                                             // interpolation - i.e. 0,0,0 lies on top of old point
                    double yy = ((double)y) / Environment::Grid::coarsenessScale;
                    double zz = ((double)z) / Environment::Grid::coarsenessScale;

                    // surrounding datapoints just addressing in same order -> i = x, j = y, k = z
                    for (int i = 0; i < 2; i++) {
                        for (int j = 0; j < 2; j++) {
                            for (int k = 0; k < 2; k++) {
                                // calculate the distance from the i,j,kth neighbour, discard if is 1 or greater in that
                                // axis
                                double distX = gcem::abs(i - xx);  // distance to center point, then distance from
                                                                   // center point to interpolation point
                                double distY = gcem::abs(j - yy);
                                double distZ = gcem::abs(k - zz);

                                double val = (1 - distX) * (1 - distY) * (1 - distZ);

                                int l = i * 2 * 2 + j * 2 + k;
                                result[linearIndexCoefficientArray(x, y, z, l)] = val;
                            }
                        }
                    }
                }
            }
        }

        return result;
    }

    static constexpr std::array<double, coefficientArraySize> coeffArr = getCoefficients();

    void interpolateChildFromParent(const LocationsContainer &parent, LocationsContainer &child)
    {
        // These are the maximum extent of the child in the parents grid indices
        vec3 childMax = child.positions / Environment::Grid::coarsenessScale;
        vec3 offset = child.offsetFromParent / Environment::Grid::coarsenessScale;

        // set up grid by interpolating each coarsenessScale^3 set of points from its neighbourhood
        // this is a long way around but will do and then refactor.
        // could do the interpolation in a big block as oppose to chunked
        for (int i = offset.x; i < childMax.x + offset.x; i++)
            for (int j = offset.y; j < childMax.y + offset.y; j++)
                for (int k = offset.z; k < childMax.z + offset.z; k++)
                    if (!child.locationAtSetVegf((i - offset.x) * Environment::Grid::coarsenessScale,
                                                 (j - offset.y) * Environment::Grid::coarsenessScale,
                                                 (k - offset.z) * Environment::Grid::coarsenessScale))
                        interpolateAndSetChildRefinedLocations(parent, child, {i, j, k},
                                                               {i - offset.x, j - offset.y, k - offset.z});
    }

    void interpolateAndSetChildRefinedLocations(const LocationsContainer &parent, LocationsContainer &child,
                                                const vec3 &parentIndex, const vec3 &pos)
    {
        for (int x = 0; x < Environment::Grid::coarsenessScale; x++)
            for (int y = 0; y < Environment::Grid::coarsenessScale; y++)
                for (int z = 0; z < Environment::Grid::coarsenessScale; z++) {
                    vec3 position = {x + pos.x * Environment::Grid::coarsenessScale,
                                     y + pos.y * Environment::Grid::coarsenessScale,
                                     z + pos.z * Environment::Grid::coarsenessScale};
                    child.locationAt(position).vegf.setConc(getRefinedChildValue(parent, parentIndex, {x, y, z}),
                                                            child.getVolume());
                    child.setVegfFlag(position.x, position.y, position.z, true);
                }
    }

    double getRefinedChildValue(const LocationsContainer &parent, const vec3 &parentIndex,
                                const vec3 &refinedChildIndex)
    {
        // the 2-2-2 neighbourhood around the point we are interpolating
        double val = 0;
        for (int x = 0; x < 2; x++) {
            for (int y = 0; y < 2; y++) {
                for (int z = 0; z < 2; z++) {
                    auto parentLocVegfConc = parent.locationAt(parentIndex.x + x, parentIndex.y + y, parentIndex.z + z)
                                                 .vegf.getConc(parent.getVolume());
                    auto coeff = coeffArr[linearIndexCoefficientArray(
                        refinedChildIndex.x, refinedChildIndex.y, refinedChildIndex.z, linearIndexDataArray(x, y, z))];
                    val += parentLocVegfConc * coeff;
                }
            }
        }

        return val;
    }

    // we know we are always interpolating at fixed positions so we can pre-calculate constants to save computation
    // this function initialises all child data from the parent by interpolation
    void interpolateChildBoundaries(const LocationsContainer &parent, LocationsContainer &child)
    {
        std::array<std::pair<int, int>, Dimension::nDimensions> boundaries;
        for (const auto &dimension : Dimensions)
            boundaries[dimension] = {(int)child.offsetFromParent[dimension] / Environment::Grid::coarsenessScale,
                                     (int)(child.positions[dimension] + child.offsetFromParent[dimension]) /
                                             Environment::Grid::coarsenessScale -
                                         1};

        // interpolate left and right
        for (auto dimension : Dimensions)
            interpolateAllBoundaries(parent, child, boundaries, dimension);
    }

    void interpolateAllBoundaries(const LocationsContainer &parent, LocationsContainer &child,
                                  const std::array<std::pair<int, int>, Dimension::nDimensions> &boundaries,
                                  int dimension)
    {
        vec3 increments = {{1, 1, 1}, dimension, boundaries[dimension].second - boundaries[dimension].first};

        for (int x = boundaries[Dimension::x].first; x < boundaries[Dimension::x].second + 1;
             x += increments[Dimension::x])
            for (int y = boundaries[Dimension::y].first; y < boundaries[Dimension::y].second + 1;
                 y += increments[Dimension::y])
                for (int z = boundaries[Dimension::z].first; z < boundaries[Dimension::z].second + 1;
                     z += increments[Dimension::z]) {
                    vec3 position = {x, y, z};
                    vec3 boundaryPosition {(int)child.offsetFromParent.x / Environment::Grid::coarsenessScale,
                                           (int)child.offsetFromParent.y / Environment::Grid::coarsenessScale,
                                           (int)child.offsetFromParent.z / Environment::Grid::coarsenessScale};
                    int bv = (position[dimension] - boundaryPosition[dimension]) * Environment::Grid::coarsenessScale;

                    interpolateBoundariesAndSetChildBoundayLocations(parent, child, position,
                                                                     position - boundaryPosition, dimension, bv);
                }
    }

    void interpolateBoundariesAndSetChildBoundayLocations(const LocationsContainer &parent, LocationsContainer &child,
                                                          const vec3 &parentIndex, const vec3 &pos, int boundaryAxis,
                                                          int boundaryLocation)
    {
        std::array<int, Dimension::nDimensions> dimMin;
        dimMin.fill(0);
        std::array<int, Dimension::nDimensions> dimMax;
        dimMax.fill(Environment::Grid::coarsenessScale);

        // Update only the boundaries, not all the intermediate child cells between parent cells
        dimMin[boundaryAxis] = (boundaryLocation == 0) ? boundaryLocation : Environment::Grid::coarsenessScale - 1;
        dimMax[boundaryAxis] = dimMin[boundaryAxis] + 1;

        for (int x = dimMin[Dimension::x]; x < dimMax[Dimension::x]; x++)
            for (int y = dimMin[Dimension::y]; y < dimMax[Dimension::y]; y++)
                for (int z = dimMin[Dimension::z]; z < dimMax[Dimension::z]; z++)
                    child
                        .locationAt(x + pos.x * Environment::Grid::coarsenessScale,
                                    y + pos.y * Environment::Grid::coarsenessScale,
                                    z + pos.z * Environment::Grid::coarsenessScale)
                        .vegf.setConc(getRefinedChildValue(parent, parentIndex, {x, y, z}), child.getVolume());
    }
}  // namespace Interpolate
