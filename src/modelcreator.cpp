#include "modelcreator.h"

ModelCreator::ModelCreator(std::string path, std::string modelName)
{
    this->processName = modelName;
    this->modelName = processName + "Model";
    this->path = path;
}

void ModelCreator::createFile()
{
    bool isSuccess = false;

    try
    {
        boost::filesystem::ofstream(path + "/Models/" + processName + "/src/" + processName+".cpp");
        boost::filesystem::ofstream(path + "/Models/" + processName + "/src/main.cpp");
        boost::filesystem::ofstream(path + "/Models/" + processName + "/incl/" + processName+".h");

        isSuccess = true;
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    if(isSuccess)
    {
        //Write code in files
        writeHeaderFile();
        std::cout << processName + ".h successfully written" << std::endl;
        writeSourceFile();
        std::cout << processName + ".cpp successfully written" << std::endl;
        addStructToDataClass();
        std::cout << "Struct " + processName + " successfully written to data.h" << std::endl;
        createCmakeFile();
        std::cout << "CMakeLists.txt successfully written" << std::endl;
        writeMainFile();
        std::cout << "main.cpp successfully written" << std::endl;
    }
}

bool ModelCreator::createFolders()
{
    if(!boost::filesystem::exists(path))
    {
        std::cout << "Selected directory " << path << " does not exist" << std::endl;
        return false;
    }
    else
    {
        boost::filesystem::create_directories(path + "/Models/"+processName);

        if(boost::filesystem::exists(path+"/incl/baseprocess.h"))
        {
            pathToBaseProcess = path + "/incl/baseprocess.h";
        }
        else
        {
            std::cout << "Baseprocess could not be found, searched at: " << path << "/incl/baseprocess.h" << std::endl;
            return false;
        }

        boost::filesystem::create_directory(path + "/Models/" + processName + "/incl");
        boost::filesystem::create_directory(path + "/Models/" + processName + "/src");
        boost::filesystem::create_directory(path + "/Models/" + processName + "/cfg");

        std::cout << "Directory structure for" << processName << " successfully created." << std::endl;

        return true;
    }
}

void ModelCreator::writeHeaderFile()
{
    std::string fileContent = "";
    std::string capsModelName = boost::to_upper_copy<std::string>(modelName);

    std::string currentIndent = "";

    fileContent += "#ifndef " + capsModelName + "_H" + "\n";
    fileContent += "#define " + capsModelName + "_H" + "\n\n";

    //Includes
    fileContent += "#include  \"../incl/baseprocess.h\"\n"; //wahrscheinlich falsch (Pfad)

    fileContent += "\n";

    fileContent += "namespace ugv\n{\n";
    currentIndent += "  ";

    fileContent += currentIndent + "class " + modelName + " : public BaseProcess" + "\n" + currentIndent + "{\n";
    currentIndent += "  ";
    fileContent += currentIndent + "public:\n";
    currentIndent += "  ";

    //Public
    fileContent += currentIndent + modelName + "(int id, const std::string& name, int duration = 100);\n";
    fileContent += currentIndent + "~" + modelName +  "();\n";
    fileContent += currentIndent + "virtual void initData();\n";
    fileContent += currentIndent + "virtual void update();\n";

    //Private
    currentIndent = currentIndent.substr(0, currentIndent.size()-2);
    fileContent += currentIndent + "private:\n";

    currentIndent = currentIndent.substr(0, currentIndent.size()-2);
    fileContent += currentIndent + "}\n";
    currentIndent = currentIndent.substr(0, currentIndent.size()-2);
    fileContent += currentIndent + "}\n";

    fileContent += "#endif // " + capsModelName + "_H";

    std::ofstream headerFile(path + "/Models/" + processName + "/incl/"+processName+".h");
    headerFile << fileContent;
    headerFile.close();
}

void ModelCreator::writeSourceFile()
{
    std::string fileContent = "";
    std::string currentIndent = "";

    fileContent += "#include \"" + modelName + ".h\"\n\n";
    fileContent += "using namespace ugv;\n\n";

    fileContent += modelName + "::" + modelName + "(int id, const std::string& name, int duration) : BaseProcess(id, name, duration), data(nullptr)\n{\n";
    currentIndent += "  ";
    fileContent += currentIndent + "debug(\"CTOR " + modelName + "\");\n";
    currentIndent = currentIndent.substr(0, currentIndent.size()-2);
    fileContent += "}\n\n";

    fileContent += modelName + "::" + "initData()\n{\n";
    currentIndent += "  ";
    fileContent += currentIndent + "debug(\"initData start ... \");\n";
    fileContent += currentIndent + "data = shmSegment->construct<ugvdata::" + modelName + ">(getName().c_str())();\n";
    fileContent += currentIndent + "if(data == nullptr)\n";
    currentIndent += "  ";
    fileContent += currentIndent + "debug(\"data is null\");\n\n";
    currentIndent = currentIndent.substr(0, currentIndent.size()-2);

    fileContent += currentIndent + "debug(\"... initData finished!\");";
    currentIndent = currentIndent.substr(0, currentIndent.size()-2);
    fileContent += "}\n\n";

    fileContent += "void " + modelName + "::update()\n{";
    fileContent += "}";

    std::ofstream sourceFile(path + "/Models/" + processName + "/src/"+processName+".cpp");
    sourceFile << fileContent;
    sourceFile.close();
}

void ModelCreator::createCmakeFile()
{
    if(!boost::filesystem::exists(path + "/Models/" + processName + "/CMakeLists.txt"))
    {
        boost::filesystem::ofstream(path + "/Models/" + processName + "/CMakeLists.txt");
    }

    std::string fileContent = "";

    fileContent += "cmake_minimum_required(VERSION 3.5)\n";
    fileContent += "find_package(Boost 1.58.0)\n";
    fileContent += "set(CMAKE_C_COMPILER gcc)\n";
    fileContent += "set(CMAKE_CXX_COMPILER /usr/bin/g++)\n";
    fileContent += "set(CMAKE_CXX_STANDARD 14)\n";
    fileContent += "set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -Wall -DBOOST_LOG_DYN_LINK -pedantic\")\n";
    fileContent += "set(CMAKE_CXX_FLAGS_DEBUG \"${CMAKE_CXX_FLAGS_DEBUG} -Wall -DBOOST_LOG_DYN_LINK\")\n";
    fileContent += "FILE(GLOB SOURCES src/*.cpp)\n";
    fileContent += "FILE(GLOB_RECURSE INC_ALL \"incl/*.h\")\n";
    fileContent += "FILE(GLOB_RECURSE INC_ALL \"../incl/*.h\")\n";
    fileContent += "include_directories(incl ../incl/)\n";
    fileContent += "project(" + processName + ")\n";
    fileContent += "add_executable (${PROJECT_NAME} ${SOURCES} ${INC_ALL})\n";
    fileContent += "target_link_libraries(${PROJECT_NAME}\n";
    fileContent += "  ${Boost_LIBRARIES}\n";
    fileContent += "  boost_system\n";
    fileContent += "  rt\n";
    fileContent += "  pthread\n";
    fileContent += "  boost_thread\n";
    fileContent += "  boost_filesystem\n";
    fileContent += "  boost_log\n";
    fileContent += "  boost_log_setup\n";
    fileContent += "  boost_date_time\n";
    fileContent += "  boost_program_options\n";
    fileContent += ")";

    std::ofstream cmakeFile(path + "/Models/" + processName + "/CMakeLists.txt");
    cmakeFile << fileContent;
    cmakeFile.close();
}

void ModelCreator::addStructToDataClass()
{
    if(boost::filesystem::exists(path + "/incl/data.h"))
    {
        std::string currentIndent = "  ";

        std::ifstream ifs(path + "/incl/data.h");
        std::string fileContent((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        if(fileContent.find("struct " + processName) == std::string::npos)
        {
            std::string fileContent0 = fileContent.substr(0,fileContent.find("}\n"));
            std::string fileContent1 = fileContent.substr(fileContent.find("}\n"),std::string::npos);
            fileContent0 += currentIndent + "struct " + processName += "\n";
            fileContent0 += currentIndent + "{\n";
            fileContent0 += currentIndent + "};\n";
            fileContent = fileContent0 + fileContent1;
        }
        std::ofstream sourceFile(path + "/incl/data.h");
        sourceFile << fileContent;
        sourceFile.close();
    }
    else
    {
        std::cout << "Could not find data.h; Searched in: " << path + "/incl/data.h" << std::endl;
    }
}

void ModelCreator::writeMainFile()
{
    std::string fileContent = "";
    std::string currentIndent = "";

    //Includes
    fileContent += "#include \"steering.h\"\n\n";
    fileContent += "int main(int argc, char* argv[])\n";
    fileContent += "{\n";
    currentIndent += " ";
    fileContent += currentIndent + "SteeringModel model\n";
    fileContent += currentIndent + "model.initData();\n";
    currentIndent = "";
    fileContent += "}";

    std::ofstream mainFile(path + "/Models/" + processName + "/src/main.cpp");
    mainFile << fileContent;
    mainFile.close();
}
