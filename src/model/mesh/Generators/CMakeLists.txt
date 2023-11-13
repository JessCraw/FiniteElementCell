set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/mesh/Generators")

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
include_directories(${VTK_INCLUDE_DIRS}) #  gives path for vtk, which is set in the bash profile, cxxtest lives with all the other brew softwear

# add_subdirectory(Generators)


# TODO: Maybe profile performance STATIC vs SHARED ?
add_library(
    Generators STATIC
    CircleMeshGenerator.cpp
)


target_include_directories(
    Generators
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    INTERFACE ${SOURCE_DIR}
    # PUBLIC ${CMAKE_SOURCE_DIR}/extern/gcem/include/
    # PUBLIC ${CMAKE_SOURCE_DIR}/extern/
    PUBLIC ${CMAKE_SOURCE_DIR}/src/model/
    PUBLIC ${CMAKE_SOURCE_DIR}/src/model/Population/
    # PUBLIC ${CMAKE_SOURCE_DIR}/src/debug/
)


# target_include_directories(
#     model
#     PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
#     INTERFACE ${SOURCE_DIR}
# )

target_link_libraries(
    Generators
    PUBLIC CLI11::CLI11 Boost::headers ${OpenCV_LIBS}  # VTK::Common
    VTK::IOLegacy VTK::IOXML VTK::IOGeometry VTK::IOCore
    PRIVATE ${VTK_LIBRARIES} 
)
