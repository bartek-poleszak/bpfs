#define FUSE_USE_VERSION 26
#include <iostream>
#include <fuse.h>
#include <unistd.h>
#include <sys/types.h>
#include <cstring>
#include "fspartition.h"
#include "directory.h"
#include "filedisk.h"
#include "utils.h"
#include "diskmatrix.h"

using namespace std;

namespace bpfs {

IDisk *disk;
FSPartition *partition;
Directory *rootDirectory;
ostream &logStream = cout;
string devicePath;

File *getFileFromPath(const char *path)
{
    string fileName(path + 1);
    File *file = rootDirectory->getFile(fileName);
    return file;
}

int getattr (const char *path, struct stat *buffer) {
    string pathString(path);

    memset(buffer, 0, sizeof(struct stat));
    File *file;
    if (pathString == "/") {
        file = rootDirectory->getAsFile();
        buffer->st_mode = S_IFDIR | 0644;
        buffer->st_nlink = 2;
    }
    else {
        try {
            string fileName = Utils::getFileNameFromPath(pathString);
            file = rootDirectory->getFile(fileName);
        }
        catch (FileDosentExistException e) {
            return -ENOENT;
        }
        buffer->st_mode = S_IFREG | 0644;
        buffer->st_nlink = 1;
    }

    blkcnt_t halfKbBlockCount = file->getTotalSizeInBytes() / 512;
    if (file->getTotalSizeInBytes() % 512 > 0)
        halfKbBlockCount++;

    buffer->st_ino = file->getInode()->getId();
    buffer->st_uid = getuid();
    buffer->st_gid = getgid();
    buffer->st_size = file->getTotalSizeInBytes();
    buffer->st_blksize = partition->getBlockSize();
    buffer->st_blocks = halfKbBlockCount;

    return 0;
}

int readlink (const char *path, char *buffer, size_t bufferSize) { return 1; }

int mknod (const char *path, mode_t mode, dev_t dev)
{
    if ( !((mode & S_IFREG) == S_IFREG) )
        return -EPERM;

    Inode *newInode = partition->getFreeInode();
    newInode->clear();
    string fileName = Utils::getFileNameFromPath(path);
    rootDirectory->link(fileName, newInode);

    return 0;
}

int mkdir (const char *path, mode_t mode) { return 1; }
int unlink (const char *path) { return 1; }
int rmdir (const char *path) { return 1; }
int symlink (const char *oldPath, const char *newPath) { return 1; }
int rename (const char *oldPath, const char *newPath) { return 1; }
int link (const char *oldPath, const char *newPath) { return 1; }
int chmod (const char *path, mode_t mode) { return 1; }
int chown (const char *path, uid_t userId, gid_t groupId) { return 1; }

int truncate (const char *path, off_t length)
{
    try {
        File *file = getFileFromPath(path);
        file->truncate(length);
    }
    catch (FileDosentExistException e) {
        return -ENOENT;
    }

    return 0;
}

int open (const char *path, struct fuse_file_info *fileInfo)
{
//    if ((fileInfo->flags & 3) != O_RDONLY)
//        return -EACCES;
    return 0;
}

int read (const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fileInfo)
{
    int result;
    try {
        File *file = getFileFromPath(path);
        result = file->read(buffer, size, offset);
    }
    catch (FileDosentExistException e) {
        result = 0;
    }

    return result;
}

int write (const char *path, const char *buffer, size_t size, off_t offset,
          struct fuse_file_info *fileInfo)
{
    int result;
    try {
        File *file = getFileFromPath(path);
        result = file->write(buffer, size, offset);
    }
    catch (FileDosentExistException e) {
        result = 0;
    }
    return result;
}
int statfs (const char *path, struct statvfs *buffer) { return 1; }
int flush (const char *path, struct fuse_file_info *fileInfo) { return 1; }
int release (const char *path, struct fuse_file_info *fileInfo) { return 1; }
int fsync (const char *path, int datasync, struct fuse_file_info *fileInfo) { return 1; }
int setxattr (const char *path, const char *name, const char *value, size_t size, int flags) { return 1; }
int getxattr (const char *path, const char *name, char *value, size_t size) { return 1; }
int listxattr (const char *path, char *list, size_t size) { return 1; }
int removexattr (const char *path, const char *name) { return 1; }
int opendir (const char *path, struct fuse_file_info *fileInfofiseFileInfo) {
    return 0;
}

int readdir (const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset,
        struct fuse_file_info *fileInfo)
{
    if (string(path) != "/")
        return -ENOENT;
    vector<File *> fileList = rootDirectory->getFileList();
    filler(buffer, ".", nullptr, 0);
    filler(buffer, "..", nullptr, 0);
    for (File *file : fileList) {
        filler(buffer, file->getName().c_str(), nullptr, 0);
    }
    return 0;
}

int releasedir (const char *path, struct fuse_file_info *fileInfo) { return 1; }
int fsyncdir (const char *path, int datasync, struct fuse_file_info *fileInfo) { return 1; }

void *init (struct fuse_conn_info *conn)
{
    partition = new FSPartition(*disk);
    rootDirectory = Directory::rootOf(partition);
    return nullptr;
}

void destroy (void *userData)
{
    logStream << "destroy" << endl;

    if (rootDirectory != nullptr)
        delete rootDirectory;
    if (partition != nullptr)
        delete partition;
    if (disk != nullptr)
        delete disk;
}

int access (const char *path, int mask) {
    return 0;
}
int create (const char *path, mode_t mode, struct fuse_file_info *fileInfo) { return 1; }
int ftruncate (const char *path, off_t offset, struct fuse_file_info *fileInfo) { return 1; }
int fgetattr (const char *path, struct stat *statBuffer, struct fuse_file_info *fileInfo) { return 1; }
int lock (const char *path, struct fuse_file_info *fileInfo, int cmd,
         struct flock *flockBuffer) { return 1; }
int utimens (const char *path, const struct timespec tv[2]) { return 1; }
int bmap (const char *path, size_t blocksize, uint64_t *idx) { return 1; }
int ioctl (const char *path, int cmd, void *arg,
          struct fuse_file_info *fileInfo, unsigned int flags, void *data) { return 1; }
int poll (const char *path, struct fuse_file_info *fileInfo,
         struct fuse_pollhandle *ph, unsigned *reventsp) { return 1; }
int write_buf (const char *path, struct fuse_bufvec *buf, off_t off,
          struct fuse_file_info *fileInfo) { return 1; }
int read_buf (const char *path, struct fuse_bufvec **bufp,
         size_t size, off_t off, struct fuse_file_info *fileInfo) { return 1; }
int flock (const char *path, struct fuse_file_info *fileInfo, int op) { return 1; }
int fallocate (const char *path, int mode, off_t offset, off_t length,
          struct fuse_file_info *fileInfo) { return 1; }

}

