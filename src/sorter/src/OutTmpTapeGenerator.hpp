#ifndef OUT_TMP_TAPE_GENERATOR_HPP
#define OUT_TMP_TAPE_GENERATOR_HPP

#include "Tape.hpp"
#include <chrono>
#include <fmt/format.h>

struct out_tmp_tape 
{
    Tape operator()()
    {
        using namespace std::chrono;
        auto timestamp = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
        return {.dummy_series = 0, .series = 0, .file_path = fmt::format("tmp_out_{}.tape", timestamp)};
    }
};

#endif // OUT_TMP_TAPE_GENERATOR_HPP