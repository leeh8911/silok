#pragma once

#include <memory>
#include <string>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace silok::logger
{
inline spdlog::level::level_enum stringToLevel(const std::string& level)
{
    if (level == "trace")
        return spdlog::level::trace;
    else if (level == "debug")
        return spdlog::level::debug;
    else if (level == "info")
        return spdlog::level::info;
    else if (level == "warn")
        return spdlog::level::warn;
    else if (level == "error")
        return spdlog::level::err;
    else if (level == "critical")
        return spdlog::level::critical;
    else
        return spdlog::level::off;  // Default to off for unknown levels
}

class Logger
{
 public:
    static void init(const std::string& name = "silok", const std::string& level = "trace",
                     const std::string& flush_on = "warn")
    {
        if (!spdlog::get(name))
        {
            logger = spdlog::stdout_color_mt(name);
        }
        else
        {
            logger = spdlog::get(name);
        }

        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%f] %^[%l]%$ [%s:%#:%!] [thread %t] %v");
        logger->set_level(stringToLevel(level));
        logger->flush_on(stringToLevel(flush_on));
    }

    static std::shared_ptr<spdlog::logger> get()
    {
        if (!logger)
        {
            init();
        }
        return logger;
    }

 private:
    inline static std::shared_ptr<spdlog::logger> logger{nullptr};
};

}  // namespace silok::logger

#define SILOK_LOG_TRACE(...) \
    SPDLOG_LOGGER_CALL(silok::logger::Logger::get(), spdlog::level::trace, __VA_ARGS__)
#define SILOK_LOG_DEBUG(...) \
    SPDLOG_LOGGER_CALL(silok::logger::Logger::get(), spdlog::level::debug, __VA_ARGS__)
#define SILOK_LOG_INFO(...) \
    SPDLOG_LOGGER_CALL(silok::logger::Logger::get(), spdlog::level::info, __VA_ARGS__)
#define SILOK_LOG_WARN(...) \
    SPDLOG_LOGGER_CALL(silok::logger::Logger::get(), spdlog::level::warn, __VA_ARGS__)
#define SILOK_LOG_ERROR(...) \
    SPDLOG_LOGGER_CALL(silok::logger::Logger::get(), spdlog::level::err, __VA_ARGS__)
#define SILOK_LOG_CRITICAL(...) \
    SPDLOG_LOGGER_CALL(silok::logger::Logger::get(), spdlog::level::critical, __VA_ARGS__)
