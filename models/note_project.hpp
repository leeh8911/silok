#pragma once

#include <string>

#pragma db value
struct NoteProjectId
{
    unsigned long note_id;
    unsigned long project_id;

    NoteProjectId() = default;
    NoteProjectId(unsigned long nid, unsigned long pid) : note_id(nid), project_id(pid) {}

    bool operator==(const NoteProjectId& other) const = default;
};

#pragma db object
class NoteProject
{
 public:
#pragma db id
    NoteProjectId id;
    NoteProject() = default;
    NoteProject(unsigned long nid, unsigned long pid) : id{nid, pid} {}
};
