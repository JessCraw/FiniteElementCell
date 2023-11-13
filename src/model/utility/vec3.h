
#ifndef VEC3_H
#define VEC3_H

#include "Coordinates.h"

#include <iostream>

struct vec3
{
    vec3() = default;

    vec3(int x, int y, int z)
        : x(x),
          y(y),
          z(z)
    {
    }

    vec3(const vec3 &other, int dimension, int dimensionVal)
        : x(dimension == 0 ? dimensionVal : other.x),
          y(dimension == 1 ? dimensionVal : other.y),
          z(dimension == 2 ? dimensionVal : other.z)
    {
    }

    explicit vec3(Coordinates coords)
        : x(static_cast<int>(coords.x)),
          y(static_cast<int>(coords.y)),
          z(static_cast<int>(coords.z))
    {
    }

    // --- Arithmetic Operators --- //

    vec3 &operator+=(const vec3 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    vec3 &operator-=(const vec3 &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    vec3 operator+(const Coordinates &c) const
    {
        return {x + static_cast<int>(c.x), y + static_cast<int>(c.y), z + static_cast<int>(c.z)};
    }

    vec3 operator-(const Coordinates &c) const
    {
        return {x - static_cast<int>(c.x), y - static_cast<int>(c.y), z - static_cast<int>(c.z)};
    }

    vec3 operator/(int val) const { return {x / val, y / val, z / val}; }

    vec3 operator/(const vec3 &other) const { return {x / other.x, y / other.y, z / other.z}; }

    vec3 operator*(int val) const { return {x * val, y * val, z * val}; }

    vec3 operator%(int val) const { return {x % val, y % val, z % val}; }

    vec3 &operator+=(int val)
    {
        x += val;
        y += val;
        z += val;
        return *this;
    }

    vec3 &operator-=(int val)
    {
        x -= val;
        y -= val;
        z -= val;
        return *this;
    }

    // --- Boolean Operators --- //

    // TODO: These boolean operator overloads do not currently adhere to requirements for use in STL algorithms, as they
    // do not adhere to strict weak ordering.
    //       They can be refactored using std::tie or std::tuple to implement this concisely, if required.
    bool operator==(const vec3 &other) const { return x == other.x && y == other.y && z == other.z; }

    bool operator!=(const vec3 &other) const { return !(operator==(other)); }

    bool operator==(int val) const { return x == val && y == val && z == val; }

    bool operator!=(int val) const { return !(operator==(val)); }

    bool operator<(const vec3 &other) const { return x < other.x && y < other.y && z < other.z; }

    bool operator<=(const vec3 &other) const { return x <= other.x && y <= other.y && z <= other.z; }

    bool operator<(int v) const { return x < v && y < v && z < v; }

    bool operator<=(int v) const { return x <= v && y <= v && z <= v; }

    bool operator>(const vec3 &other) const  // this is maybe a bit dodgy
    {
        return x > other.x && y > other.y && z > other.z;
    }

    bool operator>=(const vec3 &other) const  // this is maybe a bit dodgy
    {
        return x >= other.x && y >= other.y && z >= other.z;
    }

    bool operator>(int other) const  // this is maybe a bit dodgy
    {
        return (x > other) && (y > other) && (z > other);
    }

    bool operator>=(int v) const  // this is maybe a bit dodgy
    {
        return x >= v && y >= v && z >= v;
    }

    /**
     * Access using array subcsript notation (return by value)
     */
    int operator[](int idx) const
    {
        if (idx == 0) {
            return x;
        } else if (idx == 1) {
            return y;
        } else if (idx == 2) {
            return z;
        } else {
            std::cout << "Index out of bounds for vec3" << std::endl;
            throw(-1);
        }
    }

    /**
     * Access using array subcsript notation (return by reference)
     */
    int &operator[](int idx)
    {
        if (idx == 0) {
            return x;
        } else if (idx == 1) {
            return y;
        } else if (idx == 2) {
            return z;
        } else {
            std::cout << "Index out of bounds for vec3" << std::endl;
            throw(-1);
        }
    }

    /**
     * Set the value of dimensionsional component 'axis' to 'value'.
     */
    void set(int axis, int val)
    {
        if (axis == 0) {
            x = val;
        } else if (axis == 1) {
            y = val;
        } else if (axis == 2) {
            z = val;
        } else {
            std::cout << "Axis out of bounds for vec3" << std::endl;
            throw(-1);
        }
    }

    /**
     * Check if the vec3 Object is zero in all dimensions.
     * @return  True if zero in all dimensions, otherwise False.
     */
    bool isZero() const { return (x == 0) && (y == 0) && (z == 0); }

    friend std::ostream &operator<<(std::ostream &os, const vec3 &v)
    {
        os << "{" << v.x << ", " << v.y << ", " << v.z << "}";
        return os;
    }

    // --- Conversions --- //

    /**
     * Create a Coordinates object from this vec3 object.
     * Note: Simply casts int values to float values, without scaling; therefore if conservation of spatial geometry
     * is required, the vec3's values MUST already be in units of finest grid spacing.
     *
     * @return  Coordinates object created from this vec3 object.
     */
    Coordinates toCoordinates() const { return {static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)}; }

    /**
     * Create a vec3 object from a Coordinates object.
     * Note: Simply casts float values to int values, without scaling; therefore if conservation of spatial geometry
     * is required, the vec3's values will need to be scaled as a post-processing step.
     *
     * @return  Coordinates object created from this vec3 object.
     */
    static vec3 fromCoordinates(const Coordinates &coordinates) { return vec3(coordinates); }

    operator Coordinates() const { return this->toCoordinates(); }

    // --- Explicitly Named Comparison Functions --- // (Intentionallly not strict weak ordering)

    /**
     * Less Than comparison for all components of this and other.
     *
     * @return  True if all components of this are less than other.
     */
    bool isAllDimsLT(const vec3 &other) const { return x < other.x && y < other.y && z < other.z; }

    /**
     * Less Than or Equal comparison for all components of this and other.
     *
     * @return  True if all components of this are less than or equal than other.
     */
    bool isAllDimsLE(const vec3 &other) const { return x <= other.x && y <= other.y && z <= other.z; }

    /**
     * Greater Than comparison for all components of this and other.
     *
     * @return  True if all components of this are greater than other.
     */
    bool isAllDimsGT(const vec3 &other) const { return x > other.x && y > other.y && z > other.z; }

    /**
     * Greater Than or Equal comparison for all components of this and other.
     *
     * @return  True if all components of this are greater or equal than other.
     */
    bool isAllDimsGE(const vec3 &other) const { return x >= other.x && y >= other.y && z >= other.z; }

    /* Vector Components */
    int x = 0, y = 0, z = 0;
};

inline vec3 operator+(vec3 lhs, const vec3 &rhs)
{
    lhs += rhs;
    return lhs;
}

inline vec3 operator-(vec3 lhs, const vec3 &rhs)
{
    lhs -= rhs;
    return lhs;
}

inline vec3 operator+(vec3 lhs, int rhs)
{
    lhs += rhs;
    return lhs;
}

inline vec3 operator-(vec3 lhs, int rhs)
{
    lhs -= rhs;
    return lhs;
}

#endif  // VEC3_H
