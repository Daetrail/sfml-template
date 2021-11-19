#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <algorithm>

const std::string CONFIGPATH = "runnerconfig.txt";

const std::string VERSION = "1.0";

enum operations
{
    HELP,
    GETVERSION,

    RUNCMAKE,
    RUNMAKE,
    RUNPROGRAM,

    SETEXECUTABLEPATH,
    SETCMAKECOMMAND,
    SETMAKEDIR
};

enum pathIndex
{
    EXECUTABLEPATH,
    CMAKECOMMAND,
    MAKEDIR
};

void printHelp()
{
    std::cout <<
    "-- Subcommands --" << std::endl <<
    "help --> Displays this subcommand list."
    << std::endl <<
    "version --> Displays info about this program."
    << std::endl <<
    "runcmake --> Runs the CMake command given. Spits out an error if no CMake command is set."
    << std::endl <<
    "runmake --> Runs the makefile on the path given. Spits out an error if no makefile path is set."
    << std::endl <<
    "runprogram --> Runs the executable on the path given. Spits out an error if no executable path is set."
    << std::endl <<
    "setexecutablepath <path-to-executable> --> Sets the executable path for the runprogram subcommand."
    << std::endl <<
    "setcmakecommand <cmake-command> --> Sets the CMake command for the runcmake subcommand."
    << std::endl <<
    "setmakedir <path-to-makefile-directory> --> Sets the makefile directory for the runmake subcommand."
    << std::endl <<
    "--------------------------------------------------------------------------------------------------------"
    << std::endl <<
    "In '" + CONFIGPATH + "',"
    << std::endl <<
    "1st Line: Executable path"
    << std::endl <<
    "2nd Line: CMake command"
    << std::endl << 
    "3rd Line: Makefile directory"
    << std::endl;
}

void printVersion()
{
    std::cout << "-- Runner --" << std::endl <<
    "Made for running CMake, Makefiles and executables."
    << std::endl <<
    "Created by danangthang/archaicfox."
    << std::endl <<
    "Version: " + VERSION
    << std::endl;
}

void openFile(std::fstream& file, std::string path, bool trunc)
{
    if (!trunc)
    {
        file.open(path);
        if (!file.is_open())
        {
            std::cerr << "--- Error: Cannot open file. Please create a file named: '" << CONFIGPATH << "' in the same directory as this program. ---" << std::endl;
            std::exit(-1);
        }
    }
    else
    {
        file.open(path, std::ios::in | std::ios::out | std::ios::trunc);
        if (!file.is_open())
        {
            std::cerr << "--- Error: Cannot open file. Please create a file named: '" << CONFIGPATH << "' in the same directory as this program. ---" << std::endl;
            std::exit(-1);
        }   
    }
}

void getLines(std::fstream& file, std::array<std::string, 3>& lines)
{
    size_t i = 0;
    while(!file.eof() && i < lines.size())
    {
        std::string temp;
        std::getline(file, temp);
        lines[i] = temp;
        ++i;
    }
}

void setContents(std::fstream &file, std::array<std::string, 3> &lines)
{
    openFile(file, CONFIGPATH, true);
    for (auto line : lines)
    {
        file << line << std::endl;
    }
    file.close();
}

int main(int argc, char* argv[])
{
    int currentOperation = -1;

    std::array<std::string, 3> paths;

    std::fstream configFile;
    openFile(configFile, CONFIGPATH, false);
    getLines(configFile, paths);
    configFile.close();

    if (argc == 1)
    {
        std::cerr << "--- Error: Please choose from these subcommands ---" << std::endl;
        currentOperation = HELP;
    }

    if (currentOperation != HELP)
    {
        if (std::string(argv[1]) == "help")
            currentOperation = HELP;
        else if (std::string(argv[1]) == "version")
            currentOperation = GETVERSION;
        else if (std::string(argv[1]) == "runcmake")
            currentOperation = RUNCMAKE;
        else if (std::string(argv[1]) == "runmake")
            currentOperation = RUNMAKE;
        else if (std::string(argv[1]) == "runprogram")
            currentOperation = RUNPROGRAM;
        else if (std::string(argv[1]) == "setexecutablepath")
            currentOperation = SETEXECUTABLEPATH;
        else if (std::string(argv[1]) == "setcmakecommand")
            currentOperation = SETCMAKECOMMAND;
        else if (std::string(argv[1]) == "setmakedir")
            currentOperation = SETMAKEDIR;
        else
        {
            std::cerr << "--- Error: Invalid subcommand. Run 'runner help' for help. ---" << std::endl;
            return EXIT_FAILURE;
        }
    }

    switch (currentOperation)
    {
        case HELP:
            printHelp();
            break;

        case GETVERSION:
            printVersion();
            break;

        case RUNCMAKE:
            if (std::all_of(paths[CMAKECOMMAND].begin(), paths[CMAKECOMMAND].end(), isspace) || paths[CMAKECOMMAND].empty())
            {
                std::cerr << "--- Error: No CMake command is set. Run 'runner help' for help. ---" << std::endl;
                return EXIT_FAILURE;
            }
            system(paths[CMAKECOMMAND].c_str());
            break;
        
        case RUNMAKE:
        {
            if (std::all_of(paths[MAKEDIR].begin(), paths[MAKEDIR].end(), isspace) || paths[MAKEDIR].empty())
            {
                std::cerr << "--- Error: No Makefile directory is set. Run 'runner help' for help. ---" << std::endl;
                return EXIT_FAILURE;
            }
            std::string makeCommand = "make -C " + paths[MAKEDIR];
            system(makeCommand.c_str());
            break;
        }

        case RUNPROGRAM:
        {
            if (std::all_of(paths[EXECUTABLEPATH].begin(), paths[EXECUTABLEPATH].end(), isspace) || paths[EXECUTABLEPATH].empty())
            {
                std::cerr << "--- Error: No Executable path is set. Run 'runner help' for help. ---" << std::endl;
                return EXIT_FAILURE;
            }
            std::string runCommand = "./" + paths[EXECUTABLEPATH];
            system(runCommand.c_str());
            break;
        }

        case SETEXECUTABLEPATH:
            if (argc != 3)
            {
                std::cerr << "--- Error: Invalid arguments to 'setexecutablepath' subcommand. Run 'runner help' for help. ---" << std::endl;
                return EXIT_FAILURE;
            }
            paths[EXECUTABLEPATH] = argv[2];
            setContents(configFile, paths);
            break;

        case SETCMAKECOMMAND:
        {
            if (argc < 3)
            {
                std::cerr << "--- Error: Invalid arguments to 'setcmakecommand' subcommand. Run 'runner help' for help. ---" << std::endl;
                return EXIT_FAILURE;
            }

            std::stringstream command;

            for (size_t i = 2; i < argc; ++i)
            { 
                command << argv[i] << " ";
            }
            paths[CMAKECOMMAND] = command.str();
            setContents(configFile, paths);
            break;   
        }
        case SETMAKEDIR:
            if (argc != 3)
            {
                std::cerr << "--- Error: Invalid arguments to 'setmakedir' subcommand. Run 'runner help' for help. ---" << std::endl;
                return EXIT_FAILURE;
            }
            paths[MAKEDIR] = argv[2];
            setContents(configFile, paths);
            break;
    }

    return 0;
}