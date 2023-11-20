

#ifndef MathsFunctions_HPP_
#define MathsFunctions_HPP_

#include "utility/Coordinates.h"
#include <cassert>
#include <math.h>

#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/vector.hpp>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using boost::numeric::ublas::c_vector;
using boost::numeric::ublas::inner_prod;
using boost::numeric::ublas::zero_vector;

/**
 * Membrane Surface Force
 * Force tyring to minimising surface area to the relaxed state
 */
// class MathsFunctions
// {
// public:
/**
 * Convenience function for quickly creating test vectors (1D).
 *
 * @param x entry in vector
 * @return vector=(x)
 */
c_vector<double, 1> Create_c_vector(double x);

/**
 * Convenience function for quickly creating test vectors (2D).
 *
 * @param x entry in vector
 * @param y entry in vector
 * @return vector=(x,y)
 */
c_vector<double, 2> Create_c_vector(double x, double y);

/**
 * Convenience function for quickly creating test vectors (3D).
 *
 * @param x entry in vector
 * @param y entry in vector
 * @param z entry in vector
 * @return vector=(x,y,z)
 */
c_vector<double, 3> Create_c_vector(double x, double y, double z);
c_vector<double, 4> Create_c_vector(double x, double y, double z, double p);

// Convert vector into a set

template <typename T>
std::set<T> convertToSet(std::vector<T> v);

// // std::map<unsigned, c_vector< c_vector< double, 3>, 3> > mMapping;
// // std::map<unsigned, c_vector<double, 3> > mForceMap;

template <typename T>
c_vector<c_vector<T, 3>, 3> Inverse(c_vector<c_vector<T, 3>, 3> Matrix);

template <typename T>
c_vector<c_vector<T, 3>, 3> MatrixMultiplication(c_vector<c_vector<T, 3>, 3> Matrix1,
                                                 c_vector<c_vector<T, 3>, 3> Matrix2);

template <typename T>
c_vector<T, 3> MatrixMultiplication(c_vector<c_vector<T, 3>, 3> Matrix, c_vector<T, 3> Vector);

template <typename T>
c_vector<c_vector<T, 3>, 3> MatrixTranspose(c_vector<c_vector<T, 3>, 3> Matrix1);

// c_vector<c_vector<double, 3>, 3> RowReduction(c_vector<c_vector<double, 3>, 3> Matrix);

// c_vector<c_vector<double, 3>, 3> MappingMatrix(MeshBasedCellPopulation<2, 3>* p_cell_population, typename
// // AbstractTetrahedralMesh<2, 3>::ElementIterator elem_iter, double a, double b, double theta);

// double det(c_vector<c_vector<double, 2>, 2> Matrix);

// double tr(c_vector<c_vector<double, 2>, 2> Matrix);

template <typename T>
std::pair<T, T> Create_pair(T x, T y);

template <typename T>
double MaintainOutwardsPointingNormal(c_vector<T, 3> Normal, c_vector<T, 3> x1);

template <typename T>
void PRINT_PAIR(std::pair<unsigned, unsigned> Pair);

template <typename T>
double AreVectorsSame(std::vector<T> Vector1, std::vector<T> Vector2);

template <typename T>
std::vector<unsigned> Intersection(std::vector<unsigned> Vector1, std::vector<unsigned> Vector2);

template <typename T>
std::vector<T> RemoveElement(std::vector<T> Vector1, T number);

template <typename T>
bool IsPairInVector(std::vector<std::pair<unsigned, unsigned>> Vector, std::pair<unsigned, unsigned> Pair);

template <typename T>
bool IsNumberInVector(std::vector<T> Vector, T number);

template <typename T>
T min_value(std::vector<T> Vector);

template <typename T>
T max_value(std::vector<T> Vector);

template <typename T>
T AddAngles(T alpha, T beta);

template <typename T>
T PeriodicAngle(T eta);

template <typename T>
bool IsVectorInVector(std::vector<c_vector<T, 3>> Vector, c_vector<T, 3> Locaiton);

void print(double statement);
void print(std::string statement);

void print(c_vector<double, 3> Locaiton);
void print(c_vector<float, 3> Locaiton);
void print(c_vector<int, 3> Locaiton);
void print(Coordinates Locaiton);

// std::string FormDebugHead();
#define print_var1(var) std::cout << "- " << #var " = " << var << " -" << std::endl << std::flush;
#define print_var2(var1, var2) \
    std::cout << "- " << #var1 " = " << var1 << ", " #var2 " = " << var2 << " -" << std::endl << std::flush;
#define print_var3(var1, var2, var3)                                                                         \
    std::cout << "- " << #var1 " = " << var1 << ", " #var2 " = " << var2 << ", " #var3 " = " << var3 << " -" \
              << std::endl                                                                                   \
              << std::flush;
// template <typename T, typename U>
// void print(T statement1, U statement2);

// template <typename T, typename U, typename S>
// void print(T statement1, U statement2, S statement3);

// template <typename T, typename U, typename S, typename V>
// void print(T statement1, U statement2, S statement3, V statement4);

template <class T>
c_vector<T, 3> VectorProduct(const c_vector<T, 3> &rA, const c_vector<T, 3> &rB)
{
    c_vector<T, 3> result;

    result(0) = rA(1) * rB(2) - rA(2) * rB(1);
    result(1) = rA(2) * rB(0) - rA(0) * rB(2);
    result(2) = rA(0) * rB(1) - rA(1) * rB(0);

    return result;
}

#define ASSERT(condition, message)                                 \
    if (!(condition)) {                                            \
        std::cerr << "Assertion Failed: " << message << std::endl; \
        std::abort();                                              \
    }

// };

// // Declare identifier for the serializer
// #include "SerializationExportWrapper.hpp"
// CHASTE_CLASS_EXPORT(MathsFunctions)

#endif /*MathsFunctions_HPP_*/