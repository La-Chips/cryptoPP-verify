#pragma once 

#include <iostream>
#include <openssl/x509v3.h> //x509 implementation for compatibility
#include <openssl/bn.h> // 
#include <openssl/asn1.h>
#include <openssl/x509.h> // x509 implementation
#include <openssl/x509_vfy.h> 
#include <openssl/pem.h> // for reading certificates & keys
#include <openssl/bio.h>
#include <openssl/err.h>
#include <assert.h>
#include <fstream>
#include <cryptopp/files.h>
#include <cryptopp/rsa.h>
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pssr.h>

class OpenSSL
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
    OpenSSL(/* args */);
    ~OpenSSL();

    void sign();


    bool verify(CryptoPP::SecByteBlock signature);
    


    void test();
};

