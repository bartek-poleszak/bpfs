#include <iostream>
#include "filedisk.h"
#include "idisk.h"
#include "partitionheader.h"
#include "rawdatautils.h"

using namespace std;

void testOk() {
    cout << "\tOK" << endl;
}

void testFail() {
    cout << "\tFAIL" << endl;
}

void test(bool testCase) {
    if (testCase)
        testOk();
    else
        testFail();
}

void testZapisuIOdczytuUintZBufora() {
    cout << "testZapisuIOdczytuUintZBufora";

    //given
    char buffer[100] = {0};
    unsigned value = 2349874399;
    unsigned position = 89;
    //when

    RawDataUtils::writeUintToBuffer(value, buffer, position, sizeof(value));
    unsigned resultValue = RawDataUtils::readUintFromBuffer(buffer, position, sizeof(resultValue));
    //then
    test(resultValue == value);
}

void deployTests() {
    testZapisuIOdczytuUintZBufora();
}

#include "inode.h"
#include "file.h"
#include <fstream>



int main/*Write*/(int argc, char *argv[])
{
//    if (argc != 2) {
//        throw "Zla liczba argumentow!";
//    }
//    FileDisk disk(argv[1], 1024);
    FileDisk disk("/dev/sdb1", 1024);
////    PartitionHeader header;
////    header.writeToDisk(disk, 256);

    PartitionHeader header(disk);
    cout << "Block count: " << header.getBlockCount() << endl
         << "Block size: " << header.getBlockSize() << endl
         << "Inode size: " << header.getInodeSize() << endl
         << "Inode count: " << header.getInodeCount() << endl;
////    deployTests();

    FSPartition partition(&disk);
    Inode *inode = partition.getInode(0);
    inode->clearForDebug();
    File file(inode, &partition);

    char napis[100] = "To jest testowy napis!!!\n";
    ifstream fileIn;
    fileIn.open("/home/bp/fileIn");
    file.write(fileIn);
    fileIn.close();
    file.append(napis, 25);
    partition.flushInodeTable();

    ofstream plikOut;
    plikOut.open("/home/bp/fileOut2");
    cout << "Odczyt" << endl;
    file.get(plikOut);
    plikOut.close();

    /*FSPartition partition(&disk);
    INode inode(256);
    File file(&inode, &partition);

    ifstream plikIn;
    plikIn.open("/home/bp/fileIn");

    ofstream plikOut;
    plikOut.open("/home/bp/fileOut");

    cout << "Zapis" << endl;
    file.write(plikIn);
    cout << "Odczyt" << endl;
    file.get(plikOut);

    plikIn.close();
    plikOut.close();*/

    return 0;
}
#include "fscreator.h"
int mainn() {
    FileDisk disk("/dev/sdb1", 1024);
    FSCreator creator;
    creator.createFilesystem(disk, 1024, 256, 4);
    return 0;
}
