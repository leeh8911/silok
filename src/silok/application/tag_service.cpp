#include "silok/application/tag_service.hpp"

#include "silok/domain/model.hpp"
#include "silok/domain/model_relation.hpp"
#include "silok/domain/user_token.hpp"
#include "silok/infra/storage_manager.hpp"

namespace silok::application
{
TagService& TagService::Get()
{
    static TagService instance;
    return instance;
}

void TagService::Create(const std::string& name, std::string user_token,
                        const std::optional<domain::Note>& note)
{
    auto user_id = domain::DecodeUserToken(user_token);
    if (!user_id.has_value())
    {
        throw std::runtime_error("Invalid user token");
    }

    domain::Tag tag;
    tag.name = name;

    auto tag_id = infra::StorageManager::Insert(tag);

    domain::UserTag tag_user;
    tag_user.tag_id = tag_id;
    tag_user.user_id = user_id.value();

    infra::StorageManager::Insert(tag_user);

    if (note.has_value())
    {
        domain::NoteTag note_tag;
        note_tag.note_id = note->id;
        note_tag.tag_id = tag_id;

        auto found = infra::StorageManager::FindByFields<domain::NoteTag>(
            &domain::NoteTag::note_id, note_tag.note_id, &domain::NoteTag::tag_id, tag_id);

        if (found.empty())
        {
            infra::StorageManager::Insert(note_tag);
        }
        else
        {
            // 이미 해당 노트와 태그의 관계가 존재하는 경우, 아무 작업도 하지 않음
            // 이 부분은 필요에 따라 로깅하거나 예외를 던질 수 있음
        }
    }
}

std::vector<domain::Tag> TagService::FindAll(std::string user_token)
{
    auto user_id = domain::DecodeUserToken(user_token);
    if (!user_id.has_value())
    {
        throw std::runtime_error("Invalid user token");
    }

    auto user_tag = infra::StorageManager::FindByField(&domain::UserTag::user_id, user_id.value());
    if (user_tag.empty())
    {
        return {};  // 해당 사용자의 태그가 없으면 빈 벡터 반환
    }

    auto lambda = [](const domain::UserTag& tag_user) { return tag_user.tag_id; };
    std::vector<int64_t> tag_ids{};

    std::transform(user_tag.begin(), user_tag.end(), std::back_inserter(tag_ids), lambda);

    return infra::StorageManager::FindByFieldIn(&domain::Tag::id, tag_ids);
}

void TagService::Update(const domain::Tag& tag, const std::optional<domain::Note>& note)
{
    if (tag.id <= 0)
    {
        throw std::runtime_error("Invalid tag ID for update");
    }

    infra::StorageManager::Update(tag);

    if (note.has_value())
    {
        domain::NoteTag note_tag;
        note_tag.note_id = note->id;
        note_tag.tag_id = tag.id;

        infra::StorageManager::Insert(note_tag);

        auto found = infra::StorageManager::FindByFields<domain::NoteTag>(
            &domain::NoteTag::note_id, note_tag.note_id, &domain::NoteTag::tag_id, tag.id);

        if (found.empty())
        {
            infra::StorageManager::Insert(note_tag);
        }
        else
        {
            // 이미 해당 노트와 태그의 관계가 존재하는 경우, 아무 작업도 하지 않음
            // 이 부분은 필요에 따라 로깅하거나 예외를 던질 수 있음
        }
    }
}

void TagService::Detach(const domain::Tag& tag, const domain::Note& note)
{
    if (tag.id <= 0 || note.id <= 0)
    {
        throw std::runtime_error("Invalid tag or note ID for detachment");
    }

    auto note_tag = infra::StorageManager::FindByFields<domain::NoteTag>(
        &domain::NoteTag::note_id, note.id, &domain::NoteTag::tag_id, tag.id);

    if (!note_tag.empty())
    {
        infra::StorageManager::Remove(note_tag.front());
    }
}

void TagService::Delete(int64_t tag_id, std::string user_token)
{
    auto user_id = domain::DecodeUserToken(user_token);
    if (!user_id.has_value())
    {
        throw std::runtime_error("Invalid user token");
    }

    auto tags = infra::StorageManager::FindByField(&domain::Tag::id, tag_id);
    if (tags.empty())
    {
        throw std::runtime_error("Tag not found for deletion");
    }

    infra::StorageManager::Remove<domain::Tag>(tags.front());

    // Remove the association with the user
    auto user_tag = infra::StorageManager::FindByField(&domain::UserTag::tag_id, tag_id);
    if (!user_tag.empty())
    {
        infra::StorageManager::Remove<domain::UserTag>(user_tag.front());
    }
    auto note_tags = infra::StorageManager::FindByField(&domain::NoteTag::tag_id, tag_id);
    for (const auto& note_tag : note_tags)
    {
        infra::StorageManager::Remove<domain::NoteTag>(note_tag);
    }
}

}  // namespace silok::application
