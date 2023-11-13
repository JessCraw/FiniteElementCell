#include "VtkMeshWriter.hpp"

VtkMeshWriter::VtkMeshWriter(Population &rCellPopulation, std::string OutputDirectory)
    : mpCellPopulation(rCellPopulation),
      mOutputDirectory(OutputDirectory)
{
}

VtkMeshWriter::~VtkMeshWriter() {}

void VtkMeshWriter::WriteVTKMesh(int time)
{
    // Create the cells mesh
    vtkUnstructuredGrid *pVtkUnstructedMesh = this->MakeVtkMesh();
    this->WriteMesh(pVtkUnstructedMesh, "mesh", time);

    // Create the grid mesh
    vtkUnstructuredGrid *pVtkUnstructedMesh2 = this->GetGridData();
    this->WriteMesh(pVtkUnstructedMesh2, "grid", time);

    this->GetGridDataFirstGo(time);
    // this->UpdatePVDFile(time);

    // Make some random test grid

    // vtkDataSetSurfaceFilter *surfaceFilter = vtkDataSetSurfaceFilter::New();
}

void VtkMeshWriter::WriteVTKCellMesh(int time)
{
    // Create the cells mesh
    vtkUnstructuredGrid *pVtkUnstructedMesh = this->MakeVtkCellMesh();
    this->WriteMesh(pVtkUnstructedMesh, "mesh", time);
}

// #include <fstream>
// #include <iostream>

void VtkMeshWriter::UpdatePVDFile(int time)
{
    vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    std::string directory = mOutputDirectory;  //+ std::to_string(time) + ".pvtp";

    // Number of time steps
    int numTimeSteps = time;  // Adjust this to your actual number of time steps

    // Create a PVD (ParaView Collection) file
    std::ofstream pvdFile("time_series.pvd");

    if (!pvdFile.is_open()) {
        std::cerr << "Error: Could not open the PVD file for writing." << std::endl;
        // return 1;
    }
    // Write the PVD file header
    pvdFile << "<?xml version=\"1.0\"?>" << std::endl;
    pvdFile << "<VTKFile type=\"Collection\" version=\"0.1\">" << std::endl;
    pvdFile << "  <Collection>" << std::endl;

    // Loop through each time step
    for (int timestep = 0; timestep < numTimeSteps; timestep++) {
        // Construct the VTU file name for the current time step
        std::string vtuFileName = "time_step_Jessssss" + std::to_string(timestep) + ".vtu";

        // Write the DataSet entry for the current time step
        pvdFile << "    <DataSet timestep=\"" << timestep << "\" file=\"" << directory << vtuFileName << "\"/>"
                << std::endl;

        // At this point, you can generate and save the VTU file for the current time step
        // Replace the following lines with your VTU generation and saving code

        // Create and save the VTU file
        std::ofstream vtu(vtuFileName);
        if (!vtu.is_open()) {
            std::cerr << "Error: Could not open the VTU file for writing." << std::endl;
            // return 1;
        }

        // Write VTU data for the current time step
        vtu << "<?xml version=\"1.0\"?>" << std::endl;
        // Write VTU data as needed
        vtu.close();
    }

    // Close the PVD file
    pvdFile << "  </Collection>" << std::endl;
    pvdFile << "</VTKFile>" << std::endl;
    pvdFile.close();

    std::cout << "PVD file created successfully." << std::endl;

    // return 0;
}

void VtkMeshWriter::GetCellCenter(vtkCell *cell, double *center)
{
    center[0] = center[1] = center[2] = 0.0;
    int numPoints = cell->GetNumberOfPoints();
    for (int i = 0; i < numPoints; i++) {
        double point[3];
        cell->GetPoints()->GetPoint(i, point);
        center[0] += point[0];
        center[1] += point[1];
        center[2] += point[2];
    }
    center[0] /= numPoints;
    center[1] /= numPoints;
    center[2] /= numPoints;
}

