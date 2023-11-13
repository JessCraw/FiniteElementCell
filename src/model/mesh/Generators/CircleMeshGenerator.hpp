#ifndef CIRCLEMESHGENERATOR_HPP_
#define CIRCLEMESHGENERATOR_HPP_

#include "MathsFunctions.hpp"
#include "Parameters.h"

#include <iostream>
#include <memory>
#include <vector>
// #include "debug/Instrumentor.h"
#include "utility/vec3.h"

#include </opt/homebrew/Cellar/vtk/9.2.6_5/include/vtk-9.2/vtkCellArray.h>

#include <algorithm>
#include <array>
#include <boost/shared_ptr.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>

// #include <iostream>
// #include <memory>
#include "AbstractMesh.hpp"

#include <utility>
typedef boost::shared_ptr<AbstractMesh> MeshPtr;
class CircleMeshGenerator : public AbstractMesh

{
    // protected:
    /** A pointer to the mesh this class creates */
    // boost::shared_ptr<AbstractMesh> mpMesh;

public:
    CircleMeshGenerator(int nNodes, double Radius, double centerX, double centerY);

    std::string mMeshType = "IdealCircle";
};

#endif  // CELL_H
