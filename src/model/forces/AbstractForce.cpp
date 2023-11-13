#include "AbstractForce.hpp"

AbstractForce::AbstractForce() {}

AbstractForce::~AbstractForce() {}

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
