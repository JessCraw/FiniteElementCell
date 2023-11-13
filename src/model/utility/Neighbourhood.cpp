#include "Neighbourhood.h"

#include "utility/Coordinates.h"
#include "utility/vec3.h"

namespace Neighbourhood
{
    namespace FromRadius  // TODO: hack for now... just looking at more neigh + radius should be actual circumference
    {
        std::vector<Coordinates> get(const Coordinates &coordinates, int radius)
        {
            std::vector<Coordinates> neighbourhood;
            neighbourhood.reserve(nPositions);
            // top layer
            neighbourhood.emplace_back(coordinates.x, coordinates.y + radius, coordinates.z);
            neighbourhood.emplace_back(coordinates.x + radius, coordinates.y + radius, coordinates.z);
            neighbourhood.emplace_back(coordinates.x - radius, coordinates.y + radius, coordinates.z);
            neighbourhood.emplace_back(coordinates.x, coordinates.y + radius, coordinates.z + radius);
            neighbourhood.emplace_back(coordinates.x, coordinates.y + radius, coordinates.z - radius);
            neighbourhood.emplace_back(coordinates.x + radius, coordinates.y + radius, coordinates.z + radius);
            neighbourhood.emplace_back(coordinates.x - radius, coordinates.y + radius, coordinates.z + radius);
            neighbourhood.emplace_back(coordinates.x + radius, coordinates.y + radius, coordinates.z - radius);
            neighbourhood.emplace_back(coordinates.x - radius, coordinates.y + radius, coordinates.z - radius);
            // middle
            neighbourhood.emplace_back(coordinates.x + radius, coordinates.y, coordinates.z);
            neighbourhood.emplace_back(coordinates.x - radius, coordinates.y, coordinates.z);
            neighbourhood.emplace_back(coordinates.x, coordinates.y, coordinates.z + radius);
            neighbourhood.emplace_back(coordinates.x, coordinates.y, coordinates.z - radius);
            neighbourhood.emplace_back(coordinates.x + radius, coordinates.y, coordinates.z + radius);
            neighbourhood.emplace_back(coordinates.x - radius, coordinates.y, coordinates.z + radius);
            neighbourhood.emplace_back(coordinates.x + radius, coordinates.y, coordinates.z - radius);
            neighbourhood.emplace_back(coordinates.x - radius, coordinates.y, coordinates.z - radius);
            // bottom
            neighbourhood.emplace_back(coordinates.x, coordinates.y - radius, coordinates.z);
            neighbourhood.emplace_back(coordinates.x + radius, coordinates.y - radius, coordinates.z);
            neighbourhood.emplace_back(coordinates.x - radius, coordinates.y - radius, coordinates.z);
            neighbourhood.emplace_back(coordinates.x, coordinates.y - radius, coordinates.z + radius);
            neighbourhood.emplace_back(coordinates.x, coordinates.y - radius, coordinates.z - radius);
            neighbourhood.emplace_back(coordinates.x + radius, coordinates.y - radius, coordinates.z + radius);
            neighbourhood.emplace_back(coordinates.x - radius, coordinates.y - radius, coordinates.z + radius);
            neighbourhood.emplace_back(coordinates.x + radius, coordinates.y - radius, coordinates.z - radius);
            neighbourhood.emplace_back(coordinates.x - radius, coordinates.y - radius, coordinates.z - radius);
            return neighbourhood;
        }
    }  // namespace FromRadius
    namespace VonNeu
    {
        std::vector<Coordinates> get(const Coordinates &coordinates)
        {
            std::vector<Coordinates> neighbourhood;
            neighbourhood.reserve(nPositions);
            neighbourhood.emplace_back(coordinates.x + 1, coordinates.y, coordinates.z);
            neighbourhood.emplace_back(coordinates.x - 1, coordinates.y, coordinates.z);
            neighbourhood.emplace_back(coordinates.x, coordinates.y + 1, coordinates.z);
            neighbourhood.emplace_back(coordinates.x, coordinates.y - 1, coordinates.z);
            neighbourhood.emplace_back(coordinates.x, coordinates.y, coordinates.z + 1);
            neighbourhood.emplace_back(coordinates.x, coordinates.y, coordinates.z - 1);
            return neighbourhood;
        }
    }  // namespace VonNeu

