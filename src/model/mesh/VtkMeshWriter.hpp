
#ifndef VTKMESHWRITER_HPP_
#define VTKMESHWRITER_HPP_

#include "/Users/crawshj/Documents/Projects/Tufts/FiniteElementCell/build/config.h"
#include "MathsFunctions.hpp"
#include "Population/Cell/Cell.hpp"
#include "utility/Coordinates.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <utility>
// #include "utility/vec3.h"

#include "AbstractMesh.hpp"
#include "Population.hpp"

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkCellLocator.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkDoubleArray.h>
#include <vtkLine.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkQuad.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkStructuredGrid.h>
#include <vtkTetra.h>
#include <vtkTriangle.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkXMLPUnstructuredGridWriter.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkXMLUnstructuredGridWriter.h>

class VtkMeshWriter
{
public:
    // Constructor
    // VtkMeshWriter();

    VtkMeshWriter(Population &rCellPopulation, std::string OutputDirectory);
    // std::vector<CellPtr> mpCells;
    Population &mpCellPopulation;

    // VtkMeshWriter(std::vector<std::unique_ptr<Cell>> *Cells, Grid *grid, std::string OutputDirectory);

    // Destructor
    ~VtkMeshWriter();

    void WriteVTKMesh(int time);
    void WriteVTKCellMesh(int time);

    void WriteMesh(vtkUnstructuredGrid *vtkMesh, std::string filename, int time);

    vtkUnstructuredGrid *MakeVtkMesh();
    vtkUnstructuredGrid *MakeVtkCellMesh();

    void CleanVtkMeshes();

    vtkUnstructuredGrid *GetGridData();
    void GetGridDataFirstGo(int time);
    void UpdatePVDFile(int time);

    std::vector<std::unique_ptr<Cell>> *mCells;
    // Grid *mGrid;

    // private:
    //     vtkUnstructuredGrid *mpVtkUnstructedMesh = vtkUnstructuredGrid::New();
    //     vtkUnstructuredGrid *mpVtkUnstructedGrid = vtkUnstructuredGrid::New();
    std::string mOutputDirectory;

    void GetCellCenter(vtkCell *cell, double *center);
};
#endif /*VTKMESHWRITER_HPP_*/
