# set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/model")

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
include_directories(${VTK_INCLUDE_DIRS}) #  gives path for vtk, which is set in the bash profile, cxxtest lives with all the other brew softwear

# TODO: Maybe profile performance STATIC vs SHARED ?
add_library(
    model STATIC
    Simulation.cpp
    SmartPointers.hpp
    # Population
    Population/Population.cpp
    Population/Cell/Cell.cpp
    MathsFunctions.cpp
    # Forces
    forces/AbstractForce.cpp
    forces/LinearSpringForce.cpp
    forces/RadialForce.cpp
    # Meshes
    mesh/VtkMeshWriter.cpp
    mesh/VtkMeshReader.cpp
    mesh/Generators/CircleMeshGenerator.cpp
    mesh/AbstractMesh.cpp
    mesh/Nodes/Node.cpp
    mesh/Edges/Edge.cpp
    mesh/SimpleRemeshing.cpp
    # Signalling
    signalling/AbstractSignalling.cpp
    signalling/VEGFsignalling.cpp
    # Properties
    Properties/AbstractNodeProperty.hpp
)



target_include_directories(
    model
    # PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} mesh/Generators mesh/Population mesh/Population/Cell
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} mesh Properties signalling forces Population Population/spring  Population/Cell environment utility ode mesh/Generators
    INTERFACE ${SOURCE_DIR}
)
