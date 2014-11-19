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

int main()
{
    FileDisk disk("/dev/sda2", 1024);
//    PartitionHeader header;
//    header.writeToDisk(disk, 256);

    PartitionHeader header(disk);
    cout << "Block number: " << header.getBlockNumber() << endl
         << "Block size: " << header.getBlockSize() << endl
         << "Inode size: " << header.getINodeSize() << endl;
//    deployTests();
    return 0;
}

