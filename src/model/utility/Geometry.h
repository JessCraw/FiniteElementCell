#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "AceChecks.h"
#include "Constants.h"
#include "Coordinates.h"
#include "vec3.h"

#include <math.h>

#include <cassert>
#include <vector>

namespace Geometry
{
    inline float getLength(const Coordinates &start, const Coordinates &end)
    {
        // return sqrt(pow(start.x - end.x, 2) + pow(start.y - end.y, 2) + pow(start.z - end.z, 2));
        return static_cast<float>(sqrt(pow(start.x - end.x, 2) + pow(start.y - end.y, 2) + pow(start.z - end.z, 2)));
    }

    inline std::pair<Coordinates, Coordinates> getIncreasingVector(const Coordinates &start, const Coordinates &end)
    {
        return {{std::min(start.x, end.x), std::min(start.y, end.y), std::min(start.z, end.z)},
                {std::max(start.x, end.x), std::max(start.y, end.y), std::max(start.z, end.z)}};
    }

    inline bool interesect(const Coordinates &aMin, const Coordinates &aMax, const Coordinates &bMin,
                           const Coordinates &bMax)
    {
        return (aMin.x <= bMax.x && aMin.y <= bMax.y && aMin.z <= bMax.z) &&
               (aMax.x >= bMin.x && aMax.y >= bMin.y && aMax.z >= bMin.z);
    }

    inline bool interesect(const vec3 &aMin, const vec3 &aMax, const vec3 &bMin, const vec3 &bMax)
    {
        return (aMin.x <= bMax.x && aMin.y <= bMax.y && aMin.z <= bMax.z) &&
               (aMax.x >= bMin.x && aMax.y >= bMin.y && aMax.z >= bMin.z);
    }

    inline vec3 getOffsetDifference(const vec3 &offset, const vec3 &otherOffset, int range)
    {
        return ((offset - otherOffset) / range) * -1;
    }

    inline int mapOldLinearPosToNew(int pos, const vec3 &sizeOld, const vec3 &sizeNew, const vec3 &offsetDifferece)
    {
        int q = pos / sizeOld.z;
        int k = pos % sizeOld.z;

        int i = q / sizeOld.y;
        int j = q % sizeOld.y;

        return (i - offsetDifferece.x) * sizeNew.y * sizeNew.z + (j - offsetDifferece.y) * sizeNew.z +
               (k - offsetDifferece.z);
    }

    inline bool onLineBetween(const Coordinates &a, const Coordinates &b, const Coordinates &c, float tolerance = 0)
    {
        float ac = Geometry::getLength(a, c);
        float bc = Geometry::getLength(b, c);
        float ab = Geometry::getLength(a, b);
        return ac + bc <= ab * tolerance;
    }

    /**
     * Round int 'n' down to the closest multiple of int 'm'. Notes: -  passing 0 as a value of 'm' wlll simply return
     * 'n'.
     *  -  negative value of 'n' are rounded down (away from 0).
     *
     * @param n   The int to be rounded down to nearest multiple.
     * @param m   The multiple to be rounded down to.
     * @return    The result of the rounding.
     */
    inline int roundDownToMultiple(int n, int m)
    {
        if (m == 0) {
            return n;
        }

        return n >= 0 ? (n / m) * m : ((n - m + 1) / m) * m;
    }

    /**
     * Round int 'n' up to the closest multiple of int 'm'.
     *  -  passing 0 as a value of 'm' wlll simply return 'n'.
     *  -  negative value of 'n' are rounded up (towards 0).
     *
     * @param n   The number to be rounded up to nearest multiple.
     * @param m   The multiple to be rounded up to.
     * @return    The result of the rounding.
     */
    inline int roundUpToMultiple(int n, int m)
    {
        if (m == 0) {
            return n;
        }

        return n >= 0 ? ((n + m - 1) / m) * m : (n / m) * m;
    }

    /**
     * Round int 'n' up to the closest multiple of int 'm'.
     * Note: Multiple 'm' is required to be a power of two, allowing a more efficient implementation that does not use
     * more expensive division or multiplication operations.
     *
     * @param n   The number to be rounded up to nearest multiple.
     * @param m   The multiple to be rounded up to.
     * @return    The result of the rounding.
     */
    inline int roundUpToPowerOfTwoMultiple(int n, int m)
    {
        assert(Checks::IsPowerOfTwo(m));
        return (n + m - 1) & -m;
    }

    /**
     * Round a number of Grid Sites 'n' up to (the smallest) multiple of parent Grid Sites.
     *
     * @param n     The number of grid sites at coarsenessLevel x.
     * @return      The result of the rounding (a multiple of Grid sites at coarsenessLevel x + 1).
     */
    inline int roundUpToMultipleOfParentGridSites(int n)
    {
        if constexpr (Environment::Grid::coarsenessScaleIsPowerOfTwo()) {
            return roundUpToPowerOfTwoMultiple(n, Environment::Grid::coarsenessScale);
        } else {
            return roundUpToMultiple(n, Environment::Grid::coarsenessScale);
        }
    }

    std::vector<float> equationOfLineThroughTwoPoints(float x1, float y1, float x2, float y2);

    Coordinates calcPointOnLineThroughExternalPointGivingPerpendicularLine(Coordinates C, Coordinates S,
                                                                           Coordinates En);

    float reverseAngle(float angle);

    bool doLinesIntersect(const Coordinates &line1Start, const Coordinates &line1End, const Coordinates &line2Start,
                          const Coordinates &line2End);
}  // namespace Geometry

#endif  // GEOMETRY_H
