#include "permissions.h"

Permissions::Permissions(RawPermissions rawData)
{
    fileType = static_cast<FileType>(rawData);
}

Permissions::Permissions(FileType type, unsigned u, unsigned g, unsigned o)
    : Permissions(type, Rwx(u), Rwx(g), Rwx(o)) {}

Permissions::Permissions(FileType type, Rwx u, Rwx g, Rwx o)
{
    this->fileType = type;
}

FileType Permissions::getFileType()
{
    return fileType;
}

RawPermissions Permissions::getRawData()
{
    return static_cast<RawPermissions>(fileType);
}

unsigned Permissions::getRawDataSize()
{
    return sizeof(RawPermissions);
}


Rwx::Rwx() : Rwx(7) {}

Rwx::Rwx(bool r, bool w, bool x)
{
    this->r = r;
    this->w = w;
    this->x = x;
}

Rwx::Rwx(unsigned rwx)
{
    if (rwx > 7)
        throw InvalidPermissionsException();

    x = rwx % 2;
    rwx /= 2;
    w = rwx % 2;
    rwx /= 2;
    r = rwx;
}

unsigned Rwx::toInt()
{
    return 7;
}
