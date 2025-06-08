#pragma once

#include <memory>
#include <string>

namespace silok::db
{
class BaseDBStatement;

class BaseDBConnection
{
 public:
    virtual ~BaseDBConnection() = default;
    virtual void exec(const std::string& sql) = 0;
    virtual std::shared_ptr<BaseDBStatement> prepare(const std::string& sql) = 0;
    virtual int64_t lastInsertId() = 0;
};

using BaseDBConnectionPtr = std::shared_ptr<BaseDBConnection>;
}  // namespace silok::db
