#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace silok::db
{
class BaseDBStatement
{
 public:
    virtual ~BaseDBStatement() = default;
    virtual void bind(int idx, int64_t v) = 0;
    virtual void bind(int idx, const std::string& v) = 0;
    virtual void bindNull(int idx) = 0;
    virtual bool step() = 0;
    virtual int64_t columnInt(int col) const = 0;
    virtual std::string columnText(int col) const = 0;
};

using BaseDBStatementPtr = std::shared_ptr<BaseDBStatement>;
}  // namespace silok::db
