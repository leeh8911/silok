#include "silok/repository/user_repository.hpp"

#include "silok/db/base/base_db_connection.hpp"

namespace silok::repository
{

UserRepository::UserRepository(BaseDBConnectionPtr db_) : db(std::move(db_)) {}

void UserRepository::createUser(const std::string& username)
{
    auto stmt = db->prepare("INSERT INTO users(username) VALUES(?);");
    stmt->bind(1, username);
    if (!stmt->step())
    {
        throw std::runtime_error("Failed to create user: " + username);
    }
}

std::optional<User> UserRepository::getUserById(uint64_t id)
{
    auto stmt = db->prepare("SELECT id, username FROM users WHERE id = ?;");
    stmt->bind(1, id);
    if (stmt->step())
    {
        User user;
        user.id = stmt->columnInt(0);
        user.username = stmt->columnText(1);
        return user;
    }
    return std::nullopt;  // User not found
}

void UserRepository::deleteUser(uint64_t id)
{
    auto stmt = db->prepare("DELETE FROM users WHERE id = ?;");
    stmt->bind(1, id);
    if (!stmt->step())
    {
        throw std::runtime_error("Failed to delete user with ID: " + std::to_string(id));
    }
}

}  // namespace silok::repository