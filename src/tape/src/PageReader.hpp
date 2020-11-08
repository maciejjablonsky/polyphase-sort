#ifndef PAGE_READER_HPP
#define PAGE_READER_HPP

#include <IHardDriveAccessor.hpp>
#include <string_view>
#include <fstream>
#include "Page.hpp"
class PageReader : public IHardDriveAccessor
{
  public:
    PageReader(const std::string_view in_tape_file_path, const int page_size);
    Page ReadPage();
    int GetHardDriveAccessesNumber() const override;

  private:
    std::ifstream in_tape_file_;
    const int page_size_ = 0;
    int hard_drive_accesses_ = 0;
};

#endif // PAGE_READER_HPP