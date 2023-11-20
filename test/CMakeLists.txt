add_subdirectory(ValidateSprings)


find_package(CxxTest REQUIRED)
find_package(VTK REQUIRED)
include_directories(${VTK_INCLUDE_DIRS}) #  gives path for vtk, which is set in the bash profile, cxxtest lives with all the other brew softwear


# Include the CxxTest headers
# 1) Give direction to cxxtest 
include_directories(${CXXTEST_INCLUDE_PATH}) #  gives path for cxxtest, which is set in the bash profile, cxxtest lives with all the other brew softwear

# 2) Apparently important for cxxtest, figure out what this does
enable_testing()
# 3) MyTestSuite is what you will make , cxxtest_runner.cpp is nothing to be and TestJessSuite.hpp is the name of the test file you are making 
cxxtest_add_test(TestRemeshingOfCircle cxxtest_runner.cpp ${CMAKE_CURRENT_SOURCE_DIR}/TestRemeshingOfCircle.hpp) 
