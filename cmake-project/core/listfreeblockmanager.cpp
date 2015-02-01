#include "listfreeblockmanager.h"
#include "indirectblock.h"
#include <cmath>
#include <cassert>
#include "log.h"
#include "rawdatautils.h"

#include <chrono>
#include <thread>


ListFreeBlockManager::ListFreeBlockManager(FSPartition *partition, bool creatingFs)
    : controlBlock(partition)
{
    this->partition = partition;
    if (!creatingFs)
        reloadCurrentIndirectBlock();
}

ListFreeBlockManager::ListFreeBlockManager(FSPartition *partition)
    : ListFreeBlockManager(partition, false)
{

}

void ListFreeBlockManager::reloadCurrentIndirectBlock()
{
    if (currentIndirectBlock != nullptr)
        delete currentIndirectBlock;
    currentIndirectBlock = new IndirectBlock(partition, controlBlock.currentIndirectBlockId);
}

ListFreeBlockManager::~ListFreeBlockManager()
{
    if (currentIndirectBlock != nullptr)
        delete currentIndirectBlock;
    controlBlock.flush(partition);
}

BlockId ListFreeBlockManager::getFreeBlock()
{
    assert(!isFull());
    controlBlock.freeBlockCount--;
    if (controlBlock.nextFreeBlockPosition == currentIndirectBlock->getMaxSize()) {
        auto result = controlBlock.currentIndirectBlockId;
        controlBlock.nextFreeBlockPosition = 0;
        controlBlock.currentIndirectBlockId = currentIndirectBlock->getNextIndirectBlockId();
        reloadCurrentIndirectBlock();
        return result;
    }
    auto result = currentIndirectBlock->getBlockId(controlBlock.nextFreeBlockPosition++);
    Log::stream << "Free block requested, recieved: " << result << std::endl;
    return result;
}

BlockCount ListFreeBlockManager::freeBlockCount()
{
    return controlBlock.freeBlockCount;
}

bool ListFreeBlockManager::isFull()
{
    return controlBlock.freeBlockCount == 0;
}

void ListFreeBlockManager::flush()
{
    controlBlock.flush(partition);
}

void ListFreeBlockManager::initializeControlBlock(BlockId firstAvailableBlock)
{
    controlBlock.freeBlockCount = calculateAvailableBlocks(firstAvailableBlock);
    controlBlock.currentIndirectBlockId = firstAvailableBlock;
}

void ListFreeBlockManager::onPartitionCreation()
{
    BlockId firstAvailableBlock = partition->getFirstDataBlock() + 1;
    initializeControlBlock(firstAvailableBlock);
    createList(firstAvailableBlock);
    flush();
}

BlockCount ListFreeBlockManager::calculateListBlockCount()
{
    double result = controlBlock.freeBlockCount / (double)(IndirectBlock::getMaxSize(partition) + 1);
    return ceil(result);
}

BlockCount ListFreeBlockManager::calculateAvailableBlocks(BlockId firstAvailableBlock)
{
    BlockCount allBlocks = partition->getHeader()->getBlockCount();
    return allBlocks - firstAvailableBlock;
}

void ListFreeBlockManager::markBlockAsFree(BlockId id)
{
    controlBlock.freeBlockCount++;
    Log::stream << "Block marked as free: " << id << std::endl;
    if (controlBlock.nextFreeBlockPosition == 0)
        initializeNewListBlock(id);
    else
        addToCurrentListBlock(id);
}

void ListFreeBlockManager::initializeNewListBlock(BlockId id)
{
    auto nextIndirectBlockId = controlBlock.currentIndirectBlockId;
    controlBlock.nextFreeBlockPosition = IndirectBlock::getMaxSize(partition);
    controlBlock.currentIndirectBlockId = id;
    reloadCurrentIndirectBlock();
    currentIndirectBlock->setNextIndirectBlockId(nextIndirectBlockId);
}

void ListFreeBlockManager::addToCurrentListBlock(BlockId id)
{
    controlBlock.nextFreeBlockPosition--;
    currentIndirectBlock->setBlockId(controlBlock.nextFreeBlockPosition, id);
}

void ListFreeBlockManager::createList(BlockId inclusiveFrom)
{
    auto listBlockCount = calculateListBlockCount();
    BlockCount blocksToStore = controlBlock.freeBlockCount - listBlockCount;
    BlockId firstIndirectBlock = inclusiveFrom;
    inclusiveFrom += listBlockCount;

    calculateInitialFreeBlockPosition(blocksToStore);
    Log::stream << "Blocks to store in ListFreeBlockManager list: " << blocksToStore << std::endl;
    for (unsigned i = 0; i < listBlockCount; i++) {
        IndirectBlock indirectBlock(partition, firstIndirectBlock + i);
        unsigned initialPosition = blocksToStore % indirectBlock.getMaxSize();
        initialPosition = initialPosition > 0 ? indirectBlock.getMaxSize() - initialPosition: 0;
        for (unsigned position = initialPosition; position < indirectBlock.getMaxSize(); position++)
            indirectBlock.setBlockId(position, inclusiveFrom++);

        indirectBlock.setNextIndirectBlockId(firstIndirectBlock + i + 1);
        blocksToStore -= indirectBlock.getMaxSize() - initialPosition;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    assert(inclusiveFrom == partition->getHeader()->getBlockCount());
    assert(blocksToStore == 0);
}

void ListFreeBlockManager::calculateInitialFreeBlockPosition(BlockCount blocksToStore) {
    unsigned initialPosition = blocksToStore % IndirectBlock::getMaxSize(partition);
    initialPosition = initialPosition > 0 ? IndirectBlock::getMaxSize(partition) - initialPosition: 0;
    controlBlock.nextFreeBlockPosition = initialPosition;
}

ListFreeBlockManager::ControlBlock::ControlBlock(FSPartition *partition)
{
    unsigned offset = 0;
    char buffer[partition->getBlockSize()];

    this->blockId = partition->getFirstDataBlock();

    partition->readDataBlock(blockId, buffer);

    freeBlockCount = RawDataUtils::readUintFromBuffer(buffer, offset, sizeof(freeBlockCount));
    offset += sizeof(freeBlockCount);
    nextFreeBlockPosition = RawDataUtils::readUintFromBuffer(buffer, offset, sizeof(nextFreeBlockPosition));
    offset += sizeof(nextFreeBlockPosition);
    currentIndirectBlockId = RawDataUtils::readUintFromBuffer(buffer, offset, sizeof(currentIndirectBlockId));
}

void ListFreeBlockManager::ControlBlock::flush(FSPartition *partition)
{
    char buffer[partition->getBlockSize()];
    unsigned offset = 0;
    RawDataUtils::writeUintToBuffer(freeBlockCount, buffer, offset, sizeof(freeBlockCount));
    offset += sizeof(freeBlockCount);
    RawDataUtils::writeUintToBuffer(nextFreeBlockPosition, buffer, offset, sizeof(nextFreeBlockPosition));
    offset += sizeof(nextFreeBlockPosition);
    RawDataUtils::writeUintToBuffer(currentIndirectBlockId, buffer, offset, sizeof(currentIndirectBlockId));

    partition->writeDataBlock(blockId, buffer);
}
