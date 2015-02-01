#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

class Encryptor
{
public:
    virtual ~Encryptor();
    virtual void setBlockSize(int blockSize) = 0;
    virtual void generateKeyBlock(char *buffer) = 0;
    virtual void addKey(const char *keyBlock) = 0;
    virtual void encryptBlock(const char *in, char *out, int keyId) = 0;
    virtual void decryptBlock(const char *in, char *out, int keyId) = 0;
};

#endif // ENCRYPTOR_H
