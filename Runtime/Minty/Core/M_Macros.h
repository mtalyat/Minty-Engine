#pragma once

#include "Minty/Tools/M_Debug.h"

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

#define BIT(x) (1 << x)

#define MINTY_BIND_EVENT_FUNCTION(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

// Deletes and sets the given pointer to null.
#define MINTY_DELETE(pointer) if((pointer)) { delete (pointer); (pointer) = nullptr; }

// Deletes and sets the given pointer to null, if the condition is true.
#define MINTY_DELETE_COND(pointer, condition) if((pointer) && (condition)) { delete (pointer); (pointer) = nullptr; }

//#define MINTY_DEBUG_INFO(message) std::format("{}/{}()/{} -> {}", Minty::Path(__FILE__).filename().string(), __func__, __LINE__, message)
#define MINTY_DEBUG_INFO(message) std::format("[{}][{}()][line {}] -> {}", Minty::Path(__FILE__).filename().string(), __func__, __LINE__, message)

#define MINTY_MAKE_VERSION(major, minor, patch) ((((uint32_t)(major)) << 22U) | (((uint32_t)(minor)) << 12U) | ((uint32_t)(patch)))    

#ifdef MINTY_RELEASE
#define MINTY_ABORT(message) { Minty::Debug::log_error(std::format("FATAL ERROR: {}", MINTY_DEBUG_INFO(message))); Minty::Debug::log_stack_trace(); }
#else
#define MINTY_ABORT(message) { Minty::Debug::log_error(std::format("FATAL ERROR: {}", MINTY_DEBUG_INFO(message))); Minty::Debug::log_stack_trace(); }
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

#define MINTY_ERROR(message) Minty::Debug::log_error(message)
#define MINTY_ERROR_FORMAT(message, ...) Minty::Debug::log_error(std::format(message, __VA_ARGS__))

#define MINTY_WARN(message) Minty::Debug::log_warning(message)
#define MINTY_WARN_FORMAT(message, ...) Minty::Debug::log_warning(std::format(message, __VA_ARGS__))

#define MINTY_LOG(message) Minty::Debug::log(message)
#define MINTY_LOG_FORMAT(message, ...) Minty::Debug::log(std::format(message, __VA_ARGS__))

#define MINTY_INFO(message) Minty::Debug::log_info(message)
#define MINTY_INFO_FORMAT(message, ...) Minty::Debug::log_info(std::format(message, __VA_ARGS__))

#define MINTY_TODO(message) Minty::Debug::log_todo(message)
#define MINTY_TODO_FORMAT(message, ...) Minty::Debug::log_todo(std::format(message, __VA_ARGS__))