#include "PageWriter.hpp"
#include <fmt/format.h>
#include <cstdlib>

PageWriter::PageWriter(const std::string_view out_file_path, const int page_size)
    : page_size_(page_size), out_tape_file_(out_file_path.data(), std::ios::out | std::ios::binary)
{
    if (!out_tape_file_.is_open())
    {
        fmt::print("ERROR: Failed to open output tape file at path [{}].\n", out_file_path);
        std::exit(-1);
    }
}

int PageWriter::GetHardDriveAccessesNumber() const
{
    return hard_drive_accesses_;
}

void PageWriter::WritePage(const byte_vector &page)
{
    if (page.size() > page_size_)
    {
        throw std::runtime_error(
            fmt::format("ERROR: Byte vector exceeds drive page size. Expected size = {}, actual size = {}\n",
                        page_size_, page.size()));
    }
    out_tape_file_.write(reinterpret_cast<const char *>(page.data()), page.size());
    ++hard_drive_accesses_;
}
