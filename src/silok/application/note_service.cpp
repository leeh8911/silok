#include "silok/application/note_service.hpp"

#include "silok/domain/model.hpp"
#include "silok/domain/model_relation.hpp"
#include "silok/domain/user_token.hpp"
#include "silok/infra/storage_manager.hpp"

namespace silok::application
{
void NoteService::Create(const std::string& content, std::string user_token)
{
    auto user_id = domain::DecodeUserToken(user_token);
    if (!user_id.has_value())
    {
        throw std::runtime_error("Invalid user token");
    }

    domain::Note note;
    note.content = content;
    note.created_at = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    note.updated_at = note.created_at;  // 초기 생성 시점과 업데이트 시점은 동일

    auto note_id = infra::StorageManager::Insert(note);

    domain::UserNote note_user;
    note_user.note_id = note_id;
    note_user.user_id = user_id.value();

    infra::StorageManager::Insert(note_user);
}

std::vector<domain::Note> NoteService::FindAll(std::string user_token)
{
    auto user_id = domain::DecodeUserToken(user_token);
    if (!user_id.has_value())
    {
        throw std::runtime_error("Invalid user token");
    }

    auto user_note =
        infra::StorageManager::FindByField(&domain::UserNote::user_id, user_id.value());
    if (user_note.empty())
    {
        return {};  // 해당 사용자의 노트가 없으면 빈 벡터 반환
    }
    auto lambda = [](const domain::UserNote& note_user) { return note_user.note_id; };
    std::vector<int64_t> note_ids{};

    std::transform(user_note.begin(), user_note.end(), std::back_inserter(note_ids), lambda);

    std::vector<domain::Note> notes;
    for (const auto& note_id : note_ids)
    {
        auto notes_for_user = infra::StorageManager::FindByField(&domain::Note::id, note_id);
        notes.insert(notes.end(), notes_for_user.begin(), notes_for_user.end());
    }

    return notes;
}

void NoteService::Update(const domain::Note& note)
{
    if (note.id <= 0)
    {
        throw std::runtime_error("Invalid note ID for update");
    }

    infra::StorageManager::Update(note);
}

void NoteService::Delete(int64_t note_id, std::string user_token)
{
    auto user_id = domain::DecodeUserToken(user_token);
    if (!user_id.has_value())
    {
        throw std::runtime_error("Invalid user token");
    }

    auto notes = infra::StorageManager::FindByField(&domain::Note::id, note_id);
    if (notes.empty())
    {
        throw std::runtime_error("Note not found or does not belong to the user");
    }

    auto note_user =
        infra::StorageManager::FindByField<domain::UserNote>(&domain::UserNote::note_id, note_id);

    infra::StorageManager::Remove(notes.front());
    infra::StorageManager::Remove(note_user.front());
}

}  // namespace silok::application
