#include "file.h"

File::File(std::string name, Inode *inode, FSPartition *fsPartition)
{
    this->name = name;
    this->inode = inode;
    this->fsPartition = fsPartition;
}

File::~File()
{

}

void File::read(char *buffer, uint64_t offset, uint64_t length)
{
    if (offset != 0 && length != 1024)
        throw NotImplementedYetException();
    readBlock(0, buffer);
}

void File::readBlock(BlockCount index, char *buffer)
{
    if (index >= inode->getSizeInBlocks())
        throw BlockIndexOutOfRangeException();
    fsPartition->readDataBlock(inode->getDataBlockNumber(index), buffer);
}

void File::writeBlock(BlockCount index, char *buffer)
{
    if (index == inode->getSizeInBlocks())
        appendByBlock(buffer, fsPartition->getBlockSize());
    else
        rewriteBlock(index, buffer);
}

void File::rewriteBlock(BlockCount index, char *buffer)
{
    if (index > inode->getSizeInBlocks())
        throw BlockIndexOutOfRangeException();
    BlockId blockId = inode->getDataBlockNumber(index);
    fsPartition->writeDataBlock(blockId, buffer);
}

void File::appendByBlock(char *buffer, BlockSize significantBytes)
{
    if (significantBytes > fsPartition->getBlockSize())
        throw TooManyDataForBlockException();
    if (significantBytes == 0)
        throw NoneDataForBlockException();

    BlockId newBlockId = fsPartition->getFreeBlock();
    inode->addDataBlock(newBlockId);
    fsPartition->writeDataBlock(newBlockId, buffer);
    inode->setLastBlockByteCount(significantBytes);
}

void File::append(char *buffer, uint64_t significantBytes)
{
    BlockSize bytesWritten = appendLastBlock(buffer, significantBytes);
    significantBytes -= bytesWritten;
    buffer += bytesWritten;
    while (significantBytes > fsPartition->getBlockSize()) {
        appendByBlock(buffer, fsPartition->getBlockSize());
        significantBytes -= bytesWritten;
        buffer += bytesWritten;
    }
    if (significantBytes > 0)
        appendByBlock(buffer, significantBytes);
}

///
/// \brief File::appendLastBlock
/// \param buffer
/// \param significantBytes
/// \return bytes written
///
BlockSize File::appendLastBlock(char *buffer, uint64_t significantBytes) {
    if (inode->getLastBlockByteCount() >= fsPartition->getBlockSize())
        return 0;

    char lastBlock[fsPartition->getBlockSize()];
    readBlock(inode->getSizeInBlocks() - 1, lastBlock);

    BlockSize freeSpace = fsPartition->getBlockSize() - inode->getLastBlockByteCount();
    BlockSize bytesToWrite = freeSpace < significantBytes ? freeSpace : significantBytes;

    for (unsigned i = 0; i < bytesToWrite; ++i) {
        lastBlock[inode->getLastBlockByteCount() + i] = buffer[i];
    }

    inode->setLastBlockByteCount(inode->getLastBlockByteCount() + bytesToWrite);
    rewriteBlock(inode->getSizeInBlocks() - 1, lastBlock);
    return bytesToWrite;
}

void File::write(std::ifstream &fileStream)
{
    char buffer[fsPartition->getBlockSize()];

    fileStream.read(buffer, fsPartition->getBlockSize());
    while (fileStream.gcount() == fsPartition->getBlockSize()) {
        appendByBlock(buffer, fsPartition->getBlockSize());
        fileStream.read(buffer, fsPartition->getBlockSize());
    }
    appendByBlock(buffer, fileStream.gcount());
}

void File::get(std::ofstream &fileStream)
{
    char buffer[fsPartition->getBlockSize()];

    for (BlockCount i = 0; i < inode->getSizeInBlocks() - 1; ++i) {
        readBlock(i, buffer);
        fileStream.write(buffer, fsPartition->getBlockSize());
    }
    readBlock(inode->getSizeInBlocks() - 1, buffer);
    fileStream.write(buffer, inode->getLastBlockByteCount());

}


bool File::isDirectory()
{
    if (inode->getPermissions()->getFileType() == FileType::DIRECTORY)
        return true;
    return false;
}
