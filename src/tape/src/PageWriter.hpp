#ifndef PAGE_WRITER_HPP
#define PAGE_WRITER_HPP

#include "IHardDriveAccessor.hpp"
#include <string_view>
#include <fstream>
#include <vector>
#include "byte.hpp"
#include "Page.hpp"
#include <type_traits>

class PageWriter : public IHardDriveAccessor
{
  public:
    PageWriter(const std::string_view out_tape_file_path, const int page_size);
    PageWriter(PageWriter&& writer) = default;
    PageWriter& operator=(PageWriter&& writer) = default;
    int GetHardDriveAccessesNumber() const override;
    void WritePage(Page&& page);
    void WritePage(const Page& page);
    void Flush();
    ~PageWriter();

  private:
    std::string_view out_path_;
    int hard_drive_accesses_ = 0;
    std::fstream out_tape_file_;
    int page_size_ = 0;
    std::vector<Page> pages_;
};

static_assert(std::is_move_assignable<PageWriter>());
static_assert(std::is_move_constructible<PageWriter>());

static_assert(std::is_move_assignable<std::fstream>());
static_assert(std::is_move_constructible<std::fstream>());

static_assert(std::is_move_assignable<std::vector<Page>>());
static_assert(std::is_move_constructible<std::vector<Page>>());

#endif // PAGE_WRITER_HPP