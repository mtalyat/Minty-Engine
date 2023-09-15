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

#define CMAKE_PATH "C:/Users/mitch/source/repos/Minty-Engine/Engine/cmake/bin/cmake.exe"
#define EXE_NAME "MyProgram.exe"

using namespace mintye;
using namespace minty;

// A quick way to split strings separated via spaces.
std::vector<std::string> split_string(std::string s)
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
	std::string path;
	std::getline(std::cin, path);
	path = std::filesystem::absolute(path).string();

	// if folder does not exist, do nothing
	if (!std::filesystem::exists(path))
	{
		std::cout << "Path does not exist: " << path << std::endl;
		return;
	}

	// set current working directory to project folder
	std::filesystem::current_path(path);

	bool running = true;
	while (running)
	{
		std::cout << std::endl << "Enter command(s): (clean | build | rebuild | run | all | quit)" << std::endl;
		std::string command;
		std::getline(std::cin, command);
		std::vector<std::string> commands = split_string(command);
		
		if (commands.size() == 0)
		{
			running = false;
			break;
		}

		command = commands.at(0);

		// create new project with path
		minty::Project project(path);

		// run commands
		for (std::string const& c : commands)
		{
			if (c.compare("clean") == 0)
			{
				clean(project);
			}
			else if (c.compare("build") == 0)
			{
				build(project);

			}
			else if (c.compare("run") == 0)
			{
				run(project);
			}
			else if(c.compare("all") == 0)
			{
				clean(project);
				build(project);
				run(project);
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

	minty::Runtime runtime;
	runtime.run(argc, argv);
}

void Application::clean(Project const& project)
{
	// clean the build
	run_command("cd " + project.getBuildPath() + " && " + std::filesystem::absolute(CMAKE_PATH).string() + " --build . --target clean");
}

void Application::build(Project const& project)
{
	std::string command = "cd " + project.getBuildPath() + " && " + std::filesystem::absolute(CMAKE_PATH).string();

	// make cmake files if needed
	run_command(command + " .");

	// build program
	run_command(command + " --build . --config Debug");
}

void Application::run(Project const& project)
{
	// call executable, pass in project path as argument for the runtime, so it knows what to run
	run_command("cd " + project.getBuildPath() + " && cd Debug && start " + EXE_NAME + " " + project.getBasePath());
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
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		//result += buffer.data();
		//std::cout << buffer.data();
		result = buffer.data();

		if (result.find("error") != std::string::npos)
		{
			std::cout << "\033[31;40m"; // red
		} else if (result.find("warning") != std::string::npos)
		{
			std::cout << "\033[33;40m"; // yellow
		}
		else
		{
			std::cout << "\033[90;40m"; // gray
		}
		std::cout << result;
	}
	// reset colors and add newline
	std::cout << "\033[0m";
	//return result;
}
