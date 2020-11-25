#include "PageWriter.hpp"
#include <fmt/format.h>
#include <cstdlib>
#include <logger/Logger.hpp>

using namespace std::string_view_literals;

PageWriter::PageWriter(const std::string_view out_file_path, const int page_size)
    : out_path_(out_file_path),
      page_size_(page_size),
      out_tape_file_(out_file_path.data(), std::ios::out | std::ios::binary)
{
    if (!out_tape_file_.is_open())
    {
        fmt::print("ERROR: Failed to open output tape file at path [{}].\n", out_file_path);
        std::exit(-1);
    }
}

int PageWriter::GetHardDriveAccessesNumber() const { return hard_drive_accesses_; }

void PageWriter::WritePage(Page&& page)
{
    try
    {
        if (page.size() > page_size_)
        {
            throw std::runtime_error(fmt::format(
                "ERROR: Byte vector exceeds drive page size. Expected size = {}, actual size = {}\n",
                page_size_, page.size()));
        }

        pages_.push_back(std::move(page));
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error(fmt::format("Failed to buffer a page. Error message: {}", e.what()));
    }
}

void PageWriter::WritePage(const Page& page)
{
    try
    {
        if (page.size() > page_size_)
        {
            throw std::runtime_error(fmt::format(
                "ERROR: Byte vector exceeds drive page size. Expected size = {}, actual size = {}\n",
                page_size_, page.size()));
        }
        out_tape_file_.write(reinterpret_cast<const char*>(page.data()), page.size());

#ifdef ENABLE_LOGGING
        Logger::Dump(fmt::format("Page written to file {}", out_path_));
#endif  // ENABLE_LOGGING
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error(
            fmt::format("Failed to write to a file [{}]. Error message: {}", out_path_, e.what()));
    }
    ++hard_drive_accesses_;
}

void PageWriter::Flush()
{
    for (const auto& page : pages_)
    {
        try
        {
            out_tape_file_.write(reinterpret_cast<const char*>(page.data()), page.size());

#ifdef ENABLE_LOGGING
            Logger::Dump(fmt::format("Page written to file {}", out_path_));
#endif  // ENABLE_LOGGING
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error(
                fmt::format("Failed to write to a file [{}]. Error message: {}", out_path_, e.what()));
        }

        ++hard_drive_accesses_;
    }
    pages_.clear();
}

PageWriter::~PageWriter()
{
    try
    {
        Flush();
    }
    catch (const std::exception& e)
    {
        fmt::print("Failed to write to a file [{}]. Error message: {}", out_path_, e.what());
        std::exit(-1);
    }
}
