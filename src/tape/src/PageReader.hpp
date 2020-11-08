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
    bool WasLastPageRead() const;
    int GetHardDriveAccessesNumber() const override;
    int PageSize() const;

  private:
    mutable std::ifstream in_tape_file_;
    const std::ifstream::pos_type tape_file_last_byte_pos_; 
    const int page_size_ = 0;
    int hard_drive_accesses_ = 0;
};

#endif // PAGE_READER_HPP