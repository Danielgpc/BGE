#ifndef BGE_LOGGING_H
#define BGE_LOGGING_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>
#include <fmt/format.h>

namespace bge {
namespace log {

inline void init() {
    auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console->set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");
    auto logger = std::make_shared<spdlog::logger>("bge", console);
    logger->set_level(spdlog::level::trace);
    spdlog::set_default_logger(logger);
}

inline void shutdown() {
    spdlog::shutdown();
}

} // namespace log
} // namespace bge

// Fmt-style logging macros (recommended)
#define LogTrace(...)   SPDLOG_TRACE(__VA_ARGS__)
#define LogDebug(...)   SPDLOG_DEBUG(__VA_ARGS__)
#define LogInfo(...)    SPDLOG_INFO(__VA_ARGS__)
#define LogWarning(...) SPDLOG_WARN(__VA_ARGS__)
#define LogError(...)   SPDLOG_ERROR(__VA_ARGS__)
#define LogFatal(...)   SPDLOG_CRITICAL(__VA_ARGS__)

// Conditional logging
#define LogTraceIf(cond, ...)   if (cond) SPDLOG_TRACE(__VA_ARGS__)
#define LogDebugIf(cond, ...)   if (cond) SPDLOG_DEBUG(__VA_ARGS__)
#define LogInfoIf(cond, ...)    if (cond) SPDLOG_INFO(__VA_ARGS__)
#define LogWarningIf(cond, ...) if (cond) SPDLOG_WARN(__VA_ARGS__)
#define LogErrorIf(cond, ...)   if (cond) SPDLOG_ERROR(__VA_ARGS__)
#define LogFatalIf(cond, ...)   if (cond) SPDLOG_CRITICAL(__VA_ARGS__)

#endif // BGE_LOGGING_H