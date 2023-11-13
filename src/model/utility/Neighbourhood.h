#ifndef NEIGHBOURHOOD_H
#define NEIGHBOURHOOD_H

#include "Coordinates.h"
#include "vec3.h"

#include <array>
#include <vector>

struct Coordinates;

namespace Neighbourhood
{
    namespace FromRadius  // TODO: hack for now... just looking at more neigh + radius should be actual circumference
    {
        static constexpr int nPositions = 26;
        std::vector<Coordinates> get(const Coordinates &coordinates, int radius);
    }  // namespace FromRadius
    namespace VonNeu
    {
        static constexpr int nPositions = 6;
        std::vector<Coordinates> get(const Coordinates &coordinates);
    }  // namespace VonNeu

    namespace Moore
    {
        static constexpr int nPositions = 26;
        std::array<vec3, nPositions> get(const vec3 &middle);
    }  // namespace Moore

    namespace TwoDimensional
    {
        static constexpr int nPositions = 8;
        std::vector<Coordinates> get(const Coordinates &coordinates, int size);
    }  // namespace TwoDimensional

    namespace TwoDimensionalMoore
    {
        static constexpr int nPositions = 8;
        std::array<vec3, nPositions> get(const vec3 &middle);
    }  // namespace TwoDimensionalMoore
}  // namespace Neighbourhood

#endif  // NEIGHBOURHOOD_H
