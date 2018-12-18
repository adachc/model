#include <iostream>
#include <string.h>
#include <stdio.h>
#include "modelcreator.h"

/*
 *
 * Dateiname und Pfad aus Config-File
 * Ordern erstellen (Model)
 * Skelett erstellen
 * CMake erstellen
 *
 *
 * TODO:
 *  - Main
 *  - ConfigFile
 *
 * */

int main()
{
    std::cout << "Generate Model: " << std::endl;
    std::string path = "/home/matthias/Test";
    std::string processName = "Steering";

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

    return 0;
}

