#pragma once

// Optional macros from outside of the runtime:
/*
* MINTY_IMGUI
*/

#ifdef NDEBUG
#define MINTY_RELEASE
#else
#define MINTY_DEBUG
#endif

#ifdef _WIN32
#define MINTY_WINDOWS
#elif defined(__APPLE__)
#define MINTY_APPLE
#elif defined(__linux__)
#define MINTY_LINUX
#else
#error "Unsupported operating system."
#endif

// Deletes and sets the given pointer to null.
#define MINTY_DELETE(pointer) if((pointer)) { delete (pointer); (pointer) = nullptr; }

// Deletes and sets the given pointer to null, if the condition is true.
#define MINTY_DELETE_COND(pointer, condition) if((pointer) && (condition)) { delete (pointer); (pointer) = nullptr; }