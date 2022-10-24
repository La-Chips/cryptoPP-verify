#pragma once 

#include <iostream>
#include <assert.h>
#include <fstream>
#include <cryptopp/files.h>
#include <cryptopp/rsa.h>
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pssr.h>

class Sign
{
private:
    CryptoPP::AutoSeededRandomPool rng;

    void generateKeys();
    CryptoPP::RSA::PrivateKey readPrivateKey(std::string filename);
    CryptoPP::RSA::PublicKey readPublicKey(std::string filename);

    void writePrivateKey(CryptoPP::RSA::PrivateKey key, std::string filename);
    void writePublicKey(CryptoPP::RSA::PublicKey key, std::string filename);

    void saveSignature(CryptoPP::SecByteBlock signature,std::string filename);
    CryptoPP::SecByteBlock loadSignature(std::string filename);

    std::string getFileData(std::string filename);
public:
    Sign(/* args */);
    ~Sign();

    void sign();


    bool verify(CryptoPP::SecByteBlock signature);
    


    void test();
};

