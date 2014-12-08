#include "commandresolver.h"
#include "fscreator.h"

unsigned long long CommandResolver::promptForUnsignedInt(std::string message)
{
    unsigned long long result;
    cout << message << ": ";
    cin >> result;
    return result;
}

void CommandResolver::ok()
{
    cout << "DONE" << endl;
}

void CommandResolver::error(std::string message) {
    cerr << "Error: " << message << endl;
}

void CommandResolver::makeFilesystem(IDisk &disk)
{
    FSCreator creator;
    BlockSize blockSize = promptForUnsignedInt("Block size");
    InodeSize inodeSize = promptForUnsignedInt("Inode size");
    InodeCount inodeCount = promptForUnsignedInt("Number of inodes");
    creator.createFilesystem(disk, blockSize, inodeSize, inodeCount);
    ok();
}

FSPartition *CommandResolver::loadFilesystem(IDisk &disk)
{
    FSPartition *partition = nullptr;
    try {
        partition = new FSPartition(disk);
        ok();
    }
    catch (InvalidFilesystemIdenifierException e) {
        error("Invalid filesystem identifier. Type \"mkfs\" to create filesystem");
    }

    return partition;
}
