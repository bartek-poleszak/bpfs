#include "prompt.h"

Prompt::Prompt()
{
    this->devicePath = "???";
}

void Prompt::setDevicePath(const char *path)
{
    this->devicePath = path;
}

string Prompt::render(deque<Directory *> directories)
{
    string result = "";
    result.append(devicePath);
    result.append("::");
    result.append(renderCurrentDirectoryPath(directories));
    result.append("> ");
    return result;
}

string Prompt::renderCurrentDirectoryPath(deque<Directory *> directories) {
    if (directories.size() == 0)
        return "???";
    return "/";
}
