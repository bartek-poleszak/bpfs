#include <iostream>
#include "xorencryptor.h"

using namespace std;

void encryptionTest() {
    XorEncryptor encryptor;
    const int blockSize = 512;
    encryptor.setBlockSize(blockSize);
    char txt[blockSize] = "123456789";
    char encrypted[blockSize];
    char decrypted[blockSize];

    cout << "before: ";
    for (int i = 0; i < blockSize; i++)
        cout << txt[i];
    cout << endl;

    encryptor.encryptBlock(txt, encrypted, 0);
    cout << "encrypted: ";
    for (int i = 0; i < blockSize; i++)
        cout << encrypted[i];
    cout << endl;

    encryptor.decryptBlock(encrypted, decrypted, 0);
    cout << "decrypted: ";
    for (int i = 0; i < blockSize; i++)
        cout << decrypted[i];
    cout << endl;
}

int main() {
    encryptionTest();
}
