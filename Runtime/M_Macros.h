#pragma once

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