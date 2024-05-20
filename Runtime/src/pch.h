// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include <string>
#include <cstring>
#include <cstdint>
#include <cstdlib>

#include <climits>
#include <optional>
#include <functional>
#include <stdexcept>
#include <chrono>
#include <algorithm>
#include <future>

#include <type_traits>
#include <typeindex>
#include <typeinfo>

#include <filesystem>
#include <fstream>

#include <array>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

#include "framework.h"

#include "libraries/M_EnTT.h"
#include "libraries/M_GLFW.h"
#include "libraries/M_GLM.hpp"
#include "libraries/M_Mono.h"
#include "libraries/M_TinyXML.h"
#include "libraries/M_Vulkan.h"

#endif //PCH_H
