#pragma once

#include <iostream>
#include <assert.h>
#include <fstream>
#include <cryptopp/files.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/oids.h>
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pssr.h>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
using json = nlohmann::json;

namespace fs = std::filesystem;
namespace greg = boost::gregorian;

class Sign
{
private:
    CryptoPP::AutoSeededRandomPool rng;

    void generate_keys();
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey read_private_key(std::string filename);
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey read_public_key(std::string filename);

    void write_private_key(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey key, std::string filename);
    void write_public_key(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey key, std::string filename);

    void save_signature(CryptoPP::SecByteBlock signature, std::string filename);
    CryptoPP::SecByteBlock load_signature(std::string filename);

    std::string get_file_data(std::string filename);
    std::string add_date_to_fingerprint(std::string &content, std::string label, greg::date date);
    greg::date ask_date();

public:
    Sign(/* args */);
    ~Sign();

    void sign();

    bool verify(CryptoPP::SecByteBlock signature, char *msg);

    void test();
};
