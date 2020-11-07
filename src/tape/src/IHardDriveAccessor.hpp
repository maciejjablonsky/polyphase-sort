#ifndef IHARD_DRIVE_ACCESSOR_HPP
#define IHARD_DRIVE_ACCESSOR_HPP

class IHardDriveAccessor
{
  public:
    virtual int GetHardDriveAccessesNumber() const = 0;
    virtual ~IHardDriveAccessor() = default;
};

#endif // IHARD_DRIVE_ACCESSOR_HPP