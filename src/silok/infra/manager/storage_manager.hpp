#pragma once

#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>

#include <sqlite_orm/sqlite_orm.h>

#include "silok/logger.hpp"
#include "silok/model.hpp"

namespace silok::infra
{

inline auto makeStorage(const std::string& path)
{
    using namespace sqlite_orm;

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
        make_table("project", make_column("id", &Project::id, primary_key()),
                   make_column("name", &Project::name), make_column("start", &Project::start),
                   make_column("end", &Project::end)),
        make_table("user_note", make_column("id", &UserNote::id, primary_key()),
                   make_column("user_id", &UserNote::user_id),
                   make_column("note_id", &UserNote::note_id),
                   make_column("role", &UserNote::role)),
        make_table("user_tag", make_column("id", &UserTag::id, primary_key()),
                   make_column("user_id", &UserTag::user_id),
                   make_column("tag_id", &UserTag::tag_id), make_column("role", &UserTag::role)),
        make_table("user_project", make_column("id", &UserProject::id, primary_key()),
                   make_column("user_id", &UserProject::user_id),
                   make_column("project_id", &UserProject::project_id),
                   make_column("role", &UserProject::role)),
        make_table("note_tag", make_column("id", &NoteTag::id, primary_key()),
                   make_column("note_id", &NoteTag::note_id),
                   make_column("tag_id", &NoteTag::tag_id)),
        make_table("note_project", make_column("id", &NoteProject::id, primary_key()),
                   make_column("note_id", &NoteProject::note_id),
                   make_column("project_id", &NoteProject::project_id)));
}

using Storage = decltype(makeStorage(""));

