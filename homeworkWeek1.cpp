
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

    for( int useLowFidelity = 0; useLowFidelity < 2; useLowFidelity++ )
    {
        std::string jsonFileName;

        if( useLowFidelity == 0 )
        {
            jsonFileName = "week1JsonInput.json";
        }
        else
        {
            jsonFileName = "week1JsonInputLowFidelity.json";
        }

        JsonSimulationManager< > jsonSimulationManager( cppFolder + jsonFileName );

        const std::string outputDirectory = getOutputPath( ) + "LROIntegrationResults/";

        // *********************** DEFINE NUMBER OF TESTS THAT YOU WILL RUN ************************
        for( unsigned int propagatorType = 1; propagatorType <= 4; propagatorType++ )
        {
            if( propagatorType == 1 )
            {
                jsonSimulationManager[ "propagators" ][ 0 ][ "type" ] = "cowell";
            }
            else if( propagatorType == 2 )
            {
                jsonSimulationManager[ "propagators" ][ 0 ][ "type" ] = "encke";
            }
            else if( propagatorType == 3 )
            {
                jsonSimulationManager[ "propagators" ][ 0 ][ "type" ] = "gaussKeplerian";
            }
            else if( propagatorType == 4 )
            {
                jsonSimulationManager[ "propagators" ][ 0 ][ "type" ] = "gaussModifiedEquinoctial";
            }

            jsonSimulationManager[ "bodies" ][ "LRO" ][ "initialState" ][ "argumentOfPeriapsis" ] =
                    static_cast< double >( 0 * 36  ) * mathematical_constants::PI / 180.0;
            jsonSimulationManager[ "bodies" ][ "LRO" ][ "initialState" ][ "longitudeOfAscendingNode" ] =
                    static_cast< double >( 0 * 36  ) * mathematical_constants::PI / 180.0;
            jsonSimulationManager[ "bodies" ][ "LRO" ][ "initialState" ][ "trueAnomaly" ] =
                    static_cast< double >( 0 * 36  ) * mathematical_constants::PI / 180.0;
            jsonSimulationManager[ "export" ][ 0 ][ "file" ] =
                    outputDirectory + "keplerOutput_" + std::to_string( propagatorType ) + "_" + std::to_string( useLowFidelity ) + ".dat";
            jsonSimulationManager[ "export" ][ 1 ][ "file" ] =
                    outputDirectory + "cartesianOutput_" +  std::to_string( propagatorType ) +  "_" + std::to_string( useLowFidelity ) + ".dat";

            // Create settings objects
            jsonSimulationManager.updateSettings( );

            // Propagate
            jsonSimulationManager.runPropagation( );

            // Export results
            jsonSimulationManager.exportResults( );

            // Silence unused key warnings after first propagation
            jsonSimulationManager[ "options" ][ "unusedKey" ] = tudat::json_interface::continueSilently;

            // Generate reference orbit file
            {
                double moonGravitationalParameter =
                        jsonSimulationManager.getBodyMap( ).at( "Moon" )->getGravityFieldModel( )->getGravitationalParameter( );
                std::map< double, Eigen::Vector6d > referenceKeplerOrbit;
                std::map< double, Eigen::VectorXd > cartesianResults =
                        jsonSimulationManager.getDynamicsSimulator( )->getEquationsOfMotionNumericalSolution( );

                Eigen::Vector6d initialCartesianElements = cartesianResults.begin( )->second;
                Eigen::Vector6d initialKeplerElements = orbital_element_conversions::convertCartesianToKeplerianElements(
                            initialCartesianElements, moonGravitationalParameter );
                double initialTime = cartesianResults.begin( )->first;

                for( std::map< double, Eigen::VectorXd >::const_iterator stateIterator = cartesianResults.begin( );
                     stateIterator != cartesianResults.end( ); stateIterator++ )
                {
                    referenceKeplerOrbit[ stateIterator->first ] =
                            orbital_element_conversions::convertKeplerianToCartesianElements(
                                orbital_element_conversions::propagateKeplerOrbit(
                                    initialKeplerElements, stateIterator->first - initialTime, moonGravitationalParameter ),
                                moonGravitationalParameter );
                }
                input_output::writeDataMapToTextFile(
                            referenceKeplerOrbit, outputDirectory + "referenceOrbit_" +  std::to_string( propagatorType ) +  "_" + std::to_string( useLowFidelity ) + ".dat" );
            }


        }
    }
}

