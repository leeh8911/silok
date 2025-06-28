#include "noop_password_hasher.hpp"

#include <bcrypt.h>

#include "silok/logger.hpp"

namespace silok::infra
{
std::string CryptPasswordHasher::HashPassword(const std::string& password) const
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

bool CryptPasswordHasher::CheckPassword(const std::string& password, const std::string& hash) const
{
    return bcrypt_checkpw(password.c_str(), hash.c_str()) == 0;
}

std::string NoOpPasswordHasher::HashPassword(const std::string& password) const
{
    // No-op implementation, returns the password as is
    return password + "_no_op_hash";
}
bool NoOpPasswordHasher::CheckPassword(const std::string& password, const std::string& hash) const
{
    // No-op implementation, checks if the password matches the hash directly
    return (password + "_no_op_hash") == hash;
}
}  // namespace silok::infra
