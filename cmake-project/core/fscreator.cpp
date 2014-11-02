#include "fscreator.h"

void FSCreator::createHeader(IDisk &partition)
{

}

void FSCreator::createINodeTable(IDisk &partition)
{

}

FSCreator::FSCreator(int blockSize)
{
}

FSPartition FSCreator::createFilesystem(IDisk &partition)
{
    createHeader(partition);
    createINodeTable(partition);
}


