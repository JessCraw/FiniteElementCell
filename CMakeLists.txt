# Minimum ascertained using https://github.com/nlohmann/cmake_min_version
cmake_minimum_required(VERSION 3.13.5 FATAL_ERROR)


project(  FiniteElementCell  VERSION 1.0 LANGUAGES CXX C)
# project(AceModel)
find_package(CxxTest REQUIRED)
include_directories(${CXXTEST_INCLUDE_PATH}) #  gives path for cxxtest, which is set in the bash profile, cxxtest lives with all the other brew softwear


# add_definitions(-w)
# add_definitions ("-Wall")
# Project wide setup
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CTEST_TEST_TIMEOUT 300) # Jess needed to add this for the build

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w") # Disable all warnings


add_custom_target(
    copy-compile-commands ALL
    ${CMAKE_COMMAND}
    -E
    copy_if_different
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_LIST_DIR}
)

# Acemodel options
# option(GRAPHICS "Show Graphics" ON)
# option(IMGUI "Show imgui windows" ON)
# option(DEBUGGING "Debug ON/OFF" OFF)
# option(BENCHMARKING "Benchmarking ON/OFF" OFF)
option(BUILD_TESTING "Build Tests" ON)
option(TRUE_RANDOM "Use non-deterministic random numbers" OFF)
option(ZERO_FLUX "Turn on zero flux boundary condition. If off then boundary condition is constant (will stay the same value as set initally)" OFF)
option(TAKE_PROTEINS_FROM_ENVIRONMENT "Remove proteins from environment when bound to membrane receptors" ON)

#  Need to set the output directory -- TODO make sure this is enforced for new users 
option(OUTPUT_DIRECTORY "Provide the output directory" "/Users/crawshj/Documents/Projects/Tufts/testoutput/") 

set(OUTPUT_DIRECTORY  "/Users/crawshj/Documents/Projects/Tufts/testoutput/")




# Add project_options v0.13.1
# https://github.com/cpp-best-practices/project_options
include(FetchContent)
FetchContent_Declare(
    _project_options URL https://github.com/cpp-best-practices/project_options/archive/refs/tags/v0.13.1.zip
)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)
project_options(
    ENABLE_CACHE
)

find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    add_definitions("-DHAS_BOOST")
endif()

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Set up default build type for when none defined
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE ${default_build_type})
endif()

add_compile_options(
    $<$<CONFIG:DEBUG>:-g>
    $<$<CONFIG:DEBUG>:-O0>
    $<$<CONFIG:RELEASE>:-O3>
    $<$<CONFIG:PROFILING>:-O3>
    $<$<CONFIG:PROFILING>:-g>
    $<$<CONFIG:PROFILING>:-shared-libgcc>
)


# Externally provided content
# add_subdirectory(extern)


add_definitions(-DUNITTEST)
enable_testing()
add_subdirectory(test)
# Main targets built by this project
add_subdirectory(src)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

set(VTK_DIR /opt/homebrew/Cellar/vtk/9.2.6_5/lib/cmake/vtk-9.2/  ) 
find_package(VTK REQUIRED PATHS /opt/homebrew/Cellar/vtk/9.2.6_5/include/vtk-9.2/ /opt/homebrew/Cellar/vtk/9.2.6_5/lib NO_DEFAULT_PATH) 


find_package(VTK REQUIRED)
include_directories(/opt/homebrew/Cellar/vtk/9.2.6_5/)
target_link_libraries(TestJessSuite ${VTK_LIBRARIES}  model)

configure_file(
  ${CMAKE_SOURCE_DIR}/config.h.in
  ${CMAKE_BINARY_DIR}/config.h
)
