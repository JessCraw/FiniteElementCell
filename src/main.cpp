#include "Parameters.h"
// #include "World.h"

#include <cstdlib>
#include <ctime>
// #if GRAPHICS
// #include "graphics/Graphics.h"
// #endif
// #if BENCHMARKING
// #include "benchmarking/Benchmarking.h"
// #endif
// #if DEBUGGING
// #include "debug/MemoryAllocationMetrics.h"
// #endif
// #include "debug/Instrumentor.h"

int main(int argc, char *argv[])
{
    //     // Declare and Parse CLI Parameters
    //     std::cout << "\n Some words here \n " << std::endl;
    //     auto &options = AceParameters::Instance();
    //     if (!options.parseCommandLineOptions(argc, argv)) {
    //         return EXIT_FAILURE;
    //     }
    //     if (options.wasHelpRequested()) {
    //         return EXIT_SUCCESS;
    //     }

    // #if DEBUGGING
    //     std::cout << "\n Debuggin is true \n  \n " << std::endl;
    //     memoryAllocationMetrics.printCurrentUsage();
    // #endif
    // #if GRAPHICS
    //     std::cout << "\n Graphics is true \n  \n " << std::endl;
    //     Graphics::run();

    // #elif BENCHMARKING
    //     std::cout << "\n Benchmarking is true \n  \n " << std::endl;
    //     Benchmarking::run();
    // #else
    //     std::cout << "\n Else??? is true \n  \n " << std::endl;

    //     Instrumentor::get().beginSession("Profile");
    //     auto world = std::make_unique<World>();
    //     world->runSimulation();
    //     Instrumentor::get().endSession();
    // #endif
    // #if DEBUGGING
    //     memoryAllocationMetrics.printCurrentUsage();
    // #endif

    return EXIT_SUCCESS;
}

// cmake -S . -B build -DGRAPHICS=OFF && cmake --build build && ./build/acemodel
// cmake -S . -B build -Wno-dev -DGRAPHICS=OFF && cmake --build build