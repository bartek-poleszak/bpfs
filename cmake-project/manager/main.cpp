#include <iostream>
#include <fstream>
#include "main.h"

int main(/*int argc, char *argv[]*/) {
    int argc = 2;
    const char *argv[argc];
    argv[1] = "/dev/sdb1";
    if (argc != 2)
        throw "Zla liczba argumentow!";
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
    for (File &file : files)
        cout << file.getName() << " ";
    cout << endl;
}

void ConsoleInterface::printFileToStdOut()
{
    string fileName;
    cin >> fileName;
    File file = currentDirectory()->getFile(fileName);
    char buffer[partition->getBlockSize()];
    BlockSize bytesRead;
    while ((bytesRead = file.read(buffer, partition->getBlockSize())) > 0) {
        for (unsigned i = 0; i < bytesRead; ++i) {
            cout << buffer[i];
        }
    }
}

void ConsoleInterface::main(const char *diskPath)
{
    prompt.setDevicePath(diskPath);
    string command;
    FileDisk disk(diskPath, 1024);

    while (true) {
        cout << prompt.render(directories);
        cin >> command;
        if (command == Command::EXIT)
            break;

        if (partition == nullptr) {
            if (command == Command::MAKE_FILESYSTEM)
                commandResolver.makeFilesystem(disk);
            else if (command == Command::INIT)
                initializeFilesystem(disk);
            else
                unknownCommand();
        }
        else {
            if (command == Command::PUT)
                put();
            else if (command == Command::LIST)
                list();
            else if (command == Command::CAT)
                printFileToStdOut();
            else
                unknownCommand();
        }
    }

}
