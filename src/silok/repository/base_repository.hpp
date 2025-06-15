#pragma once

#include <cstdint>

namespace silok::repository
{
class BaseRepository
{
 public:
    virtual ~BaseRepository() = default;

    // Add methods that all repositories should implement
    virtual void create() = 0;                  // Create a new record
    virtual void read(int64_t id) = 0;          // Read a record by ID
    virtual void update(int64_t id) = 0;        // Update a record by ID
    virtual void deleteRecord(int64_t id) = 0;  // Delete a record by ID
};

}  // namespace silok::repository
