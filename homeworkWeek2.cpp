
/*    Copyright (c) 2010-2017, Delft University of Technology
 *    All rigths reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 */

#include <Tudat/JsonInterface/jsonInterface.h>
#include "Tudat/JsonInterface/UnitTests/unitTestSupport.h"


#include <boost/lexical_cast.hpp>

using namespace tudat::json_interface;
using namespace tudat::interpolators;
using namespace tudat;

//! Get path for output directory.
static inline std::string getOutputPath(
        const std::string& extraDirectory = "" )
{
    // Declare file path string assigned to filePath.
    // __FILE__ only gives the absolute path of the header file!
    std::string filePath_( __FILE__ );

    // Strip filename from temporary string and return root-path string.
    std::string reducedPath = filePath_.substr( 0, filePath_.length( ) -
                                                std::string( "homeworkWeek1.cpp" ).length( ) );
    std::string outputPath = reducedPath + "SimulationOutput/";
    if( extraDirectory != "" )
    {
        outputPath += extraDirectory;
    }

    if( outputPath.at( outputPath.size( ) - 1 ) != '/' )
    {
        outputPath += "/";
    }

    return outputPath;
}

//! Execute propagation of orbits of Apollo during entry using the JSON Interface.
int main( )
{
    const std::string cppFilePath( __FILE__ );
    const std::string cppFolder = cppFilePath.substr( 0, cppFilePath.find_last_of("/\\") + 1 );

    std::string jsonFileName = "week2JsonInput.json";
    JsonSimulationManager< > jsonSimulationManager( cppFolder + jsonFileName );

    // First, run low-tolerance ABM, which is then used to create the interpolator
    jsonSimulationManager["integrator"]["relativeErrorTolerance"] = 1e-12;

    const std::string outputDirectory = getOutputPath( ) + "ABM/";

    jsonSimulationManager[ "bodies" ][ "LRO" ][ "initialState" ][ "argumentOfPeriapsis" ] =
            static_cast< double >( 3 * 36  ) * mathematical_constants::PI / 180.0;
    jsonSimulationManager[ "bodies" ][ "LRO" ][ "initialState" ][ "longitudeOfAscendingNode" ] =
            static_cast< double >( 4 * 36  ) * mathematical_constants::PI / 180.0;
    jsonSimulationManager[ "bodies" ][ "LRO" ][ "initialState" ][ "trueAnomaly" ] =
            static_cast< double >( 7 * 36  ) * mathematical_constants::PI / 180.0;
    jsonSimulationManager[ "export" ][ 0 ][ "file" ] =
            outputDirectory + "keplerOutput_" + "cowell" + "_" + std::to_string( 0 ) + ".dat";
    jsonSimulationManager[ "export" ][ 1 ][ "file" ] =
            outputDirectory + "modifiedEquinoctialOutput_" + "cowell" + "_" + std::to_string( 0 ) + ".dat";
    jsonSimulationManager[ "export" ][ 2 ][ "file" ] =
            outputDirectory + "cartesianOutput_" + "cowell" +  "_" + std::to_string( 0 ) + ".dat";

    // Create settings objects
    jsonSimulationManager.updateSettings( );

    // Propagate
    jsonSimulationManager.runPropagation( );

    // Extract state map from propagation results
    std::map< double, Eigen::VectorXd > stateMap;
    stateMap = jsonSimulationManager.getDynamicsSimulator()->getEquationsOfMotionNumericalSolution();

    boost::shared_ptr< InterpolatorSettings > interpolatorSettings = boost::make_shared< LagrangeInterpolatorSettings >(8);
    boost::shared_ptr< OneDimensionalInterpolator< double, Eigen::VectorXd > >  interpolator =
            createOneDimensionalInterpolator(stateMap, interpolatorSettings);

    std::map< double, Eigen::VectorXd > firstDifference;
    std::map< double, Eigen::VectorXd > secondDifference;

    for (int i = 0; i < 2; i++)
    {
        if (i == 0)
        {
            jsonSimulationManager["propagators"][0]["type"] = "cowell";
        }
        else
        {
            jsonSimulationManager[ "propagators" ][ 0 ][ "type" ] = "gaussKeplerian";
        }
        jsonSimulationManager["integrator"] =
                R"(
                    {
                    "type": "rungeKutta4",
                    "stepSize": 30
                    }
                )"_json;
        jsonSimulationManager.updateSettings();
        jsonSimulationManager.runPropagation();

        std::map< double, Eigen::VectorXd > results;
        results = jsonSimulationManager.getDynamicsSimulator()->getEquationsOfMotionNumericalSolution();

        // Compute difference between this and benchmarking solution
        for (std::map< double, Eigen::VectorXd >::iterator iter = results.begin(); iter != results.end(); iter++)
        {
            Eigen::Vector6d benchmark = interpolator->interpolate(iter->first);
            Eigen::Vector6d compare = iter->second;

            if (i == 0)
            {
                firstDifference[iter->first] = compare - benchmark;
            }
            else
            {
                secondDifference[iter->first] = compare - benchmark;
            }
        }

    }

    // Export results
    //jsonSimulationManager.exportResults( );

    //input_output::writeDataMapToTextFile(stateMap, "benchmark.dat", outputDirectory);
    input_output::writeDataMapToTextFile(firstDifference, "first.dat", outputDirectory);
    input_output::writeDataMapToTextFile(secondDifference, "second.dat", outputDirectory);
}