void VtkMeshWriter::WriteMesh(vtkUnstructuredGrid *vtkMesh, std::string filename, int time)
{
    // This block is to guard the mesh writers (vtkXMLPUnstructuredGridWriter) so that they
    // go out of scope, flush buffers and close files
    assert(vtkMesh->CheckAttributes() == 0);
    vtkXMLPUnstructuredGridWriter *p_writer = vtkXMLPUnstructuredGridWriter::New();
    p_writer->SetDataModeToBinary();

#if VTK_MAJOR_VERSION >= 6
    p_writer->SetInputData(vtkMesh);
#else
    std::cout << "You are using an old version of VTK; VTK_MAJOR_VERSION: " << VTK_MAJOR_VERSION << std::endl;
    p_writer->SetInput(vtkMesh);
#endif
    // Need to get some proper testoutput file defined like chaste
    // print(mOutputDirectory);
    // print(filename);
    std::string vtk_file_name = mOutputDirectory + filename + "_" + std::to_string(time) + ".pvtu";

    // print(vtk_file_name);
    p_writer->SetFileName(vtk_file_name.c_str());
    p_writer->Write();
    p_writer->Delete();  // Reference counted

    // Want to delete mesh here
    vtkMesh->Delete();

    // // Delete the mesh
    // mesh->DeleteCells();
    // mesh->RemoveAllPoints();

    // // Clear reference
    // mesh = nullptr;
}

vtkUnstructuredGrid *VtkMeshWriter::MakeVtkMesh()
{
    vtkUnstructuredGrid *pVtkUnstructedMesh = vtkUnstructuredGrid::New();
    // Construct nodes aka as Points
    vtkPoints *p_pts = vtkPoints::New(VTK_DOUBLE);
    p_pts->GetData()->SetName("Vertex positions");
    /**
     * mCells is a pointer to the vector containing all the cell pointers, to access cell
     * i, we need to use (*mCells)[i], this thing I get from this will be a pointer to that cell
     **/

    /**
     * Note this code currently only handles one cell -- going to have work on this
     **/
    std::vector<Coordinates> Locations = (*mCells)[0]->GetAllNodePositions();

    for (int n = 0; n < Locations.size(); ++n) {
        p_pts->InsertPoint(n, Locations[n].x, Locations[n].y, Locations[n].z);
    }

    // This isnt working here
    pVtkUnstructedMesh->SetPoints(p_pts);

    p_pts->Delete();
    return pVtkUnstructedMesh;
}

vtkUnstructuredGrid *VtkMeshWriter::MakeVtkCellMesh()
{
    vtkUnstructuredGrid *pVtkUnstructedMesh = vtkUnstructuredGrid::New();
    // Construct nodes aka as Points
    vtkPoints *p_pts = vtkPoints::New(VTK_DOUBLE);
    p_pts->GetData()->SetName("Vertex positions");
    /**
     * mCells is a pointer to the vector containing all the cell pointers, to access cell
     * i, we need to use (*mCells)[i], this thing I get from this will be a pointer to that cell
     **/
    int n = 0;
    int TotalN = 0;
    std::vector<CellPtr> pCells = mpCellPopulation.GetCells();
    for (std::vector<CellPtr>::iterator celIt = pCells.begin(); celIt != pCells.end(); ++celIt) {
        /** Get the mesh from the cell  */
        MeshPtr pcellMesh = (*celIt)->GetMesh();

        /** Get the nodes from the mesh  */
        std::vector<NodePtr> pNodes = pcellMesh->GetNodes();

        /** Loop over nodes and put them in the vtk mesh  */
        for (std::vector<NodePtr>::iterator noIt = pNodes.begin(); noIt != pNodes.end(); ++noIt) {
            c_vector<double, 2> Location = (*noIt)->GetNodeLocation();
            p_pts->InsertPoint(n, Location[0], Location[1], 0);
            n += 1;
        }

        /** Loop over edges, get the node indexes, and put them in the vtk mesh  */
        std::vector<EdgePtr> mpEdges = pcellMesh->GetEdges();
        for (std::vector<EdgePtr>::iterator edgIt = mpEdges.begin(); edgIt != mpEdges.end(); ++edgIt) {
            std::pair<int, int> IndexPair = (*edgIt)->GetNodeIndexPair();

            vtkCell *p_cell = vtkLine::New();
            vtkIdList *p_cell_id_list = p_cell->GetPointIds();
            {
                p_cell_id_list->SetId(0, (IndexPair).first + TotalN);  // Node in Spring 0
                p_cell_id_list->SetId(1, (IndexPair).second + TotalN);  // Node in Spring 1
            }
            pVtkUnstructedMesh->InsertNextCell(p_cell->GetCellType(), p_cell_id_list);
            p_cell->Delete();  // Reference counted
        }

        TotalN += n;
    }

    pVtkUnstructedMesh->SetPoints(p_pts);
    p_pts->Delete();

    return pVtkUnstructedMesh;
}
