#pragma once

#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>

#include <sqlite_orm/sqlite_orm.h>

#include "silok/domain/note.hpp"
#include "silok/domain/user.hpp"

namespace silok::infra
{

inline auto makeStorage(const std::string& path)
{
    using namespace sqlite_orm;
    using namespace ::silok::domain;

    return make_storage(
        path,
        make_table("user", make_column("id", &User::id, primary_key()),
                   make_column("name", &User::name), make_column("email", &User::email),
                   make_column("password", &User::password)),
        make_table("note", make_column("id", &User::id, primary_key()),
                   make_column("content", &Note::content),
                   make_column("created_at", &Note::created_at),
                   make_column("updated_at", &Note::updated_at)));
}

using Storage = decltype(makeStorage(""));

class StorageManager
{
 public:
    static void Initialize(const std::string& db_path, bool is_test = false)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (instance_ && !is_test)
        {
            throw std::runtime_error("StorageManager already initialized");
        }
        instance_.reset(new StorageManager(db_path));
    }

    static Storage& get()
    {
        if (!instance_)
        {
            throw std::runtime_error("StorageManager is not initialized");
        }
        return *instance_->storage_;
    }

    static void reset()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        instance_.reset();
    }

 private:
    explicit StorageManager(const std::string& db_path)
    {
        storage_ = std::make_unique<Storage>(makeStorage(db_path));
        storage_->sync_schema();  // 필수!
    }

    std::unique_ptr<Storage> storage_;
    static inline std::unique_ptr<StorageManager> instance_;
    static inline std::mutex mutex_;
};
}  // namespace silok::infra
