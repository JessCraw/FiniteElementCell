#ifndef SIMPLEREMESHING_HPP_
#define SIMPLEREMESHING_HPP_

#include "MathsFunctions.hpp"
#include <cassert>
#include "Parameters.h"
#include "Population.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include "utility/vec3.h"

#include </opt/homebrew/Cellar/vtk/9.2.6_5/include/vtk-9.2/vtkCellArray.h>

#include <algorithm>
#include <array>
#include <boost/shared_ptr.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include "AbstractMesh.hpp"
#include "AbstractForce.hpp"

#include <utility>
// typedef boost::shared_ptr<AbstractMesh> MeshPtr;

class AbstractForce;
typedef boost::shared_ptr<AbstractForce> ForcePtr;
class SimpleRemeshing : public AbstractMesh

{
    // protected:
    /** A pointer to the mesh this class creates */
    // boost::shared_ptr<AbstractMesh> mpMesh;

public:
    SimpleRemeshing();

    void RemeshRandomEdges(Population &rCellPopulation);
    void RemeshAllEdges(Population &rCellPopulation);

    void CallRemeshing(Population &rCellPopulation);

    void AddForceCollection( std::vector<ForcePtr> pForceCollection);

    std::vector<ForcePtr> mpForceCollection;
};

#endif  // CELL_H