class StorageManager
{
 public:
    /**
     * @brief StorageManager를 초기화합니다.
     *
     * @param db_path 사용할 데이터베이스 파일 경로
     * @param is_test 테스트용 초기화 여부 (중복 초기화 허용 여부 결정)
     */
    static void Initialize(const std::string& db_path, bool is_test = false)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        SILOK_LOG_DEBUG("Initializing StorageManager with path: {}", db_path);
        if (instance_ && !is_test)
        {
            throw std::runtime_error("StorageManager already initialized");
        }
        instance_.reset(new StorageManager(db_path));
    }

    /**
     * @brief 데이터를 삽입하고 해당 객체의 id 필드를 갱신한 후 반환합니다.
     *
     * @tparam T 삽입할 도메인 타입
     * @param data 삽입할 객체 (id 필드 포함)
     * @return int64_t 새로 생성된 레코드의 ID
     */
    template <typename T>
    static int64_t Insert(T& data)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        CheckInitialized();
        auto id = instance_->storage_->insert(data);
        data.id = id;
        return id;
    }

    /**
     * @brief 전체 데이터를 조회합니다.
     *
     * @tparam T 조회할 도메인 타입
     * @return std::vector<T> 전체 레코드 목록
     */
    template <typename T>
    static std::vector<T> SelectAll()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        CheckInitialized();
        return instance_->storage_->get_all<T>();
    }

    /**
     * @brief 특정 데이터를 업데이트합니다.
     *
     * @tparam T 업데이트할 도메인 타입
     * @param data 수정된 객체 (id 기준으로 갱신)
     */
    template <typename T>
    static void Update(const T& data)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        CheckInitialized();
        instance_->storage_->update(data);
    }

    /**
     * @brief 특정 데이터를 삭제합니다. 객체에 id 필드가 반드시 있어야 합니다.
     *
     * @tparam T 삭제할 도메인 타입
     * @param data 삭제할 객체 (id 필요)
     */
    template <typename T>
    static void Remove(const T& data)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        CheckInitialized();
        static_assert(requires { data.id; }, "Type T must have an id field for Remove()");
        SILOK_LOG_WARN("Removing record with ID: {}", data.id);
        instance_->storage_->remove<T>(data.id);
    }

    /**
     * @brief 단일 필드가 특정 값을 가지는 데이터를 조회합니다.
     *
     * @tparam T 조회할 도메인 타입
     * @tparam FieldType 비교할 필드의 타입
     * @param field_ptr 필드 멤버 포인터
     * @param value 비교할 값
     * @return std::vector<T> 일치하는 레코드 목록
     */
    template <typename T, typename FieldType>
    static std::vector<T> FindByField(FieldType T::* field_ptr, const FieldType& value)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        CheckInitialized();
        return instance_->storage_->get_all<T>(
            sqlite_orm::where(sqlite_orm::c(field_ptr) == value));
    }

    /**
     * @brief 여러 필드 조건으로 데이터를 조회합니다.
     *
     * @tparam T 조회할 도메인 타입
     * @tparam Args 필드와 값 쌍으로 구성된 인자 목록
     * @param args (field_ptr, value, field_ptr, value, ...) 순서로 전달
     * @return std::vector<T> 조건에 맞는 레코드 목록
     */
    template <typename T, typename... Args>
    static std::vector<T> FindByFields(Args&&... args)
    {
        static_assert(sizeof...(args) % 2 == 0, "Arguments must be in (field, value) pairs.");
        std::lock_guard<std::mutex> lock(mutex_);
        CheckInitialized();
        auto condition = BuildWhereCondition<T>(std::forward<Args>(args)...);
        return instance_->storage_->get_all<T>(sqlite_orm::where(condition));
    }

    /**
     * @brief 단일 필드에 대해 여러 값 중 하나라도 일치하는 데이터를 조회합니다.
     *
     * @tparam T 조회할 도메인 타입
     * @tparam FieldType 필드 타입
     * @param field 멤버 필드 포인터
     * @param values 포함 조건으로 사용할 값 목록
     * @return std::vector<T> 조건에 맞는 레코드 목록
     */
    template <typename T, typename FieldType>
    static std::vector<T> FindByFieldIn(FieldType T::* field, const std::vector<FieldType>& values)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        CheckInitialized();
        return instance_->storage_->get_all<T>(sqlite_orm::where(sqlite_orm::in(field, values)));
    }

    /**
     * @brief 사용자가 직접 지정한 조건으로 데이터를 조회합니다.
     *
     * @tparam T 조회할 도메인 타입
     * @tparam Condition where절 조건 표현식
     * @param condition SQL 조건 표현식
     * @return std::vector<T> 조건에 맞는 레코드 목록
     */
    template <typename T, typename Condition>
    static std::vector<T> FindWhere(const Condition& condition)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        CheckInitialized();
        return instance_->storage_->get_all<T>(sqlite_orm::where(condition));
    }

    /**
     * @brief 내부 storage 객체에 직접 접근할 수 있습니다. 주의해서 사용하세요.
     *
     * @return Storage& 내부 storage 참조
     */
    static Storage& get()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        CheckInitialized();
        return *instance_->storage_;
    }

    /**
     * @brief StorageManager를 리셋합니다. 테스트 환경 등에서 사용합니다.
     */
    static void reset()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        instance_.reset();
    }

 private:
    explicit StorageManager(const std::string& db_path)
    {
        storage_ = std::make_unique<Storage>(makeStorage(db_path));
        storage_->sync_schema();
    }

    static void CheckInitialized()
    {
        if (!instance_)
        {
            throw std::runtime_error("StorageManager not initialized");
        }
    }
    template <typename T>
    static constexpr auto BuildWhereCondition()
    {
        return sqlite_orm::c(&T::id) >= 0;  // always true-like condition
    }

    template <typename T, typename FieldType, typename ValueType, typename... Rest>
    static auto BuildWhereCondition(FieldType T::* field, const ValueType& value, Rest&&... rest)
    {
        auto current = sqlite_orm::c(field) == value;
        if constexpr (sizeof...(rest) == 0)
        {
            return current;
        }
        else
        {
            return current and BuildWhereCondition<T>(std::forward<Rest>(rest)...);
        }
    }

    std::unique_ptr<Storage> storage_;
    static inline std::unique_ptr<StorageManager> instance_;
    static inline std::mutex mutex_;
};

}  // namespace silok::infra
