#include "Random.h"

std::mt19937_64 &Random::getEngine()
{
#if TRUE_RANDOM
    static thread_local std::random_device rd;
#else
    static unsigned long int rd = 0;
#endif
    static thread_local std::mt19937_64 engine(rd);

    return engine;
}
