#include "permissions.h"

Permissions::Permissions(uint16_t rawData)
{
}

RawPermissions Permissions::getRawData()
{
    return 0;
}

unsigned Permissions::getRawDataSize()
{
    return sizeof(RawPermissions);
}
