// 2024-02-26 23:14:48
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
	try {
	runtime.init();
	runtime.get_script_engine().load_assembly("TestProject/bin/x64/Debug/MintyEngine.dll");
	runtime.get_script_engine().load_assembly("TestProject/bin/x64/Debug/TestProject.dll");
	runtime.link();
	if(int code = init(runtime)) { minty::Console::error(std::format("Failed to init program with error code {}.", code)); return code; }
	runtime.start();
	runtime.run();
	runtime.cleanup();
	if(int code = destroy(runtime)) { minty::Console::error(std::format("Failed to destroy program with error code {}.", code)); return code; }
	runtime.destroy();
	} catch (std::exception const& e) { minty::Console::error(std::format("Crash: \"{}\"", e.what())); }
	return runtime.get_exit_code();
}