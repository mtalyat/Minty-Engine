// 2023-12-29 04:28:07
#include <Minty.h>
#include <string>
#include <format>
#include <filesystem>
#include "../Assets/Scripts/init.h"
int main(int argc, char const* argv[]) {
	std::filesystem::current_path("C:/Users/mitch/source/repos/Minty-Engine/Projects/Tests/Headless");
	minty::console::log(std::filesystem::current_path().string());
	minty::Info info("TestProject - HARD CODED NAME", 0, 0, 0);
	minty::Runtime rt(info);
	if(int code = init(rt)) { minty::console::error(std::format("Failed to init program with error code {}.", code)); return code; }
	int result = rt.run();
	if(int code = destroy(rt)) { minty::console::error(std::format("Failed to destroy program with error code {}.", code)); return code; }
	return result;
}