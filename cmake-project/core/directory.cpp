#include "directory.h"
#include "rawdatautils.h"
#include "rootdirectory.h"

//void Directory::flush()
//{
//    char buffer[partition->getBlockSize()];
//    for (auto &entry : entries) {
//        //wypelnianie bufora
//    }
//}

void Directory::readEntryFromBuffer(char *buffer, uint8_t stringLength)
{
    InodeId inodeId = RawDataUtils::readUintFromBuffer(buffer, INODE_ID_OFFSET, sizeof(inodeId));
    string fileName;
    fileName.reserve(stringLength);
    for (int i = 0; i < stringLength; ++i) {
        fileName.push_back(buffer[FILE_NAME_OFFSET + i]);
    }
    entries[fileName] = inodeId;
}

Directory::Directory(File *file, FSPartition *partition)
{
    this->partition = partition;
    this->file = file;
    char buffer[partition->getBlockSize()];
    uint8_t entryLength;
    while (file->read(buffer, sizeof(entryLength)) > 0) {
        entryLength = RawDataUtils::readUintFromBuffer(buffer, ENTRY_LENGTH_OFFSET, sizeof(entryLength));
        entryLength -= sizeof(entryLength);
        if (entryLength != file->read(buffer + sizeof(entryLength), entryLength))
            throw ReadingDirectoryException();
        readEntryFromBuffer(buffer, entryLength - sizeof(InodeId));
    }
}

Directory::~Directory()
{

}

Inode *Directory::getInode(const string &fileName) {
    InodeId inodeId = entries[fileName];
    if (inodeId == Inode::INVALID_ID)
        throw FileDosentExistException();
    return partition->getInode(inodeId);
}

void Directory::link(string &fileName, Inode *inode)
{
    if (fileName.size() > MAX_FILE_NAME_LENGTH)
        throw FileNameTooLongException();
    char buffer[MAX_FILE_NAME_LENGTH];
    entries[fileName] = inode->getId();
    inode->addLink();
    unsigned entrySize = createRawEntry(buffer, fileName);
    file->append(buffer, entrySize);
}

void Directory::link(File &file)
{
    link(file.getName(), file.getInode());
}

unsigned Directory::createRawEntry(char *buffer, string &fileName)
{
    InodeId inodeId = entries[fileName];
    uint8_t entryLength = sizeof(entryLength) + sizeof(inodeId) + fileName.size();
    RawDataUtils::writeUintToBuffer(entryLength, buffer, ENTRY_LENGTH_OFFSET, sizeof(entryLength));
    RawDataUtils::writeUintToBuffer(inodeId, buffer, INODE_ID_OFFSET, sizeof(inodeId));
    for (unsigned i = 0; i < fileName.size(); ++i) {
        buffer[FILE_NAME_OFFSET + i] = fileName[i];
//        RawDataUtils::writeUintToBuffer(fileName[i], buffer, FILE_NAME_OFFSET + i, sizeof(char));
    }
    return entryLength;
}

Directory *Directory::rootOf(FSPartition *partition)
{
    File *file = new File("", partition->getInode(0), partition);
    return new RootDirectory(file, partition);
}

void Directory::unlink(string &fileName)
{
    throw NotImplementedYetException();
//    Inode *inode = getInode(fileName);
//    entries.erase(fileName);
//    inode->removeLink();
//    flush();
}

File Directory::getFile(string &fileName)
{
    Inode *inode = getInode(fileName);
    return File(fileName, inode, partition);
}

vector<File> Directory::getFileList()
{
    vector<File> result;
    result.reserve(entries.size());
    for (auto &entry : entries) {
        result.push_back(File(entry.first, getInode(entry.first), partition));
    }
    return result;
}

unsigned Directory::getFileCount()
{
    return entries.size();
}
