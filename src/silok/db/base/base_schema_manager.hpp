#pragma once

#include "silok/db/base/base_db_connection.hpp"

namespace silok::db
{
class BaseSchemaManager
{
 public:
    virtual ~BaseSchemaManager() = default;
    virtual void migrate(BaseDBConnectionPtr db) = 0;
};

}