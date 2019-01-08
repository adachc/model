#ifndef MODELCREATOR_H
#define MODELCREATOR_H

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <ctype.h>

class ModelCreator
{
public:
    ModelCreator(std::string path, std::string processName);

    bool createFolders();
    void createFile();

private:
    std::string path;
    std::string modelName;
    std::string processName;
    std::string pathToBaseProcess;

    void writeHeaderFile();
    void writeSourceFile();
    void addStructToDataClass();
    void createCmakeFile();
    void writeMainFile();

    //Helper-classes
};

#endif // MODELCREATOR_H
