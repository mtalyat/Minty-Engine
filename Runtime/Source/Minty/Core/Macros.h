#pragma once

#include "Minty/Core/Path.h"
#include "Minty/Debug/Debug.h"

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

// force vulkan for rendering for now
#define MINTY_VULKAN

// if debugging and using Visual Studio, insert debug break
#if defined(MINTY_DEBUG)

#if defined(_MSC_VER)
#define MINTY_BREAK() __debugbreak()
#else
#define MINTY_BREAK()
#endif

#else
#define MINTY_BREAK()
#endif

#define BIT(x) (1 << x)

#define MINTY_BIND_EVENT_FUNCTION(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

#define MINTY_ENUM_FLAGS_OPERATORS(type) inline type operator|(type const left, type const right) { return static_cast<type>(static_cast<Size>(left) | static_cast<Size>(right)); } \
inline type operator|=(type& left, type const right) { left = left | right; return left; } \
inline type operator&(type const left, type const right) { return static_cast<type>(static_cast<Size>(left) & static_cast<Size>(right)); } \
inline type operator&=(type& left, type const right) { left = left & right; return left; } \
inline type operator~(type const value) { return static_cast<type>(~static_cast<Size>(value)); } \
inline Bool operator!(type const value) { return static_cast<Size>(value) == 0; } \
inline Bool operator<(type const left, type const right) { return static_cast<Size>(left) < static_cast<Size>(right); } \
inline Bool operator>(type const left, type const right) { return static_cast<Size>(left) > static_cast<Size>(right); }

#define MINTY_DEBUG_INFO(message) std::format("[{}][{}()][line {}] -> {}", Minty::Path(__FILE__).filename().string(), __func__, __LINE__, message)

#define MINTY_MAKE_VERSION(major, minor, patch) ((((uint32_t)(major)) << 22U) | (((uint32_t)(minor)) << 12U) | ((uint32_t)(patch)))

#ifdef MINTY_RELEASE
#define MINTY_ABORT(message) { Minty::Debug::log_error(std::format("FATAL ERROR: {}", MINTY_DEBUG_INFO(message))); Minty::Debug::log_stack_trace(); std::exit(-1); }
#define MINTY_ABORT_FORMAT(message, ...) { Minty::Debug::log_error(std::format("FATAL ERROR: {}", MINTY_DEBUG_INFO(std::format(message, __VA_ARGS__)))); Minty::Debug::log_stack_trace(); std::exit(-1); }
#else
#define MINTY_ABORT(message) { Minty::Debug::log_error(std::format("FATAL ERROR: {}", MINTY_DEBUG_INFO(message))); Minty::Debug::log_stack_trace(); std::exit(-1); }
#define MINTY_ABORT_FORMAT(message, ...) { Minty::Debug::log_error(std::format("FATAL ERROR: {}", MINTY_DEBUG_INFO(std::format(message, __VA_ARGS__)))); Minty::Debug::log_stack_trace(); std::exit(-1); }
#endif

#ifdef MINTY_RELEASE
#define MINTY_NOT_IMPLEMENTED()
#else
#define MINTY_NOT_IMPLEMENTED() { Minty::Debug::log_error(MINTY_DEBUG_INFO("Not implemented.")); std::exit(-1); }
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