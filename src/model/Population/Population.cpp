#include "Population.hpp"

// Think I will create a couple of different constructors for acouple of different types of meshes

Population::Population(int numCells, int CellRefinment, int CellRadius)
    : mNumCells(numCells),
      mCellsRefinment(CellRefinment),
      mCellsRadius(CellRadius)
{
    for (int i = 0; i < mNumCells; i++) {
        CellPtr pNewCell(new Cell());
        MeshPtr pNewMesh(new CircleMeshGenerator(CellRefinment, CellRadius, i * 2 * CellRadius,
                                                 0));  // CircleMeshGenerator mesh = CircleMeshGenerator(10, 5);

        pNewCell->CreateMesh(pNewMesh);
        this->mCells.push_back(pNewCell);
    }
}

std::vector<CellPtr> Population::GetCells() { return mCells; }
