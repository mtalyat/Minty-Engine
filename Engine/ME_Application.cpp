#include "ME_Application.h"

#include <Minty.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>

#include <cstdio>
//#include <iostream>
#include <memory>
#include <stdexcept>
//#include <string>
#include <array>

#define CMAKE_PATH "cmake/bin/cmake.exe"
#define EXE_NAME "MyProgram.exe"

using namespace mintye;

//https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
std::string exec(const char* cmd) {
	std::cout << cmd << std::endl;
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

Application::Application()
{
}

void Application::run(int argc, char const* argv[])
{
	std::cout << "Enter path to project: " << std::endl;
	std::string path;
	std::getline(std::cin, path);
	std::cout << std::endl << "Enter command(s): (clean | build | run)" << std::endl;
	std::string commands;
	std::getline(std::cin, commands);

	// create new project with path
	minty::Project project(path);
	std::string cd = "cd " + project.getBuildPath();
	std::string cmake = cd + " && " + std::filesystem::absolute(CMAKE_PATH).string();

	// run commands
	char commandBuffer[255];
	size_t len = commands.size();
	std::string command;
	for (size_t i = 0, j = 0; i <= len; i++)
	{
		if (i >= len || commands.at(i) == ' ')
		{
			if (i - j > 0)
			{
				memcpy(commandBuffer, &commands.data()[j], i - j);
				commandBuffer[i - j] = 0; // set terinating char

				if (strcmp(commandBuffer, "clean") == 0)
				{
					// clean the build
					command = cmake + " --build . --target clean";
					std::cout << exec(command.c_str());
				}
				else if (strcmp(commandBuffer, "build") == 0)
				{
					// make cmake files if needed
					command = cmake + " .";
					std::cout << exec(command.c_str());

					// build program
					command = cmake + " --build .";
					std::cout << exec(command.c_str());
				}
				else if (strcmp(commandBuffer, "run") == 0)
				{
					// call executable
					command = cd + " && cd Debug && start " + EXE_NAME;
					std::cout << exec(command.c_str());
				}
				else
				{
					std::cout << "Did not recognize command: " << commandBuffer << std::endl;
				}
			}
			
			j = i + 1;
		}
	}

	std::cout << "Done!" << std::endl;

	// pause
	std::string dontcare;
	std::getline(std::cin, dontcare);

	//minty::Runtime runtime;
	//runtime.run(argc, argv);
}