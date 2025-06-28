#pragma once

#include <format>
#include <memory>
#include <string>

namespace silok::domain
{

class BaseLogger
{
 public:
    virtual ~BaseLogger() = default;

    virtual void Trace(const std::string& msg) = 0;
    virtual void Debug(const std::string& msg) = 0;
    virtual void Info(const std::string& msg) = 0;
    virtual void Warn(const std::string& msg) = 0;
    virtual void Error(const std::string& msg) = 0;
    virtual void Critical(const std::string& msg) = 0;
};

using BaseLoggerPtr = std::shared_ptr<BaseLogger>;

class Logger
{
 public:
    static BaseLogger& GetLogger()
    {
        if (!instance_ptr)
        {
            throw std::runtime_error("Logger instance is not initialized");
        }
        return *instance_ptr;
    }
    static void SetLogger(BaseLoggerPtr logger)
    {
        if (!logger)
        {
            throw std::runtime_error("Logger instance cannot be null");
        }
        instance_ptr = std::move(logger);
    }

 private:
    inline static BaseLoggerPtr instance_ptr{nullptr};
};

}  // namespace silok::domain

#define SILOK_LOG(level, fmt, ...) \
    silok::domain::Logger::GetLogger().level(std::format(fmt, __VA_ARGS__))

#define SILOK_LOG_TRACE(fmt, ...) SILOK_LOG(Trace, fmt, __VA_ARGS__)
#define SILOK_LOG_DEBUG(fmt, ...) SILOK_LOG(Debug, fmt, __VA_ARGS__)
#define SILOK_LOG_INFO(fmt, ...) SILOK_LOG(Info, fmt, __VA_ARGS__)
#define SILOK_LOG_WARN(fmt, ...) SILOK_LOG(Warn, fmt, __VA_ARGS__)
#define SILOK_LOG_ERROR(fmt, ...) SILOK_LOG(Error, fmt, __VA_ARGS__)
#define SILOK_LOG_CRITICAL(fmt, ...) SILOK_LOG(Critical, fmt, __VA_ARGS__)
