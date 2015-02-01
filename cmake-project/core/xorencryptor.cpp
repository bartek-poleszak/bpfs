#include <cstdlib>
#include <ctime>
#include "xorencryptor.h"

XorEncryptor::XorEncryptor()
{
    srand(time(nullptr));
}

XorEncryptor::~XorEncryptor()
{
    for (unsigned char *key : keys)
        delete [] key;
}

void XorEncryptor::setBlockSize(int blockSize)
{
    if (blockSize < KEY_SIZE)
        throw TooSmallBlockSizeForXorEncryptor();
    this->blockSize = blockSize;
}

void XorEncryptor::generateKeyBlock(char *buffer)
{
    unsigned char key[KEY_SIZE];
    for (int i = 0; i < KEY_SIZE; i++)
        key[i] = rand() % 256;

    for (int i = 0; i < KEY_SIZE; i++)
        buffer[i] = key[i];
}

void XorEncryptor::addKey(const char *keyBlock)
{
    unsigned char *key = new unsigned char[KEY_SIZE];
    for (int i = 0; i < KEY_SIZE; i++)
        key[i] = keyBlock[i];

    keys.push_back(key);
}

void XorEncryptor::encryptBlock(const char *in, char *out, int keyId)
{
    unsigned char *key = keys[keyId];
    for (int i = 0; i < blockSize; i++)
        out[i] = in[i] ^ key[i % KEY_SIZE];
}

void XorEncryptor::decryptBlock(const char *in, char *out, int keyId)
{
    encryptBlock(in, out, keyId);
}
