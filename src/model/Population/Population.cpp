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

// Population::Population(CircleMeshGenerator *pMesh, int numCells) : mpMesh(pMesh),
//                                                                    mNumCells(numCells)
// {
//     print("This should be called ");
//     print_var2(pMesh->mNumberOfCells, mNumCells);
//     if (pMesh->mNumberOfCells != mNumCells)
//     {
//         print("duplicate mesh");
//     }else
//     { // Want to use this mesh to create a bunch of cells

//     }

// }

// // CircleMeshGenerator *p_static_cast_mesh = static_cast<CircleMeshGenerator *>(pMesh);
// Population::Population(AbstractMesh *pMesh, int numCells) : mpMesh(pMesh),
//                                                             mNumCells(numCells)
// {
//     print("This should be called yet -- not implemented");
// }
