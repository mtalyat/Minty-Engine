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

//#define MINTY_INFO(message) std::format("{}/{}()/{} -> {}", minty::Path(__FILE__).filename().string(), __func__, __LINE__, message)
#define MINTY_INFO(message) std::format("[{}][{}()][line {}] -> {}", minty::Path(__FILE__).filename().string(), __func__, __LINE__, message)

#ifdef MINTY_RELEASE
#define MINTY_ABORT(message) minty::Console::error(std::format("FATAL ERROR: {}", MINTY_INFO(message)))
#else
#define MINTY_ABORT(message) throw std::runtime_error(MINTY_INFO(message))
#endif

#ifdef MINTY_RELEASE
#define MINTY_ASSERT(expression)
#define MINTY_ASSERT_MESSAGE(expression, message)
#define MINTY_ASSERT_FORMAT(expression, message, ...)
#else
#define MINTY_ASSERT(expression) if(!(expression)) MINTY_ABORT("(" #expression ") failed.")
#define MINTY_ASSERT_MESSAGE(expression, message) if(!(expression)) MINTY_ABORT("(" #expression ") failed: " #message)
#define MINTY_ASSERT_FORMAT(expression, message, ...) if(!(expression)) MINTY_ABORT("(" #expression ") failed: " + std::format(message, __VA_ARGS__))
#endif

#ifdef MINTY_RELEASE
#define MINTY_ERROR(message)
#define MINTY_ERROR_FORMAT(message, ...)
#else
#define MINTY_ERROR(message) minty::Console::error(message)
#define MINTY_ERROR_FORMAT(message, ...) minty::Console::error(std::format(message, __VA_ARGS__))
#endif

#ifdef MINTY_RELEASE
#define MINTY_WARN(message)
#define MINTY_WARN_FORMAT(message, ...)
#else
#define MINTY_WARN(message) minty::Console::warn(message)
#define MINTY_WARN_FORMAT(message, ...) minty::Console::warn(std::format(message, __VA_ARGS__))
#endif

#ifdef MINTY_RELEASE
#define MINTY_LOG(message)
#define MINTY_LOG_FORMAT(message, ...)
#else
#define MINTY_LOG(message) minty::Console::log(message)
#define MINTY_LOG_FORMAT(message, ...) minty::Console::log(std::format(message, __VA_ARGS__))
#endif