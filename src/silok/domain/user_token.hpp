#include <jwt-cpp/jwt.h>

namespace silok::domain
{

inline std::string getSecret()
{
    const char* env = std::getenv("JWT_SECRET");
    return env ? std::string(env) : "";
}

inline std::optional<std::string> EncodeUserToken(int64_t user_id)
{
    try
    {
        auto token = jwt::create()
                         .set_issuer("silok")
                         .set_type("JWT")
                         .set_payload_claim("user_id", jwt::claim(std::to_string(user_id)))
                         .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(1))
                         .sign(jwt::algorithm::hs256{getSecret()});

        return token;
    }
    catch (...)
    {
        return std::nullopt;
    }
}

inline std::optional<int64_t> DecodeUserToken(const std::string& token_str)
{
    try
    {
        auto decoded = jwt::decode(token_str);
        auto verifier =
            jwt::verify().allow_algorithm(jwt::algorithm::hs256{getSecret()}).with_issuer("silok");

        verifier.verify(decoded);

        auto claim = decoded.get_payload_claim("user_id").as_string();
        return std::stoll(claim);
    }
    catch (...)
    {
        return std::nullopt;
    }
}

}  // namespace silok::domain
