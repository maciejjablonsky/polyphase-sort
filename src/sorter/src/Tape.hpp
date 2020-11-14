#ifndef TAPE_HPP
#define TAPE_HPP

#include <cstdint>
#include <string>
#include <type_traits>

struct Tape
{
    uint64_t dummy_series = 0;
    uint64_t series = 0;
    std::string file_path;
};

static_assert(std::is_default_constructible<Tape>());
static_assert(std::is_aggregate<Tape>());
static_assert(std::is_copy_constructible<Tape>());
static_assert(std::is_copy_assignable<Tape>());
static_assert(std::is_move_assignable<Tape>());
static_assert(std::is_move_constructible<Tape>());

#endif // TAPE_HPP
