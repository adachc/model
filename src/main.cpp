#include <iostream>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <boost/program_options.hpp>
#include "modelcreator.h"

/*
 *
 * Program options für:
 *  - Name Simulation (?)
 *  - Name Model
 *
 * */

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
            ("simulationName", po::value<std::string>(), "Name of the Simulation this model will be part of")
            ("modelName", po::value<std::string>(), "Name of the model")
            ("path", po::value<std::string>(), "Path in which the Simulation is located");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("simulationName"))
    {
        if(vm.count("modelName"))
        {
            if(vm.count("path"))
            {
                std::string simulationName = vm["simulationName"].as<std::string>();
                std::string processName = vm["modelName"].as<std::string>();
                std::string path = vm["path"].as<std::string>();

                std::cout << "Generate Model: " << std::endl;

                ModelCreator mc(path, processName);
                bool isFilesGeneratedOk = false;
                isFilesGeneratedOk = mc.createFolders();
                if(isFilesGeneratedOk)
                {
                    mc.createFile();
                }
                else
                {
                    std::cout << "Did not create files because folders were not created successfully" << std::endl;
                }
            }
            else
            {
                std::cout << "Path was not set, --path <path> required" << std::endl;
            }
        }
        else
        {
            std::cout << "Modelname was not set, --modelName <name> required" << std::endl;
        }
    }
    else
    {
        std::cout << "Simulationname was not set, --simulationName <name> required" << std::endl;
    }

    return 0;
}

