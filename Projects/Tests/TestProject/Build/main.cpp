// 2024-02-10 20:40:26
#include <Minty.h>
#include <string>
#include <format>
#include <filesystem>
#include "../Assets/init.h"
int main(int argc, char const* argv[]) {
	std::filesystem::current_path("C:/Users/mitch/source/repos/Minty-Engine/Projects/Tests/TestProject");
	minty::Console::log(std::filesystem::current_path().string());
	minty::Info info("TestProject", 0, 0, 0);
	minty::Engine engine(info);
	engine.init();
	if(int code = init(engine)) { minty::Console::error(std::format("Failed to init program with error code {}.", code)); return code; }
	engine.start();
	engine.run();
	engine.cleanup();
	if(int code = destroy(engine)) { minty::Console::error(std::format("Failed to destroy program with error code {}.", code)); return code; }
	engine.destroy();
	return engine.get_exit_code();
}