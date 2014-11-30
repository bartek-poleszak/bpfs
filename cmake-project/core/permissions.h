#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include "bpfstypes.h"

class Permissions
{
public:
    Permissions(RawPermissions rawData);
    Permissions(unsigned type, unsigned u, unsigned g, unsigned o);

    RawPermissions getRawData();
    static unsigned getRawDataSize();
};


#endif // PERMISSIONS_H
