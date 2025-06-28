#include "silok/infra/logger/spdlog_logger.hpp"

namespace silok::infra
{

SpdlogLogger::SpdlogLogger(const std::string& name, const std::string& level,
                           const std::string& flush_on)
{
    if (!spdlog::get(name))
    {
        logger_ = spdlog::stdout_color_mt(name);
    }
    else
    {
        logger_ = spdlog::get(name);
    }

    logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%f] %^[%l]%$ [%s:%#:%!] [thread %t] %v");
    logger_->set_level(stringToLevel(level));
    logger_->flush_on(stringToLevel(flush_on));
}

spdlog::level::level_enum SpdlogLogger::stringToLevel(const std::string& level)
{
    if (level == "trace") return spdlog::level::trace;
    if (level == "debug") return spdlog::level::debug;
    if (level == "info") return spdlog::level::info;
    if (level == "warn") return spdlog::level::warn;
    if (level == "error") return spdlog::level::err;
    if (level == "critical") return spdlog::level::critical;
    return spdlog::level::off;
}

void SpdlogLogger::Trace(const std::string& msg)
{
    logger_->trace(msg);
}
void SpdlogLogger::Debug(const std::string& msg)
{
    logger_->debug(msg);
}
void SpdlogLogger::Info(const std::string& msg)
{
    logger_->info(msg);
}
void SpdlogLogger::Warn(const std::string& msg)
{
    logger_->warn(msg);
}
void SpdlogLogger::Error(const std::string& msg)
{
    logger_->error(msg);
}
void SpdlogLogger::Critical(const std::string& msg)
{
    logger_->critical(msg);
}

}  // namespace silok::infra