    namespace Moore
    {
        std::array<vec3, nPositions> get(const vec3 &middle)
        {
            // top
            return {vec3(middle.x, middle.y + 1, middle.z), vec3(middle.x + 1, middle.y + 1, middle.z),
                    vec3(middle.x - 1, middle.y + 1, middle.z), vec3(middle.x, middle.y + 1, middle.z + 1),
                    vec3(middle.x, middle.y + 1, middle.z - 1), vec3(middle.x + 1, middle.y + 1, middle.z + 1),
                    vec3(middle.x - 1, middle.y + 1, middle.z + 1), vec3(middle.x + 1, middle.y + 1, middle.z - 1),
                    vec3(middle.x - 1, middle.y + 1, middle.z - 1),
                    // middle
                    vec3(middle.x + 1, middle.y, middle.z), vec3(middle.x - 1, middle.y, middle.z),
                    vec3(middle.x, middle.y, middle.z + 1), vec3(middle.x, middle.y, middle.z - 1),
                    vec3(middle.x + 1, middle.y, middle.z + 1), vec3(middle.x - 1, middle.y, middle.z + 1),
                    vec3(middle.x + 1, middle.y, middle.z - 1), vec3(middle.x - 1, middle.y, middle.z - 1),
                    // bottom
                    vec3(middle.x, middle.y - 1, middle.z), vec3(middle.x + 1, middle.y - 1, middle.z),
                    vec3(middle.x - 1, middle.y - 1, middle.z), vec3(middle.x, middle.y - 1, middle.z + 1),
                    vec3(middle.x, middle.y - 1, middle.z - 1), vec3(middle.x + 1, middle.y - 1, middle.z + 1),
                    vec3(middle.x - 1, middle.y - 1, middle.z + 1), vec3(middle.x + 1, middle.y - 1, middle.z - 1),
                    vec3(middle.x - 1, middle.y - 1, middle.z - 1)};
        }
    }  // namespace Moore

    namespace TwoDimensional
    {
        std::vector<Coordinates> get(const Coordinates &coordinates, int size)
        {
            std::vector<Coordinates> neighbourhood;
            neighbourhood.reserve(nPositions);
            neighbourhood.emplace_back(coordinates.x, coordinates.y + size, coordinates.z);
            neighbourhood.emplace_back(coordinates.x + size, coordinates.y + size, coordinates.z);
            neighbourhood.emplace_back(coordinates.x - size, coordinates.y + size, coordinates.z);
            neighbourhood.emplace_back(coordinates.x + size, coordinates.y, coordinates.z);
            neighbourhood.emplace_back(coordinates.x - size, coordinates.y, coordinates.z);
            neighbourhood.emplace_back(coordinates.x, coordinates.y - size, coordinates.z);
            neighbourhood.emplace_back(coordinates.x + size, coordinates.y - size, coordinates.z);
            neighbourhood.emplace_back(coordinates.x - size, coordinates.y - size, coordinates.z);
            return neighbourhood;
        }
    }  // namespace TwoDimensional

    namespace TwoDimensionalMoore
    {
        std::array<vec3, nPositions> get(const vec3 &middle)
        {
            return {vec3(middle[0], middle[1] + 1, middle[2]),     vec3(middle[0] + 1, middle[1] + 1, middle[2]),
                    vec3(middle[0] - 1, middle[1] + 1, middle[2]), vec3(middle[0] + 1, middle[1], middle[2]),
                    vec3(middle[0] - 1, middle[1], middle[2]),     vec3(middle[0], middle[1] - 1, middle[2]),
                    vec3(middle[0] + 1, middle[1] - 1, middle[2]), vec3(middle[0] - 1, middle[1] - 1, middle[2])};
        }
    }  // namespace TwoDimensionalMoore
}  // namespace Neighbourhood
