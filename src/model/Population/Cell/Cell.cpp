#include "Cell.hpp"

class Grid;

int Cell::idCounter = 0;

Cell::Cell()
    : id(++idCounter)
{
}

void Cell::CreateMesh(MeshPtr Mesh) { mpMesh = Mesh; }

MeshPtr Cell::GetMesh() { return mpMesh; }

int Cell::GetCellID() { return id; }

std::vector<int> Cell::GetNodeList() { return mNodeList; }

// std::vector<Spring *> Cell::GetSpringPointers()
// {
//     std::vector<Spring *> SpringVector;

//     for (auto &springConnection : springs) {
//         SpringVector.push_back(springConnection.getSpring());
//     }
//     return SpringVector;
// }

// need to template this for the triangles

// void Cell::updateProteins()
// {
//     for (auto receptor : Proteins::MembraneReceptorsArray) {
//         double active = std::accumulate(springs.begin(), springs.end(), 0., [receptor](double count, auto &spring) {
//             return count + spring.getReceptorActive(receptor);
//         });
//         auto i = static_cast<int>(receptor);
//         proteinActiveTranslocation[i].push(active);
//         mProteins[receptor].setActive(proteinActiveTranslocation[i].front());
//         proteinActiveTranslocation[i].pop();
//     }

//     for (auto ligand : Proteins::MembraneLigandsArray) {
//         double total = std::accumulate(springs.begin(), springs.end(), 0., [ligand](double count, auto &spring) {
//             return count + spring.getProteinTotal(ligand);
//         });
//         auto i = static_cast<int>(ligand);
//         mProteins[ligand].setTotal(total);
//     }
// }

// void Cell::geneRegulationNetwork()
// {
//     auto length = static_cast<double>(getTotalLength());

//     std::array<double, Proteins::N_PROTEINS> concentrations;
//     concentrations[Proteins::Index::VEGFR_ACTIVE] =
//     mProteins[Proteins::MembraneReceptors::VEGFR].getActiveConc(length);
//     concentrations[Proteins::Index::VEGFR_UNBOUND] =
//         mProteins[Proteins::MembraneReceptors::VEGFR].getUnboundConc(length);

//     auto newConcentrations = Odes::Solver::solve(concentrations, Odes::Cell::definition);

//     if (concentrations[Proteins::Index::VEGFR_UNBOUND] != newConcentrations[Proteins::Index::VEGFR_UNBOUND]) {
//         auto total = newConcentrations[Proteins::Index::VEGFR_UNBOUND] * length +
//                      newConcentrations[Proteins::Index::VEGFR_ACTIVE] * length;
//         mProteins[Proteins::MembraneReceptors::VEGFR].setTotal(total);
//         updateSpringsReceptorTotal(Proteins::MembraneReceptors::VEGFR, total);
//     }
// }

// float Cell::getTotalLength() const
// {
//     return std::accumulate(springs.begin(), springs.end(), 0.,
//                            [](float count, const auto &spring) { return count + spring.getLength(); });
// }

// std::array<double, Proteins::N_CELL_PROTEINS> Cell::getProteinValues() const
// {
//     using namespace Proteins;

//     std::array<double, Proteins::N_CELL_PROTEINS> retval {};

//     retval[Proteins::Index::VEGFR_TOTAL] = mProteins[MembraneReceptors::VEGFR].getTotal();
//     retval[Proteins::Index::VEGFR_ACTIVE] = mProteins[MembraneReceptors::VEGFR].getActive();
//     retval[Proteins::Index::VEGFR_UNBOUND] = mProteins[MembraneReceptors::VEGFR].getUnbound();

//     retval[Proteins::Index::NOTCH_TOTAL] = mProteins[MembraneReceptors::NOTCH].getTotal();
//     retval[Proteins::Index::NOTCH_ACTIVE] = mProteins[MembraneReceptors::NOTCH].getActive();
//     retval[Proteins::Index::NOTCH_UNBOUND] = mProteins[MembraneReceptors::NOTCH].getUnbound();

//     retval[Proteins::DLL4_TOTAL] = mProteins[MembraneLigands::DLL4].getTotal();

//     return retval;
// }

// std::vector<std::shared_ptr<Node>> Cell::GetNodes() { return mNodes; }