void showUsageNote(char *execName) {
    cout << "Usage:\n"
         << execName << " /dev/<device> <mountpoint>" << endl
         << execName << " matrix /dev/<device0> /dev/<device1> ... /dev/<devicen> <mountpoint>" << endl;
    exit(EXIT_FAILURE);
}

void validateDeviceName(char *argv[]) {
    if (strncmp("/dev/", argv[1], 5))
        showUsageNote(argv[0]);
}

void initializeMatrix(int deviceCount, char *devicePaths[]) {
    vector<string> paths;
    for(int i = 0; i < deviceCount; i++)
        paths.push_back(string(devicePaths[i]));
    bpfs::disk = new DiskMatrix(paths, 1024);
}

void initializeFileDisk(char *path) {
    bpfs::disk = new FileDisk(path, 1024);
}

int main(int argc, char *argv[]) {
    if (argc < 3)
        showUsageNote(argv[0]);
    if (strcmp("matrix", argv[1]) == 0)
        initializeMatrix(argc - 3, argv + 2);
    else {
        validateDeviceName(argv);
        initializeFileDisk(argv[1]);
    }
    bpfs::devicePath = argv[1];

    char debugFlag[3] = "-d";
    char singleThreadFlag[3] = "-s";

    int fuseArgc = 4;
    char *fuseArgv[fuseArgc + 1];
    fuseArgv[0] = argv[0];
    fuseArgv[1] = argv[argc - 1];
    fuseArgv[2] = debugFlag;
    fuseArgv[3] = singleThreadFlag;
    fuseArgv[fuseArgc] = 0;

    fuse_operations bpfsOperations;
    memset(&bpfsOperations, 0, sizeof(struct fuse_operations));
    bpfsOperations.getattr = bpfs::getattr;
    bpfsOperations.truncate = bpfs::truncate;
    bpfsOperations.open = bpfs::open;
    bpfsOperations.read = bpfs::read;
    bpfsOperations.readdir = bpfs::readdir;
    bpfsOperations.opendir = bpfs::opendir;
    bpfsOperations.write = bpfs::write;
    bpfsOperations.init = bpfs::init;
    bpfsOperations.destroy = bpfs::destroy;
    bpfsOperations.mknod = bpfs::mknod;
////    bpfsOperations.access = bpfs::access;
    fuse_main(fuseArgc, fuseArgv, &bpfsOperations, nullptr);
    return 0;
}
