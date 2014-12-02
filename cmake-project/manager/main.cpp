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
    File file0("b", inode, &partition);

    Inode *inode2 = partition.getInode(2);
    File file2("a", inode2, &partition);

//    inode2->clearForDebug();
//    inode->clearForDebug();

//    cout << "Zapis0" << endl;
////    char napis[100] = "To jest testowy napis!!!\n";
//    ifstream fileIn;
//    fileIn.open("/home/bp/fileIn");
//    file0.write(fileIn);
//    fileIn.close();
////    file0.append(napis, 25);

//    cout << "Zapis1" << endl;
//    ifstream fileIn2;
//    fileIn2.open("/home/bp/fileIn2");
//    file2.write(fileIn2);
//    fileIn2.close();
//    partition.flushInodeTable();

    cout << "Odczyt0" << endl;
    ofstream plikOut;
    plikOut.open("/home/bp/fileOut");
    file0.get(plikOut);
    plikOut.close();

    cout << "Odczyt2" << endl;
    ofstream plikOut2;
    plikOut2.open("/home/bp/fileOut2");
    file2.get(plikOut2);
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
