#include "../include/Sign.hpp"

using namespace std;

Sign::Sign(/* args */)
{
}

Sign::~Sign()
{
}

CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey Sign::read_private_key(string filename)
{
  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey key;

  CryptoPP::FileSource input(filename.c_str(), true);
  key.BERDecode(input);
  return key;
}

CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey Sign::read_public_key(string filename)
{
  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey key;

  CryptoPP::FileSource input(filename.c_str(), true);
  key.BERDecode(input);
  return key;
}

void Sign::write_private_key(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey key, std::string filename)
{
  CryptoPP::FileSink output(filename.c_str());
  key.DEREncode(output);
}

void Sign::write_public_key(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey key, std::string filename)
{
  CryptoPP::FileSink output(filename.c_str());
  key.DEREncode(output);
}

void Sign::save_signature(CryptoPP::SecByteBlock signature, string filename)
{
  string output;
  CryptoPP::StringSink sink(output);
  sink.Put(signature, signature.size());

  ofstream MyFile(filename);

  MyFile << output.c_str();

  MyFile.close();
}

CryptoPP::SecByteBlock Sign::load_signature(string filename)
{
  std::ifstream ifs(filename.c_str());
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));

  CryptoPP::SecByteBlock signature((const CryptoPP::byte *)content.data(), content.size());

  return signature;
}

void Sign::generate_keys()
{

  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey;
  CryptoPP::DL_GroupParameters_EC<CryptoPP::ECP> params(CryptoPP::ASN1::secp256k1());

  privateKey.Initialize(this->rng, params);

  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey;

  privateKey.MakePublicKey(publicKey);

  this->write_private_key(privateKey, "priv.dat");
  this->write_public_key(publicKey, "pub.dat");
}

string Sign::get_file_data(string filepath)
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

      greg::date now = greg::day_clock::universal_day();

      if (now >= date)
      {
        std::cout << "\x1B[31m\nExpected futur date\n\033[0m\t\t" << std::endl;
        valid = false;
      }
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

  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey = this->read_private_key("priv.dat");

  std::string fingerprint = this->get_file_data("./fingerprint.dat");

  fingerprint = this->add_date_to_fingerprint(fingerprint, "creation_date");
  fingerprint = this->add_date_to_fingerprint(fingerprint, "expiration_date", this->ask_date());

  // Signer object
  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Signer signer(privateKey);

  // Create signature space
  size_t length = signer.MaxSignatureLength();
  CryptoPP::SecByteBlock signature(length);

  // Resize now we know the true size of the signature
  signature.resize(length);

  // Sign message
  length = signer.SignMessage(this->rng, (const CryptoPP::byte *)fingerprint.c_str(),
                              fingerprint.size(), signature);

  string output;
  CryptoPP::StringSink sink(output);
  sink.Put(signature, signature.size());

  ofstream file("licence.bin", ofstream::binary);

  int number = length;

  file.write(reinterpret_cast<const char *>(&number), sizeof number);
  file.write(output.c_str(), number);
  file.write(fingerprint.c_str(), fingerprint.size());

  file.close();
}

bool Sign::verify(CryptoPP::SecByteBlock signature, char *msg)
{

  CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey = this->read_public_key("pub.dat");

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

void Sign::test(){
  this->generate_keys();
  this->sign();
}