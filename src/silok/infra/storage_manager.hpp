#pragma once

#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>

#include <sqlite_orm/sqlite_orm.h>

#include "silok/domain/data.hpp"
#include "silok/domain/data_relation.hpp"

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
        make_table("note", make_column("id", &Note::id, primary_key()),
                   make_column("content", &Note::content),
                   make_column("created_at", &Note::created_at),
                   make_column("updated_at", &Note::updated_at)),
        make_table("tag", make_column("id", &Tag::id, primary_key()),
                   make_column("name", &Tag::name)),
        make_table("user_note", make_column("id", &UserNote::id, primary_key()),
                   make_column("user_id", &UserNote::user_id),
                   make_column("note_id", &UserNote::note_id)),
        make_table("user_tag", make_column("id", &UserTag::id, primary_key()),
                   make_column("user_id", &UserTag::user_id),
                   make_column("tag_id", &UserTag::tag_id)),
        make_table("note_tag", make_column("id", &NoteTag::id, primary_key()),
                   make_column("note_id", &NoteTag::note_id),
                   make_column("tag_id", &NoteTag::tag_id)));
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

    template <typename T>
    static void Insert(const T& data)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!instance_)
        {
            throw std::runtime_error("StorageManager not initialized");
        }
        instance_->storage_->insert(data);
    }
    template <typename T>
    static T Last()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!instance_)
        {
            throw std::runtime_error("StorageManager not initialized");
        }
        auto id = instance_->storage_->last_insert_rowid();
        auto obj = instance_->storage_->get<T>(id);

        return obj;
    }

    template <typename T>
    static std::vector<T> SelectAll()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!instance_)
        {
            throw std::runtime_error("StorageManager not initialized");
        }
        return instance_->storage_->get_all<T>();
    }

    template <typename T>
    static void Update(const T& data)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!instance_)
        {
            throw std::runtime_error("StorageManager not initialized");
        }
        return instance_->storage_->update(data);
    }

    template <typename T>
    static void Remove(const T& data)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!instance_)
        {
            throw std::runtime_error("StorageManager not initialized");
        }
        return instance_->storage_->remove<T>(data.id);
    }

    template <typename T, typename FieldType>
    static std::vector<T> FindByField(FieldType T::* field_ptr, const FieldType& value)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!instance_)
        {
            throw std::runtime_error("StorageManager not initialized");
        }
        return instance_->storage_->get_all<T>(
            sqlite_orm::where(sqlite_orm::c(field_ptr) == value));
    }

    static Storage& get()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!instance_)
        {
            throw std::runtime_error("StorageManager not initialized");
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
