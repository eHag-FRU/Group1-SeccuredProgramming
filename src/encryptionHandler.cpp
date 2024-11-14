#include "encryptionHandler.hpp"
#include <iostream>
#include <aes.h>
#include <modes.h>
#include <filters.h>

using std::string; using std::map; using std::cout; using std::endl;


std::string encrypt(const std::string& message, const std::string& key, map<string, string>& input)
{
    //Makes an internal copy of the key incase padding is needed
    string newKey = string(key.c_str());

    cout << key.size() % CryptoPP::AES::BLOCKSIZE << endl;
    //Check if the size of the message is a multiple of 16, if not pad until then
    if ((key.size() % CryptoPP::AES::BLOCKSIZE) > 0) {
        //Key is not of factor 16

        if (int(newKey.size()) < CryptoPP::AES::BLOCKSIZE) {
            //Insert the number of key.size() - 16 0's to the start
            //of the string
            int numberOf0s = CryptoPP::AES::BLOCKSIZE - int(newKey.size());
            newKey.insert(int(key.size()), numberOf0s, '0');
        } else {
            //Keysize is larger

            //Determine what multiple of 16 is present
            int numberOf16sInKeyLen = int(newKey.size()) / CryptoPP::AES::BLOCKSIZE;

            //Determine the number of 0's needed to complete the next block of 16
            int numberOf0s = CryptoPP::AES::BLOCKSIZE - (int(newKey.size()) - (CryptoPP::AES::BLOCKSIZE * numberOf16sInKeyLen));

            //Insert the number of 0's needed to complete the block of 16 to the front
            newKey.insert(int(key.size()), numberOf0s, '0');
        }
    }

    //Have this be put into the resulting key
    //input["-K"] = newKey;

    using namespace CryptoPP;
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x00, AES::BLOCKSIZE); // Initialize IV to all zeros
    std::string encryptedMessage;
    try{
        CBC_Mode<AES>::Encryption encryption;
        encryption.SetKeyWithIV((const byte*)newKey.data(), newKey.size(), iv);
            StringSource(message, true,
            new StreamTransformationFilter(encryption,
            new StringSink(encryptedMessage)
            )
        );
    }
    catch (const Exception& ex)
    {
        // Handle exception
        std::cerr << "ENCRYPTION ERROR: " << ex.GetWhat() << endl;
    }
    return encryptedMessage;
}


std::string decrypt(const std::string& encryptedMessage, const std::string& key) {
    cout << "=====DECRYPT=====" << endl;

    //Makes an internal copy of the key incase padding is needed
    string newKey = string(key.c_str());

    //Check if the size of the message is a multiple of 16, if not pad until then
    if ((newKey.size() % CryptoPP::AES::BLOCKSIZE) > 0) {
        //Key is not of factor 16

        if (int(newKey.size()) < CryptoPP::AES::BLOCKSIZE) {
            //Insert the number of key.size() - 16 0's to the start
            //of the string
            int numberOf0s = CryptoPP::AES::BLOCKSIZE - int(newKey.size());
            newKey.insert(int(key.size()), numberOf0s, '0');
        } else {
            //Keysize is larger

            //Determine what multiple of 16 is present
            int numberOf16sInKeyLen = int(newKey.size()) / CryptoPP::AES::BLOCKSIZE;

            //Determine the number of 0's needed to complete the next block of 16
            int numberOf0s = CryptoPP::AES::BLOCKSIZE - (int(newKey.size()) - (CryptoPP::AES::BLOCKSIZE * numberOf16sInKeyLen));

            //Insert the number of 0's needed to complete the block of 16 to the front
            newKey.insert((int(key.size())), numberOf0s, '0');
        }
    }

    cout << "newKey: " << newKey << endl;
    cout << "newKey size: " << int(newKey.size()) << endl;
    cout << "key: " << key << endl;
    cout << "key size: " << key.size() << endl;

    using namespace CryptoPP;
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x00, AES::BLOCKSIZE); // Initialize IV to all zeros
    std::string decryptedMessage;
    try
    {
    CBC_Mode<AES>::Decryption decryption;
    decryption.SetKeyWithIV((const byte*)newKey.data(), newKey.size(), iv);
    StringSource(encryptedMessage, true,
    new StreamTransformationFilter(decryption,
    new StringSink(decryptedMessage))
    );
    }
    catch (const Exception& ex)
    {
    // Handle exception
    std::cerr << "DECRYPTION ERROR: " << ex.GetWhat() << endl;

    }

    cout << "=====END DECRYPT=====" << endl;
    return decryptedMessage;
}