# CMakeLists.txt in the 'tests' directory

find_package(CxxTest REQUIRED)
find_package(VTK REQUIRED)
include_directories(${VTK_INCLUDE_DIRS}) #  gives path for vtk, which is set in the bash profile, cxxtest lives with all the other brew softwear


# Include the CxxTest headers
# 1) Give direction to cxxtest 
include_directories(${CXXTEST_INCLUDE_PATH}) #  gives path for cxxtest, which is set in the bash profile, cxxtest lives with all the other brew softwear

# 2) Apparently important for cxxtest, figure out what this does
enable_testing()
# 3) MyTestSuite is what you will make , cxxtest_runner.cpp is nothing to be and TestJessSuite.hpp is the name of the test file you are making 
cxxtest_add_test(TestJessSuite cxxtest_runner.cpp ${CMAKE_CURRENT_SOURCE_DIR}/TestJessSuite.hpp) 

# include_directories("/Users/crawshj/Documents/Projects/Tufts/adaptive-cell-effort-model/","/Users/crawshj/Documents/Projects/Tufts/adaptive-cell-effort-model/src/" )
#add_library("/Users/crawshj/Documents/Projects/Tufts/adaptive-cell-effort-model/src/")


# dd_library(Chaste_COMMON_DEPS INTERFACE)
# target_link_libraries(TestJessSuite model)



# target_link_libraries(${VTK_LIBRARIES})


# find_package(VTK REQUIRED)
# include_directories(${VTK_INCLUDE_DIRS})




# 
# target_link_libraries(VTK::CommonCore)

# # Print out or log the value of VTK_INC_DIRS after it is set to verify it is correct

