#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <cstdint>

class Permissions
{
public:
    Permissions(uint16_t rawData);
    Permissions(unsigned type, unsigned u, unsigned g, unsigned o);

    uint16_t getRawData();
    static unsigned getRawDataSize();
};

#endif // PERMISSIONS_H
