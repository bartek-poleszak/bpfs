#include "directory.h"

void Directory::flush()
{
    char buffer[partition->getBlockSize()];
    for (auto &entry : entries) {
        //wypelnianie bufora
    }
}

Directory::Directory(File *file, FSPartition *partition)
{
    this->partition = partition;
    char buffer[partition->getBlockSize()];
    file->readBlock(0, buffer);
    //wypelnianie mapy
}

Inode *Directory::getInode(string fileName) {
    InodeId inodeId = entries[fileName];
    if (inodeId == Inode::INVALID_ID)
        throw FileDosentExistException();
    return partition->getInode(inodeId);
}

void Directory::link(string fileName, Inode *inode)
{
    entries[fileName] = inode->getId();
    flush();
}

void Directory::unlink(string fileName)
{
    Inode *inode = getInode(fileName);
    entries.erase(fileName);
    inode->removeLink();
    flush();
}

File Directory::getFile(string fileName)
{
    Inode *inode = getInode(fileName);
    return File(fileName, inode, partition);
}
