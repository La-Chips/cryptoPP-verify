#include "../include/Sign.hpp"

using namespace std;

Sign::Sign(/* args */)
{
}

Sign::~Sign()
{
}

CryptoPP::RSA::PrivateKey Sign::readPrivateKey(string filename)
{
  CryptoPP::RSA::PrivateKey key;

  CryptoPP::FileSource input(filename.c_str(), true);
  key.BERDecode(input);
  return key;
}

CryptoPP::RSA::PublicKey Sign::readPublicKey(string filename)
{
  CryptoPP::RSA::PublicKey key;

  CryptoPP::FileSource input(filename.c_str(), true);
  key.BERDecode(input);
  return key;
}

void Sign::writePrivateKey(CryptoPP::RSA::PrivateKey key, std::string filename)
{
  // CryptoPP::FileSink::Output output(filename);
  // key.DEREncode(output);
}

void Sign::writePublicKey(CryptoPP::RSA::PublicKey key, std::string filename)
{
  // CryptoPP::FileSink output(filename);
  //  key.DEREncode(output);
}

void Sign::saveSignature(CryptoPP::SecByteBlock signature, string filename)
{
    string output;
		CryptoPP::StringSink sink(output);
		sink.Put(signature, signature.size());

  ofstream MyFile(filename);

  MyFile << output;

  MyFile.close();
}

CryptoPP::SecByteBlock Sign::loadSignature(string filename)
{
  std::ifstream ifs(filename.c_str());
  std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );

  CryptoPP::SecByteBlock signature((const CryptoPP::byte *)content.data(), content.size());


  return signature;
}

void Sign::generateKeys()
{

  CryptoPP::InvertibleRSAFunction parameters;
  parameters.GenerateRandomWithKeySize(this->rng, 3072);

  CryptoPP::RSA::PrivateKey privateKey(parameters);
  CryptoPP::RSA::PublicKey publicKey(parameters);

  // this->writePrivateKey(privateKey, "priv.dat");
  // this->writePublicKey(publicKey, "pub.dat");
}

string Sign::getFileData(string filename)
{
  FILE *myfile = fopen(filename.c_str(), "r");

  string message;
  if (myfile != NULL)
  {
    short stringlength = 6;
    string mystring(stringlength, '\0');
    fseek(myfile, 0, SEEK_SET);
    fread(&message[0], sizeof(char), (size_t)stringlength, myfile);

    fclose(myfile);
  }

  return message;
}

void Sign::sign()
{
  CryptoPP::RSA::PrivateKey privateKey = this->readPrivateKey("priv.dat");
  string message = this->getFileData("a.xml");

  // Signer object
  CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Signer signer(privateKey);

  // Create signature space
  size_t length = signer.MaxSignatureLength();
  CryptoPP::SecByteBlock signature(length);

  // Resize now we know the true size of the signature
  signature.resize(length);

  // Sign message
  length = signer.SignMessage(this->rng, (const CryptoPP::byte *)message.c_str(),
                              message.length(), signature);

  
  this->saveSignature(signature, "sign.dat");

}

void Sign::test()
{

  this->sign();
  this->verify(this->loadSignature("sign.dat"));
}

bool Sign::verify(CryptoPP::SecByteBlock signature)
{
  CryptoPP::RSA::PublicKey publicKey = this->readPublicKey("pub.dat");

  // Verifier object
  CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Verifier verifier(publicKey);
  string message = this->getFileData("a.xml");

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