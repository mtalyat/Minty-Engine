// 2023-12-14 04:05:49
#include <Minty.h>
#include <filesystem>
#include "../Assets/Scripts/init.h"
int main(int argc, char const* argv[]) {
	std::filesystem::current_path("C:/Users/mitch/source/repos/Minty-Engine/Projects/Tests/TestProject");
	minty::console::log(std::filesystem::current_path().string());
	minty::Info info("TestProject - HARD CODED NAME", 0, 0, 0);
	minty::Runtime rt(info);
	init(rt);
	int result = rt.run();
	destroy(rt);
	return result;
}