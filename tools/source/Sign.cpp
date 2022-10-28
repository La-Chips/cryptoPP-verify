#include "../include/Sign.hpp"
#include "../../os/include/Linux.hpp"

using namespace std;

Sign::Sign(/* args */)
{
}

Sign::~Sign()
{
}

CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey Sign::readPrivateKey(string filename)
{
  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey key;

  CryptoPP::FileSource input(filename.c_str(), true);
  key.BERDecode(input);
  return key;
}

CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey Sign::readPublicKey(string filename)
{
  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey key;

  CryptoPP::FileSource input(filename.c_str(), true);
  key.BERDecode(input);
  return key;
}

void Sign::writePrivateKey(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey key, std::string filename)
{
  CryptoPP::FileSink output(filename.c_str());
  key.DEREncode(output);
}

void Sign::writePublicKey(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey key, std::string filename)
{
  CryptoPP::FileSink output(filename.c_str());
  key.DEREncode(output);
}

void Sign::saveSignature(CryptoPP::SecByteBlock signature, string filename)
{
  string output;
  CryptoPP::StringSink sink(output);
  sink.Put(signature, signature.size());




  // MyFile << output;

  // MyFile.close();
}

CryptoPP::SecByteBlock Sign::loadSignature(string filename)
{
  std::ifstream ifs(filename.c_str());
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));

  auto end = content.find("json");
  if (end != std::string::npos)
  {
    content = content.substr(0, end);
  }

  CryptoPP::SecByteBlock signature((const CryptoPP::byte *)content.data(), content.size());

  return signature;
}

void Sign::generateKeys()
{

  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey;
  CryptoPP::DL_GroupParameters_EC<CryptoPP::ECP> params(CryptoPP::ASN1::secp256k1());

  privateKey.Initialize(this->rng, params);

  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey;

  privateKey.MakePublicKey(publicKey);

  this->writePrivateKey(privateKey, "priv.dat");
  this->writePublicKey(publicKey, "pub.dat");
}

string Sign::getFileData(string filepath)
{

  if (!fs::exists(filepath.c_str()))
  {
    throw std::runtime_error("file not found");
  }

  std::ifstream infile(filepath, std::ifstream::binary);
  infile.seekg(0);

  size_t fingerprint_length = sizeof infile;

  char *message = new char[fingerprint_length];
  infile.read(message, fingerprint_length);
  infile.close();

  return message;
}

std::string Sign::add_date_to_fingerprint(std::string &content, std::string label, greg::date date = greg::day_clock::universal_day())
{
  json fingerprint = json::parse(content);

  

  std::string string_date = greg::to_iso_extended_string(date);

  fingerprint.push_back({label, string_date});

  std::cout << fingerprint.dump() << std::endl;

  return fingerprint.dump();
}

greg::date Sign::ask_date()
{
  bool valid = false;
  greg::date date;

  while (!valid)
  {
    try
    {
      std::string date_string;
      std::cout << "Input date (e.g :2022-2-25) :" << std::endl;
      std::cin >> date_string;

       date = greg::from_simple_string(date_string);
      valid = true;
    }
    catch (const boost::exception &e)
    {
      std::cout << "\x1B[31m\nInvalid Date format\n\033[0m\t\t" << std::endl;
    }
  }

  return date;

}

void Sign::sign()
{

  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey = this->readPrivateKey("priv.dat");
  Linux os;

std::string fingerprint = this->getFileData("./fingerprint.dat");

  fingerprint = this->add_date_to_fingerprint(fingerprint, "creation_date");
  fingerprint = this->add_date_to_fingerprint(fingerprint,"expiration_date",this->ask_date());

  // Signer object
  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Signer signer(privateKey);

  // Create signature space
  size_t length = signer.MaxSignatureLength();
  CryptoPP::SecByteBlock signature(length);

  // Resize now we know the true size of the signature
  signature.resize(length);

  std::cout << length << std::endl;

  // Sign message
  length = signer.SignMessage(this->rng, (const CryptoPP::byte *)fingerprint.c_str(),
                              fingerprint.size(), signature);

  // this->saveSignature(signature, "sign.dat");

  string output;
  CryptoPP::StringSink sink(output);
  sink.Put(signature, signature.size());

  ofstream file("sign.dat", ofstream::binary);

  int number = length;

  file.write(to_string(number).c_str(), sizeof number);
  file.write(output.c_str(), number);
  file.write(fingerprint.c_str(), fingerprint.size());

  file.close();
}

void Sign::test()
{
  // this->generateKeys();
  this->sign();
  // this->verify(this->loadSignature("sign.dat"));
}

bool Sign::verify(CryptoPP::SecByteBlock signature, char *msg)
{

  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey = this->readPublicKey("pub.dat");

  // Verifier object
  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Verifier verifier(publicKey);
  string message = msg;

  // Verify
  bool result = verifier.VerifyMessage((const CryptoPP::byte *)message.c_str(),
                                       message.length(), signature, signature.size());

  // Result
  if (result)
  {
    cout << "Signature on message verified" << endl;
  }
  else
  {
    cout << "Message verification failed" << endl;
  }
  return result;
}