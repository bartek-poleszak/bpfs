#ifndef DUMMYENCRYPTOR_H
#define DUMMYENCRYPTOR_H

#include <exception>
#include <vector>
#include "encryptor.h"

class TooSmallBlockSizeForXorEncryptor : public std::exception {};

class XorEncryptor : public Encryptor
{
private:
    static const int KEY_SIZE = 512;
    int blockSize;
    std::vector<unsigned char *> keys;
public:
    XorEncryptor();
    ~XorEncryptor() override;
    void setBlockSize(int blockSize) override;
    void generateKeyBlock(char *buffer) override;
    void addKey(const char *keyBlock) override;
    void encryptBlock(const char *in, char *out, int keyId) override;
    void decryptBlock(const char *in, char *out, int keyId) override;
};

#endif // DUMMYENCRYPTOR_H
