#include "pch.h"
#include "M_Resources.h"

#include "M_File.h"
#include "M_Renderer.h"
#include "M_Basic_Vertex.h"

#include <unordered_map>

using namespace minty;
using namespace minty::basic;
using namespace minty::resources;

// if in release, load from virtual file system
// if in debug, load from system files

