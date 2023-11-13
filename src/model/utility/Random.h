#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <type_traits>

// Magic to infer Integer/Real distribution from the type parameter
// https://stackoverflow.com/a/55275707/14789685
template <class T>
using uniform_distribution = typename std::conditional<
    std::is_floating_point<T>::value, std::uniform_real_distribution<T>,
    typename std::conditional<std::is_integral<T>::value, std::uniform_int_distribution<T>, void>::type>::type;

namespace Random
{

    std::mt19937_64 &getEngine();

    template <typename T>
    T generateNumberBetween(T min, T max)
    {
        uniform_distribution<T> distribution(min, max);
        return distribution(getEngine());
    }

    /**
     * Generate random number in range: [0, 1] for integral types, [0, 1) for real types.
     *
     * @return      Random number in range [0, 1] (integral) or [0, 1) (real)
     */
    template <typename T>
    T generateNumberZeroOne()
    {
        return generateNumberBetween<T>(0, 1);
    }

    /**
     * Generate random number in given range: [min, max] for integral types, [min, max) for real types.
     *
     * @param min   The start of the range (inclusive)
     * @param max   The end of the range (inclusive)
     * @return      Random number in range [min, max] (integral) or [min, max) (real)
     */
    template <typename T>
    bool randomChance(T probability)
    {
        return generateNumberZeroOne<T>() < probability;
    }

}  // namespace Random

#endif  // RANDOM_H
