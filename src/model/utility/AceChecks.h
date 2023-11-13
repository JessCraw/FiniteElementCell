#ifndef ACECHECKS_H
#define ACECHECKS_H

#include <cassert>

namespace Checks
{
    template <typename T>
    constexpr bool IsPowerOfTwo(T value)
    {
        assert(value > 0);
        return (value & (value - 1)) == 0;
    }
}  // namespace Checks

#endif  // ACECHECKS_H
