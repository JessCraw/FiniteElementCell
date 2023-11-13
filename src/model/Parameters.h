#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <CLI/CLI.hpp>
#include <cstddef>

namespace Parameters::Defaults
{
    constexpr int timesteps = 15000;
    constexpr int runNumber = 0;
    constexpr float extensionActivation = 0.5;
    constexpr float extensionActivationMin = 1;
    constexpr float filopodiaExtensionAngleVariation = 0.122173;
    constexpr float cellActinMax = 1000;
    constexpr double remeshActivation = 0.5;
    constexpr double remeshVegfrMin = 5;
    constexpr int remeshTime = 100;
    constexpr int unmeshTime = 100;
    constexpr int retractionTime = 200;
    constexpr int stopRetractionDistance = 1;
}  // namespace Parameters::Defaults

class AceParameters
{
public:
    /*
     * Return Singleton Object.
     * @return  Singleton AceParameters Object
     */
    static AceParameters &Instance()
    {
        static AceParameters instance;
        return instance;
    }

    /*
     * Delete copy constructor.
     */
    AceParameters(AceParameters const &) = delete;

    /*
     * Delete copy assignment operator.
     */
    AceParameters operator=(AceParameters const &) = delete;

    /*
     * Delete move constructor.
     */
    AceParameters(AceParameters const &&) = delete;

    /*
     * Delete move assignment operator.
     */
    AceParameters operator=(AceParameters const &&) = delete;

    /*
     * Parse Options from Command Line.
     * Sets the helpRequested flag to True if help was requested.
     * @return  True if no error occurred, false otherwise.
     */
    bool parseCommandLineOptions(int argc, char *argv[])
    {
        try {
            app.parse(argc, argv);
        } catch (const CLI::ParseError &error) {
            if (app.exit(error) == static_cast<int>(CLI::ExitCodes::Success)) {
                helpRequested = true;
            }
            return false;
        }

        return true;
    }

    /*
     * Check if help option was passed to CLI.
     * @return  True if help requested, otherwise False.
     */
    bool wasHelpRequested() const { return helpRequested; }

    // ---  Methods to get Options --- //

    /*
     * @return  Number of simulation timesteps.
     */
    int getTimesteps() const { return timesteps; }

    int getRunNumber() const { return runNumber; }

    /* percentage of spring's VEGFR total that must be activated to be able to attempt extending */
    float extensionActivation = Parameters::Defaults::extensionActivation;

    /* Minimum amount of active VEGFR that spring must have to be able to attempt extension */
    float extensionActivationMin = Parameters::Defaults::extensionActivationMin;

    /* +/- radians filopodia extension angle can vary from
     * previous extension angle */
    float filopodiaExtensionAngleVariation = Parameters::Defaults::filopodiaExtensionAngleVariation;

    /* Maximum actin cell can use. Determines max collective
     * length of cell's filopodia */
    float cellActinMax = Parameters::Defaults::cellActinMax;

    /* Percentage of VEGFR that must be activated to warrant remeshing */
    float remeshActivation = Parameters::Defaults::remeshActivation;

    /* Minimum amount of active VEGFR that spring must have to warrant remeshing */
    float remeshVegfrMin = Parameters::Defaults::remeshVegfrMin;

    /* Amount of timesteps that remesh requirements must be met before remeshing */
    int remeshTime = Parameters::Defaults::remeshTime;

    /* Amount of timesteps that remesh requirements must be met before remeshing */
    int unmeshTime = Parameters::Defaults::unmeshTime;

    /* Amount of timesteps of inactivity until filopodia tip starts retracting */
    int retractionTime = Parameters::Defaults::unmeshTime;

    /* Distance from original start point to stop retraction at */
    int stopRetractionDistance = Parameters::Defaults::stopRetractionDistance;

private:
    /*
     * Private Constructor to prevent non-singleton initialisation.
     *
     * Declares the built-in CLI options of AceParameters.
     */
    AceParameters() { declareCommandLineParametersInternal(); }

    /*
     * CLI11 Instance;
     */
    CLI::App app {"ACE Model CLI Parameters"};

    /*
     * Declare the built-in CLI options of AceParameters.
     */
    void declareCommandLineParametersInternal()
    {
        // TODO: Enforce type validity, e.g. timesteps should be within [UINT_MIN, UINTMAX]
        app.add_option("-t,--timesteps", timesteps, "The number of timesteps to run Simulation for");
        app.add_option("-n,--run-number", runNumber, "Run number to identify outputs from parallel runs");
    }

    bool helpRequested = false; /**< Flag indicating CLI help requested */

    // --- CLI Option Variables --- //
    int timesteps = Parameters::Defaults::timesteps; /**< Number of Timesteps */
    int runNumber = Parameters::Defaults::runNumber; /**< Run number */
};

#endif  // PARAMETERS_H
