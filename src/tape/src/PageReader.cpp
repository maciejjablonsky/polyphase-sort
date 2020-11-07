#include "PageReader.hpp"
#include <fmt/format.h>

PageReader::PageReader(const std::string_view in_tape_file_path, const int page_size)
    : in_tape_file_(in_tape_file_path.data(), std::ios::binary), page_size_(page_size)
{
    if (!in_tape_file_.is_open())
    {
        fmt::print("ERROR: Failed to open input tape file at path [{}].\n", in_tape_file_path);
        std::exit(-1);
    }
}

byte_vector PageReader::ReadPage()
{
    byte_vector page(page_size_);
    in_tape_file_.read(reinterpret_cast<char *>(page.data()), page_size_);
    ++hard_drive_accesses_;
    return page;
}

int PageReader::GetHardDriveAccessesNumber() const
{
    return hard_drive_accesses_;
}
