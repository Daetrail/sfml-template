#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <algorithm>
#include <bitset>

const std::string CONFIGPATH = "runnerconfig.txt";

const std::string VERSION = "1.1";

enum operations
{
    HELP,
    GETVERSION,

    RUNCMAKE,
    RUNMAKE,
    RUNPROGRAM,

    OUTPUTPATHS,

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

enum colours
{
    RED = 1,
    GREEN = 2,
    BLUE = 4,
    MAGENTA = 8,

    BOLD = 16,
};

enum colourPos
{
    REDPOS,
    GREENPOS,
    BLUEPOS,
    MAGENTAPOS,

    BOLDPOS,
};

class Colour
{
public:
    Colour(std::string text, unsigned int styles)
    : text(text)
    {
        std::bitset<sizeof(unsigned int) * 8> tempstyle = styles;

        if (tempstyle.test(BOLDPOS))
            this->actualStyle = 1;
        else
            this->actualStyle = 0;

        if (tempstyle.test(REDPOS))
            this->colourVal = 91;
        else if (tempstyle.test(GREENPOS))
            this->colourVal = 92;
        else if (tempstyle.test(BLUEPOS))
            this->colourVal = 94;
        else if (tempstyle.test(MAGENTAPOS))
            this->colourVal = 95;
    }

    friend std::ostream& operator << (std::ostream& os, const Colour& colour);
private:
    std::string text;

