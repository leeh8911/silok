#pragma once

#include <string>

#include <bcrypt.h>

#include "silok/logger.hpp"

namespace silok
{
#include <bcrypt.h>

std::string HashPassword(const std::string& password)
{
    char hash[61];
    char salt[30];

    if (bcrypt_gensalt(12, salt) != 0)
    {
        SILOK_LOG_ERROR("Salt generation failed");
        return "";
    }

    if (bcrypt_hashpw(password.c_str(), salt, hash) != 0)
    {
        SILOK_LOG_ERROR("Salt generation failed");
        return "";
    }

    return std::string(hash);
}

bool CheckPassword(const std::string& password, const std::string& hash)
{
    return bcrypt_checkpw(password.c_str(), hash.c_str()) == 0;
}

}  // namespace silok
