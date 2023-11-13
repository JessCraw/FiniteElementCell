# set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
add_subdirectory(model)

# CLI11 Package for setting parameters from command-line
include(FetchContent)
FetchContent_Declare(
  cli11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11
  GIT_TAG        v2.2.0
)
FetchContent_MakeAvailable(cli11)


find_package(VTK REQUIRED)
include_directories(/opt/homebrew/Cellar/vtk/9.2.6_5/)


add_library(config_lib INTERFACE)
target_sources(config_lib INTERFACE 
  ${CMAKE_BINARY_DIR}/config.h
)

# Model requires debug module
target_link_libraries(
    model 
    PUBLIC CLI11::CLI11 Boost::headers ${OpenCV_LIBS}  config_lib  # VTK::Common
    VTK::IOLegacy VTK::IOXML VTK::IOGeometry VTK::IOCore
    PRIVATE project_warnings # ${CMAKE_BINARY_DIR}/config.h
    ${VTK_LIBRARIES} 
)


# Jess turned this off 13th novemebr 2023
# target_link_libraries(
#     mesh
#     PUBLIC model gcem CLI11::CLI11 Boost::headers ${OpenCV_LIBS}
#     PRIVATE project_warnings FEcell_debug output
#     ${VTK_LIBRARIES} 
# )

# Benchmarking module requires Model and Debug modules
# target_link_libraries(FEcell_benchmarking PRIVATE project_warnings model FEcell_debug ${VTK_LIBRARIES} )

# Main executable depends on Model, Debug and Benchmarking modules
# 
# add_executable(FEcell main.cpp)
# target_link_libraries(
#     FEcell
#     PRIVATE project_options
#             project_warnings
#             model
#             # model/Population
#             # model/mesh
#             # FEcell_debug
#             # FEcell_benchmarking
#             # mesh
# )


# Set main executable location
# set_target_properties(FEcell PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


if(TRUE_RANDOM)
    target_compile_definitions(model PUBLIC TRUE_RANDOM=1)
else()
    target_compile_definitions(model PUBLIC TRUE_RANDOM=0)
endif()

if(ZERO_FLUX)
    target_compile_definitions(model PUBLIC ZERO_FLUX=1)
else()
    target_compile_definitions(model PUBLIC ZERO_FLUX=0)
endif()

if(TAKE_PROTEINS_FROM_ENVIRONMENT)
    target_compile_definitions(model PUBLIC TAKE_PROTEINS_FROM_ENVIRONMENT=1)
else()
    target_compile_definitions(model PUBLIC TAKE_PROTEINS_FROM_ENVIRONMENT=0)
endif()


set(VTK_DIR /opt/homebrew/Cellar/vtk/9.2.6_5/lib/cmake/vtk-9.2/  ) 
find_package(VTK REQUIRED PATHS /opt/homebrew/Cellar/vtk/9.2.6_5/include/vtk-9.2/ /opt/homebrew/Cellar/vtk/9.2.6_5/lib NO_DEFAULT_PATH) 

set(SOURCES
  # other source files
  ${CMAKE_BINARY_DIR}/config.h
)



message(${CMAKE_BINARY_DIR}/config.h "config location here")

