#include "ME_Application.h"

#include <Minty.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>
#include <filesystem> // for accessing file system
#include <fstream> // for accessing files
#include <chrono> // for time
#include <format>

#include <cstdio>
//#include <iostream>
#include <memory>
#include <stdexcept>
//#include <string>
#include <array>

#define CMAKE_PATH "C:/Users/mitch/source/repos/Minty-Engine/Engine/cmake/bin/cmake.exe"
#define APPLICATION_NAME "TestProject"
#define EXE_NAME std::string(APPLICATION_NAME).append(".exe")

using namespace mintye;
using namespace minty;

// A quick way to split strings separated via spaces.
std::vector<std::string> splitString(std::string s)
{
	std::stringstream ss(s);
	std::string word;
	std::vector<std::string> words;
	while (ss >> word) {
		words.push_back(word);
	}
	return words;
}

Application::Application()
{
}

void Application::run(int argc, char const* argv[])
{
	std::cout << "Enter path to project: " << std::endl;
	//std::string path;
	//std::getline(std::cin, path);
	std::string path = "C:/Users/mitch/source/repos/Minty-Engine/Projects/Tests/TestProject"; // hard coded for now, for testing purposes only
	std::cout << path << std::endl;
	path = std::filesystem::absolute(path).string();

	// if folder does not exist, do nothing
	if (!std::filesystem::exists(path))
	{
		std::cout << "Path does not exist: " << path << std::endl;
		return;
	}

	// create new project with path
	minty::Project project(path);

	// create info for commands
	Info info
	{
		.project = project,
		.debug = true, // debug by default
	};

	// create cmake file for building and other commands
	generate_cmake(info);

	// set current working directory to project folder
	//std::filesystem::current_path(path);

	bool running = true;
	while (running)
	{
		// print info
		std::cout << std::endl << "Info:" << std::endl <<
			"Config: " << info.getConfig() << std::endl;

		// print commands
		std::cout << std::endl <<
			" ______________ Commands ______________ " << std::endl <<
			"| test ......... test run the runtime  |" << std::endl <<
			"| debug ........ set config to debug   |" << std::endl <<
			"| release ...... set config to release |" << std::endl <<
			"| clean ........ clean project         |" << std::endl <<
			"| build ........ build the project     |" << std::endl <<
			"| rebuild ...... clean and build       |" << std::endl <<
			"| run .......... run the executable    |" << std::endl <<
			"| rerun ........ build and run         |" << std::endl <<
			"| all .......... clean, build, and run |" << std::endl <<
			"| quit ......... quit the program      |" << std::endl <<
			"|______________________________________|" << std::endl << std::endl;
		std::string command;
		std::getline(std::cin, command);
		std::vector<std::string> commands = splitString(command);
		
		if (commands.size() == 0)
		{
			running = false;
			break;
		}

		command = commands.at(0);

		// run commands
		for (std::string const& c : commands)
		{
			if (c.compare("test") == 0)
			{
				// pass in executable name, then project name
				char path[256];
				strcpy_s(path, info.project.getBasePath().c_str());
				char const* runtimeArgs[2]
				{
					argv[0],
					path
				};

				Runtime runtime;
				runtime.run(2, runtimeArgs);
			}
			else if (c.compare("debug") == 0)
			{
				if (!info.debug)
				{
					info.debug = true;

					// regenrate cmake file
					generate_cmake(info);
				}
			}
			else if (c.compare("release") == 0)
			{
				if (info.debug)
				{
					info.debug = false;

					// regenerate cmake file
					generate_cmake(info);
				}
			} 
			else if (c.compare("clean") == 0)
			{
				clean(info);
			}
			else if (c.compare("build") == 0)
			{
				build(info);

			}
			else if (c.compare("rebuild") == 0)
			{
				clean(info);
				build(info);
			}
			else if (c.compare("run") == 0)
			{
				run(info);
			}
			else if (c.compare("rerun") == 0)
			{
				build(info);
				run(info);
			}
			else if(c.compare("all") == 0)
			{
				clean(info);
				build(info);
				run(info);
			}
			else if (c.compare("quit") == 0)
			{
				running = false;
				break;
			}
			else
			{
				std::cout << "Did not recognize command: " << command << std::endl;
			}
		}
	}
}

void Application::generate_cmake(Info const& info)
{
	// get path to cmake file
	std::string path = (std::filesystem::path(info.project.getBuildPath()) / "CMakeLists.txt").string();

	// open file to overwrite
	std::ofstream file(path, std::ios::trunc);

	// if not open, error
	if (!file.is_open())
	{
		std::cerr << "could not open cmake file: " << path << std::endl;
		return;
	}

	// get timestamp
	const auto now = std::chrono::system_clock::now();

	// write contents
	file <<
		"# " << std::format("{:%Y-%m-%d %H:%M:%OS}", now) << std::endl <<
		"cmake_minimum_required(VERSION 3.16)" << std::endl <<
		"project(" << APPLICATION_NAME << " LANGUAGES CXX)" << std::endl <<
		"set(CMAKE_CXX_STANDARD 20)" << std::endl <<
		"set(CMAKE_CXX_STANDARD_REQUIRED ON)" << std::endl <<
		"set(CMAKE_CXX_EXTENSIONS OFF)" << std::endl <<
		"set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\\\"MSVCRT\\\")" << std::endl <<
		"add_executable(${PROJECT_NAME} main.cpp)" << std::endl <<
		"target_include_directories(${PROJECT_NAME} PUBLIC C:/Users/mitch/source/repos/Minty-Engine/Runtime)" << std::endl <<
		"target_link_libraries(${PROJECT_NAME} C:/Users/mitch/source/repos/Minty-Engine/Runtime/x64/" << info.getConfig() << "/MintyRuntime.lib)" << std::endl;
}

void Application::clean(Info const& info)
{
	// clean the build
	run_command("cd " + info.project.getBuildPath() + " && " + std::filesystem::absolute(CMAKE_PATH).string() + " --build . --target clean");
}

void Application::build(Info const& info)
{
	std::string command = "cd " + info.project.getBuildPath() + " && " + std::filesystem::absolute(CMAKE_PATH).string();

	// make cmake files if needed
	run_command(command + " .");

	// build program
	run_command(command + " --build . --config " + info.getConfig());
}

void Application::run(Info const& info)
{
	// call executable, pass in project path as argument for the runtime, so it knows what to run
	run_command("cd " + info.project.getBuildPath() + " && cd " + info.getConfig() + " && start " + EXE_NAME + " " + info.project.getBasePath());
}

//https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
void Application::run_command(std::string const& cmd)
{
	std::cout << std::endl << cmd << std::endl;
	std::array<char, 128> buffer;
	//std::string result;
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	// print file contents as a different color
	
	std::string result;
	bool changeColor = true;
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		//result += buffer.data();
		//std::cout << buffer.data();
		result = buffer.data();

		if (changeColor)
		{
			if (result.find("error") != std::string::npos)
			{
				std::cout << "\033[31;40m"; // red
			}
			else if (result.find("warning") != std::string::npos)
			{
				std::cout << "\033[33;40m"; // yellow
			}
			else
			{
				std::cout << "\033[90;40m"; // gray
			}
		}
		
		std::cout << result;

		// if newline, it was the end of the inputted line, so update the color for the next line
		changeColor = result.ends_with('\n');
	}
	// reset colors and add newline
	std::cout << "\033[0m";
	//return result;
}
