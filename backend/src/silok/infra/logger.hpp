#pragma once

#include <string>

#include "silok/domain/base_logger.hpp"
#include "silok/infra/logger/spdlog_logger.hpp"

namespace silok::infra
{
inline void SetUpLogger(const std::string& name = "silok", const std::string& level = "trace",
                        const std::string& flush_on = "warn")
{
    auto logger = std::make_shared<SpdlogLogger>(name, level, flush_on);
    silok::domain::Logger::SetLogger(logger);
}
}  // namespace silok::infra