    unsigned int actualStyle;
    unsigned int colourVal;
};

std::ostream& operator << (std::ostream& os, const Colour& colour)
{
    return os << "\033[" << std::to_string(colour.actualStyle) + ";" + std::to_string(colour.colourVal) << "m" << colour.text << "\033[0m";
}

void printHelp()
{
    std::cout <<
    Colour("-- Subcommands --", BOLD | MAGENTA) << std::endl <<
    Colour("help", BOLD | BLUE) << " --> Displays this subcommand list."
    << std::endl <<
    Colour("version", BOLD | BLUE) << " --> Displays info about this program."
    << std::endl <<
    Colour("runcmake", BOLD | BLUE) << " --> Runs the CMake command given. Spits out an error if no CMake command is set."
    << std::endl <<
    Colour("runmake", BOLD | BLUE) << " --> Runs the makefile on the path given. Spits out an error if no makefile path is set."
    << std::endl <<
    Colour("runprogram", BOLD | BLUE) << " --> Runs the executable on the path given. Spits out an error if no executable path is set."
    << std::endl <<
    Colour("output", BOLD | BLUE) << " --> Outputs the contents in '" + CONFIGPATH + "'."
    << std::endl <<
    Colour("setexecutablepath <path-to-executable>", BOLD | BLUE) << " --> Sets the executable path for the runprogram subcommand."
    << std::endl <<
    Colour("setcmakecommand <cmake-command>", BOLD | BLUE)<< " --> Sets the CMake command for the runcmake subcommand."
    << std::endl <<
    Colour("setmakedir <path-to-makefile-directory>", BOLD | BLUE) << " --> Sets the makefile directory for the runmake subcommand."
    << std::endl << std::endl << 
    Colour("In '" + CONFIGPATH + "',", BOLD | BLUE)
    << std::endl <<
    Colour("1st Line:", BOLD | BLUE) << " Executable path"
    << std::endl <<
    Colour("2nd Line:", BOLD | BLUE) << " CMake command"
    << std::endl << 
    Colour("3rd Line:", BOLD | BLUE) << " Makefile directory"
    << std::endl;
}

void printVersion()
{
    std::cout << Colour("-- Runner --", BOLD | MAGENTA) << std::endl <<
    "Made for running " << Colour("CMake", BOLD | BLUE) << ", " << Colour("Makefiles", BOLD | BLUE) << " and " << Colour("executables", BOLD | BLUE) << "."
    << std::endl <<
    "Created by " << Colour("danangthang/archaicfox", BOLD | MAGENTA) << "."
    << std::endl <<
    Colour("Version: ", BOLD | BLUE) << VERSION
    << std::endl;
}

void openFile(std::fstream& file, std::string path, bool trunc)
{
    if (!trunc)
    {
        file.open(path);
        if (!file.is_open())
        {
            std::cerr << Colour("--- Error: Cannot open file. Please create a file named: '" + CONFIGPATH + "' in the same directory as this program. ---", BOLD | RED) << std::endl;
            std::exit(-1);
        }
    }
    else
    {
        file.open(path, std::ios::in | std::ios::out | std::ios::trunc);
        if (!file.is_open())
        {
            std::cerr << Colour("--- Error: Cannot open file. Please create a file named: '" + CONFIGPATH + "' in the same directory as this program. ---", BOLD | RED) << std::endl;
            std::exit(-1);
        }   
    }
}

void getLines(std::fstream& file, std::array<std::string, 3>& lines)
{
    size_t i = 0;
    while(!file.eof())
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
        std::cerr << Colour("--- Error: Please choose from these subcommands ---", BOLD | RED) << std::endl;
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
        else if (std::string(argv[1]) == "output")
            currentOperation = OUTPUTPATHS;
        else if (std::string(argv[1]) == "setexecutablepath")
            currentOperation = SETEXECUTABLEPATH;
        else if (std::string(argv[1]) == "setcmakecommand")
            currentOperation = SETCMAKECOMMAND;
        else if (std::string(argv[1]) == "setmakedir")
            currentOperation = SETMAKEDIR;
        else
        {
            std::cerr << Colour("--- Error: Invalid subcommand. Run 'runner help' for help. ---", BOLD | RED) << std::endl;
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
                std::cerr << Colour("--- Error: No CMake command is set. Run 'runner help' for help. ---", BOLD | RED) << std::endl;
                return EXIT_FAILURE;
            }
            system(paths[CMAKECOMMAND].c_str());
            break;
        
        case RUNMAKE:
        {
            if (std::all_of(paths[MAKEDIR].begin(), paths[MAKEDIR].end(), isspace) || paths[MAKEDIR].empty())
            {
                std::cerr << Colour("--- Error: No Makefile directory is set. Run 'runner help' for help. ---", BOLD | RED) << std::endl;
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
                std::cerr << Colour("--- Error: No Executable path is set. Run 'runner help' for help. ---", BOLD | RED) << std::endl;
                return EXIT_FAILURE;
            }
            std::string runCommand = "./" + paths[EXECUTABLEPATH];
            system(runCommand.c_str());
            break;
        }

        case OUTPUTPATHS:
            std::cout <<
            Colour("-- Contents of '" + CONFIGPATH + "' --", BOLD | MAGENTA)
            << std::endl <<
            Colour("Executable path: ", BOLD | BLUE) << paths[0]
            << std::endl <<
            Colour("CMake command: ", BOLD | BLUE) << paths[1]
            << std::endl << 
            Colour("Makefile directory: ", BOLD | BLUE) << paths[2]
            << std::endl;
            break;

        case SETEXECUTABLEPATH:
            if (argc != 3)
            {
                std::cerr << Colour("--- Error: Invalid arguments to 'setexecutablepath' subcommand. Run 'runner help' for help. ---", BOLD | RED) << std::endl;
                return EXIT_FAILURE;
            }
            paths[EXECUTABLEPATH] = argv[2];
            setContents(configFile, paths);
            break;

        case SETCMAKECOMMAND:
        {
            if (argc < 3)
            {
                std::cerr << Colour("--- Error: Invalid arguments to 'setcmakecommand' subcommand. Run 'runner help' for help. ---", BOLD | RED) << std::endl;
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
                std::cerr << Colour("--- Error: Invalid arguments to 'setmakedir' subcommand. Run 'runner help' for help. ---", BOLD | RED) << std::endl;
                return EXIT_FAILURE;
            }
            paths[MAKEDIR] = argv[2];
            setContents(configFile, paths);
            break;
    }

    return 0;
}