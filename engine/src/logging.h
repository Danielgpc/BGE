#ifndef BGE_LOGGING_H
#define BGE_LOGGING_H

/**
 * @file logging.h
 * @brief Engine logging facade on top of simple-cpp-logger.
 *
 * Exposes fmt-style macros (LogInfo, LogError, ...) so call sites can write
 * LogError("failed to load {}", path) instead of streaming values by hand.
 * The message is rendered into a std::string first and then handed to the
 * underlying logger, which owns prefixes, colors and output routing.
 */

#include <Logger.h>

#include <cstddef>
#include <sstream>
#include <string>
#include <utility>

namespace bge {
namespace log {

/**
 * @brief Initializes the underlying logger (called once by BGE::init()).
 *
 * The defaults from LoggerParameters.h are used: full verbosity, production
 * prefix format. Safe to call more than once.
 */
inline void init() {}

/** @brief Flushes/tears the logger down (called once by BGE::shutdown()). */
inline void shutdown() {}

namespace detail {

/** @brief Base case: append everything that is left of the format string. */
inline void substitute(std::ostringstream &os, const std::string &fmt,
                       std::size_t from) {
  os << fmt.substr(from);
}

/**
 * @brief Replaces the first "{}" at or after @p from with @p value, then
 *        recurses for the remaining arguments.
 *
 * If the format string runs out of placeholders the extra arguments are
 * dropped; leftover "{}" without a matching argument are printed verbatim.
 */
template <typename T, typename... Rest>
void substitute(std::ostringstream &os, const std::string &fmt,
                std::size_t from, T &&value, Rest &&...rest) {
  const std::size_t placeholder = fmt.find("{}", from);
  if (placeholder == std::string::npos) {
    os << fmt.substr(from);
    return;
  }
  os << fmt.substr(from, placeholder - from) << std::forward<T>(value);
  substitute(os, fmt, placeholder + 2, std::forward<Rest>(rest)...);
}

} // namespace detail

/**
 * @brief Renders a "{}"-style format string with the given arguments.
 *
 * @param fmt Format string; each "{}" consumes one argument in order.
 * @param args Values substituted into @p fmt.
 * @return The fully rendered message.
 */
template <typename... Args>
std::string format(const char *fmt, Args &&...args) {
  std::ostringstream os;
  detail::substitute(os, std::string(fmt), 0, std::forward<Args>(args)...);
  return os.str();
}

} // namespace log
} // namespace bge

// The simple-cpp-logger macros are object-like (stream style). Redefine them
// as fmt-style function macros; everything else (LogInfoIf, LogInfoOnce, ...)
// keeps the original behaviour.
#undef LogFatal
#undef LogError
#undef LogAlert
#undef LogWarning
#undef LogInfo
#undef LogDebug
#undef LogTrace

#define LogFatal(...)                                                         \
  (LogFatalImpl(true, false) << bge::log::format(__VA_ARGS__) << std::endl)
#define LogError(...)                                                         \
  (LogErrorImpl(true, false) << bge::log::format(__VA_ARGS__) << std::endl)
#define LogAlert(...)                                                         \
  (LogAlertImpl(true, false) << bge::log::format(__VA_ARGS__) << std::endl)
#define LogWarning(...)                                                       \
  (LogWarningImpl(true, false) << bge::log::format(__VA_ARGS__) << std::endl)
#define LogInfo(...)                                                          \
  (LogInfoImpl(true, false) << bge::log::format(__VA_ARGS__) << std::endl)
#define LogDebug(...)                                                         \
  (LogDebugImpl(true, false) << bge::log::format(__VA_ARGS__) << std::endl)
#define LogTrace(...)                                                         \
  (LogTraceImpl(true, false) << bge::log::format(__VA_ARGS__) << std::endl)

#endif // BGE_LOGGING_H
