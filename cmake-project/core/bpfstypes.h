#ifndef BPFSTYPES_H
#define BPFSTYPES_H

#include <cstdint>
#include <exception>

class NotImplementedYetException : std::exception {};

typedef uint64_t BlockId;
typedef uint64_t BlockCount;
typedef uint32_t BlockSize;
typedef uint16_t InodeSize;
typedef uint32_t InodeCount;
typedef uint32_t InodeId;
typedef uint16_t RawPermissions;

const BlockId INVALID_BLOCK_ID = 0;


#endif // BPFSTYPES_H
