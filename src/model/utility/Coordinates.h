#ifndef COORDINATES_H
#define COORDINATES_H

// #include "Constants.h"

#include <iostream>

struct Coordinates
{
    // COORDINATES ARE NOW UNIVERSAL AND ALWAYS ARE COUNTED IN THE FINEST LOCATION
    Coordinates(float x, float y, float z)
        : x(x),
          y(y),
          z(z)
    {
    }

    bool operator==(const Coordinates &coordinates) const
    {
        return x == coordinates.x && y == coordinates.y && z == coordinates.z;
    }

    bool operator!=(const Coordinates &coordinates) const { return !(*this == coordinates); }

    Coordinates &operator+=(const Coordinates &coordinates)
    {
        x += coordinates.x;
        y += coordinates.y;
        z += coordinates.z;
        return *this;
    }

    Coordinates &operator-=(const Coordinates &coordinates)
    {
        x -= coordinates.x;
        y -= coordinates.y;
        z -= coordinates.z;
        return *this;
    }

    template <class T>
    Coordinates operator/(T val) const
    {
        // TODO: change this back... done for the onLineRemesh func
        // return {(float)((int)(x / val)), (float)((int)(y / val)), (float)((int)(z / val))}; // Jess fixing this
        // warning
        return {static_cast<float>(static_cast<int>(x / val)), static_cast<float>(static_cast<int>(y / val)),
                static_cast<float>(static_cast<int>(z / val))};
    }

    template <class T>
    Coordinates operator*(T val) const
    {
        return {x * val, y * val, z * val};
    }

    float operator[](int idx) const
    {
        switch (idx) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                throw(-1);
        }
    }

    // float operator[](unsigned dimension) const
    // {
    //     switch (dimension) {
    //         case Dimension::x:
    //             return x;
    //         case Dimension::y:
    //             return y;
    //         case Dimension::z:
    //             return z;
    //             // default: // Jess turned this off, it is causing a warning (making the complie slower) and seems
    //             // unnessary
    //             //     assert(false);
    //             //     return nan;
    //     }
    // }

    /**
     * Check if x, y, z coordinates are less than x, y, z coordinates of another Coordinates object.
     *
     * @param other   Coordinates to check against
     * @return        true if x, y, and z are less than other Coordinate's x, y and z.
     */
    bool lessThanXYZ(const Coordinates &other) const { return x < other.x && y < other.y && z < other.z; }

    /**
     * Check if x, y, z coordinates are less than or equal to x, y, z coordinates of another Coordinates object.
     *
     * @param other   Coordinates to check against
     * @return        true if x, y, and z are less than or equal to other Coordinate's x, y and z.
     */
    bool lessThanEqualXYZ(const Coordinates &other) const { return x <= other.x && y <= other.y && z <= other.z; }

    /**
     * Check if x, y, z coordinates are greater than x, y, z coordinates of another Coordinates object.
     *
     * @param other   Coordinates to check against
     * @return        true if x, y, and z are greater than other Coordinate's x, y and z.
     */
    bool greaterThanXYZ(const Coordinates &other) const { return x > other.x && y > other.y && z > other.z; }

    /**
     * Check if x, y, z coordinates are greater than or equal to x, y, z coordinates of another Coordinates object.
     *
     * @param other   Coordinates to check against
     * @return        true if x, y, and z are greater than or equal to other Coordinate's x, y and z.
     */
    bool greaterThanEqualXYZ(const Coordinates &other) const { return x >= other.x && y >= other.y && z >= other.z; }

    friend std::ostream &operator<<(std::ostream &os, const Coordinates &c)
    {
        os << "{" << c.x << ", " << c.y << ", " << c.z << "}";
        return os;
    }

    float x, y, z;

    static bool compare2D(const Coordinates &lhs, const Coordinates &rhs)
    {
        return (lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y));
    }
};

inline Coordinates operator+(Coordinates lhs, const Coordinates &rhs)
{
    lhs += rhs;
    return lhs;
}

inline Coordinates operator-(Coordinates lhs, const Coordinates &rhs)
{
    lhs -= rhs;
    return lhs;
}

#endif  // COORDINATES_H
