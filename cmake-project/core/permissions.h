#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <exception>
#include "bpfstypes.h"

class InvalidPermissionsException : public std::exception {};

enum FileType {
    REGULAR,
    DIRECTORY,
    NONE
};

struct Rwx {
    bool r;
    bool w;
    bool x;
    Rwx();
    Rwx(bool r, bool w, bool x);
    Rwx(unsigned rwx);
    unsigned toInt();
};

class Permissions
{
private:
    FileType fileType;
    Rwx user;
    Rwx group;
    Rwx others;
public:
    Permissions(RawPermissions rawData);
    Permissions(FileType type, unsigned u, unsigned g, unsigned o);
    Permissions(FileType type, Rwx u, Rwx g, Rwx o);
    FileType getFileType();

    RawPermissions getRawData();
    static unsigned getRawDataSize();
};


#endif // PERMISSIONS_H
