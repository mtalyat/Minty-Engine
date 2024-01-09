#include "init.h"

#include <iostream>

using namespace minty;
using namespace minty::rendering;

InputMap input;

// called when the engine is initialized
int init(Runtime &runtime)
{
    // create wrap file
    Wrap wrap;

    // add files in Assets folder only, no sub folders
    wrap.emplace("Data/", false, Wrap::Compression::None);

    // check if files are in there
    console::log("pumpkin_tex.jpg: " + std::to_string(wrap.contains("pumpkin_tex.jpg")));
    console::log("pumpkin_tex.jpg.meta: " + std::to_string(wrap.contains("pumpkin_tex.jpg.meta")));
    console::log("pumpkin.obj: " + std::to_string(wrap.contains("pumpkin.obj")));
    console::log("test.txt: " + std::to_string(wrap.contains("test.txt")));
    console::log("Test/test2.txt: " + std::to_string(wrap.contains("Test/test2.txt")));

    // save
    wrap.save("wrap.wrap");

    // load
    wrap.load("wrap.wrap");

    // check if files are in there
    console::log("pumpkin_tex.jpg: " + std::to_string(wrap.contains("pumpkin_tex.jpg")));
    console::log("pumpkin_tex.jpg.meta: " + std::to_string(wrap.contains("pumpkin_tex.jpg.meta")));
    console::log("pumpkin.obj: " + std::to_string(wrap.contains("pumpkin.obj")));
    console::log("test.txt: " + std::to_string(wrap.contains("test.txt")));
    console::log("Test/test2.txt: " + std::to_string(wrap.contains("Test/test2.txt")));

    // do not run app, just this
    return 1;
}

// called when the engine is destroyed
int destroy(Runtime &runtime)
{
    console::log("Game over.");

    return 0;
}