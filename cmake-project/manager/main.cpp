#include <iostream>
#include <fstream>
#include <exception>
#include "main.h"
#include "diskmatrix.h"
#include "xorencryptor.h"

class TooFewArgumentsException : public std::exception {};

int main(int argc, char *argv[]) {
    if (argc != 2)
        throw TooFewArgumentsException();
    deque<Directory *> directories;
    ConsoleInterface console(directories);
    console.main(argv[1]);
}


ConsoleInterface::ConsoleInterface(deque<Directory *> &directories)
    : directories(directories)
{

}

ConsoleInterface::~ConsoleInterface()
{
    if (partition != nullptr)
        delete partition;
}

void ConsoleInterface::initializeFilesystem(IDisk &disk)
{
    partition = commandResolver.loadFilesystem(disk);
    if (partition != nullptr) {
        directories.push_back(Directory::rootOf(partition));
    }
}

Directory *ConsoleInterface::currentDirectory()
{
    return directories.back();
}

void ConsoleInterface::unknownCommand()
{
    cout << "Unknown Command" << endl;
}

string ConsoleInterface::resolveFileName(string &fullPath) {
    auto lastSlash = fullPath.find_last_of("/");
    if (lastSlash == string::npos)
        return fullPath;
    return fullPath.substr(lastSlash + 1, fullPath.size());
}

void ConsoleInterface::put()
{
    string fileInPath, fileName;
    cin >> fileInPath;
    fileName = resolveFileName(fileInPath);

    ifstream fileIn;
    fileIn.open(fileInPath);
    File file(fileName, partition->getFreeInode(), partition);
    file.write(fileIn);
    currentDirectory()->link(file);
    partition->flushInodeTable();
}

void ConsoleInterface::list()
{
    auto files = directories.back()->getFileList();
    for (File *file : files)
        cout << file->getName() << " ";
    cout << endl;
}

void ConsoleInterface::printFileToStdOut()
{
    string fileName;
    cin >> fileName;
    File *file = currentDirectory()->getFile(fileName);
    char buffer[partition->getBlockSize()];
    BlockSize bytesRead;
    while ((bytesRead = file->read(buffer, partition->getBlockSize())) > 0) {
        for (unsigned i = 0; i < bytesRead; ++i) {
            cout << buffer[i];
        }
    }
}

void ConsoleInterface::makeMatrix(const char *diskPath)
{
    vector<string> paths;
    int matrixSize;
    cout << "Matrix size: ";
    cin >> matrixSize;
    if (matrixSize < 2) {
        cout << "Matrix must contain at least 2 devices" << endl;
    }
    else {
        paths.push_back(diskPath);
        cout << "Device 0: " << diskPath << endl;
        string path;
        for (int i = 1; i < matrixSize; i++) {
            cout << "Device " << i << ": ";
            cin >> path;
            paths.push_back(path);
        }
        XorEncryptor encryptor;
        DiskMatrix diskMatrix(paths, 1024, encryptor, true);
        commandResolver.makeFilesystem(diskMatrix);
    }
}

void ConsoleInterface::main(const char *diskPath)
{
    prompt.setDevicePath(diskPath);
    string command;

    while (true) {
        cout << prompt.render(directories);
        cin >> command;
        if (command == Command::EXIT)
            break;

        if (command == Command::MAKE_FILESYSTEM) {
            FileDisk disk(diskPath, 1024);
            commandResolver.makeFilesystem(disk);
        }
        else if (command == Command::MAKE_MATRIX) {
            makeMatrix(diskPath);
        }
        else
            unknownCommand();

    }

}
