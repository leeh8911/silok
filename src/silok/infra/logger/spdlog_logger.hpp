#pragma once

#include <memory>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "silok/domain/base_logger.hpp"

namespace silok::infra
{

class SpdlogLogger : public silok::domain::BaseLogger
{
 public:
    SpdlogLogger(const std::string& name = "silok", const std::string& level = "trace",
                 const std::string& flush_on = "warn");

    void Trace(const std::string& msg) override;
    void Debug(const std::string& msg) override;
    void Info(const std::string& msg) override;
    void Warn(const std::string& msg) override;
    void Error(const std::string& msg) override;
    void Critical(const std::string& msg) override;

 private:
    std::shared_ptr<spdlog::logger> logger_;

    spdlog::level::level_enum stringToLevel(const std::string& level);
};

}  // namespace silok::infra
