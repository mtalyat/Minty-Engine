// 2024-02-19 06:48:12
#include <Minty.h>
#include <string>
#include <format>
#include <filesystem>
#include "../Assets/init.h"
int main(int argc, char const* argv[]) {
	std::filesystem::current_path("C:/Users/mitch/source/repos/Minty-Engine/Projects/Tests/TestProject");
	minty::Console::log(std::filesystem::current_path().string());
	minty::Info info("TestProject", 0, 0, 0);
	minty::Runtime runtime(info);
	runtime.init();
	runtime.get_script_engine().load_assembly("Assembly/bin/Debug/MintyEngine.dll");
	runtime.get_script_engine().load_assembly("Assembly/bin/Debug/Assembly.dll");
	minty::ScriptEngine::link();
	if(int code = init(runtime)) { minty::Console::error(std::format("Failed to init program with error code {}.", code)); return code; }
	runtime.start();
	runtime.run();
	runtime.cleanup();
	if(int code = destroy(runtime)) { minty::Console::error(std::format("Failed to destroy program with error code {}.", code)); return code; }
	runtime.destroy();
	return runtime.get_exit_code();
}