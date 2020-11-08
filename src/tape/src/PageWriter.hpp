#ifndef PAGE_WRITER_HPP
#define PAGE_WRITER_HPP

#include "IHardDriveAccessor.hpp"
#include <string_view>
#include <fstream>
#include <vector>
#include "byte.hpp"
#include "Page.hpp"

class PageWriter : public IHardDriveAccessor
{
  public:
    PageWriter(const std::string_view out_tape_file_path, const int page_size);
    int GetHardDriveAccessesNumber() const override;
    void WritePage(const Page  &page);    
  private:
    int hard_drive_accesses_ = 0;
    std::fstream out_tape_file_;
    const int page_size_ = 0;
};

#endif // PAGE_WRITER_HPP