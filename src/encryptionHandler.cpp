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
        cout << "Key is not of factor 16" << endl;

        if (int(newKey.size()) < CryptoPP::AES::BLOCKSIZE) {
            //Insert the number of key.size() - 16 0's to the start
            //of the string
            cout << "Key size is smaller than 16" << endl;
            int numberOf0s = CryptoPP::AES::BLOCKSIZE - int(newKey.size());
            newKey.insert(0, numberOf0s, '0');
            cout << "newKey:" <<  newKey << endl;
        } else {
            //Keysize is larger
            cout << "Key size is larger than 16" << endl;
            cout << "Key size is " << newKey.size() << endl;

            //Determine what multiple of 16 is present
            int numberOf16sInKeyLen = int(newKey.size()) / CryptoPP::AES::BLOCKSIZE;
            cout << "Number of 16's in the key: " << numberOf16sInKeyLen << endl;

            //Determine the number of 0's needed to complete the next block of 16
            int numberOf0s = CryptoPP::AES::BLOCKSIZE - (int(newKey.size() - (CryptoPP::AES::BLOCKSIZE * numberOf16sInKeyLen)));
            cout << "Number of 0's needed: " << numberOf0s << endl;

            //Insert the number of 0's needed to complete the block of 16 to the front
            newKey.insert(0, numberOf0s, '0');

            cout << "Keys NEW size is " << newKey.size() << endl;
        }

        //Have this be put into the resulting key
        input["-K"] = newKey;
    }

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
        std::cerr << "ERROR" << endl;
    }
    return encryptedMessage;
}


std::string decrypt(const std::string& encryptedMessage, const std::string& key) {
    using namespace CryptoPP;
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x00, AES::BLOCKSIZE); // Initialize IV to all zeros
    std::string decryptedMessage;
    try{
        CBC_Mode<AES>::Decryption decryption;
        decryption.SetKeyWithIV((const byte*)key.data(), key.size(), iv);
        StringSource(encryptedMessage, true,
        new StreamTransformationFilter(decryption,
        new StringSink(decryptedMessage)
        )
        );
    }catch (const Exception& ex){
        // Handle exception
    }
    return decryptedMessage;
}