#include "AbstractNodeProperty.hpp"

AbstractNodeProperty::AbstractNodeProperty()
{
}

AbstractNodeProperty::~AbstractNodeProperty() {}


// void AbstractNodeProperty::SetNodeProperty() 
// {
//     // Set the property
//     mpNode->rGetCellData().SetItem(mPropertyName, mpNode->rGetCellData().GetItem("CellType"));
// }

// void AbstractNodeProperty::GetNodeProperty() 
// {
//     // Set the property
//     mpNode->rGetCellData().SetItem(mPropertyName, mpNode->rGetCellData().GetItem("CellType"));
// }

// void AbstractNodeProperty::SetLabel() 
// {
//     // Set the property
//     mpNode->rGetCellData().SetItem(mPropertyName, mpNode->rGetCellData().GetItem("CellType"));
// }

void AbstractNodeProperty::SetLabel(std::string PropertySetLabel)
{
    mPropertyName = PropertySetLabel;
}

// void AbstractForce::OutputForceInfo(out_stream& rParamsFile)
// {
//     std::string force_type = GetIdentifier();

//     *rParamsFile << "\t\t<" << force_type << ">\n";
//     OutputForceParameters(rParamsFile);
//     *rParamsFile << "\t\t</" << force_type << ">\n";
// }

// void AbstractForce::OutputForceParameters(out_stream& rParamsFile)
// {
//     // No parameters to output
// }

// void AbstractForce::WriteDataToVisualizerSetupFile(out_stream& pVizSetupFile)
// {
//     // Nothing to output
// }
