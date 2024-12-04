#include "encryptionHandler.hpp"
#include <iostream>
#include <aes.h>
#include <modes.h>
#include <filters.h>

// Encrypt and Decrypt code from the following blog post:
// https://yingo.ca/articles/exploring-cryptographic-examples-using-crypto-library
//
//
// The padKey function was written from scratch from the understanding of what
// encrypt and decrypt do in their functions

using std::string; using std::map; using std::cout; using std::endl;

std::string padKey(std::string& key) {
    //Makes an internal copy of the key incase padding is needed
    string newKey = string(key.c_str());

    //cout << key.size() % CryptoPP::AES::BLOCKSIZE << endl;
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

    return newKey;
}


std::string encrypt(const std::string& message, const std::string& key, map<string, string>& input)
{
    //Makes an internal copy of the key incase padding is needed
    string newKey = padKey(input["-K"]);

    //Have this be put into the resulting key
    //input["-K"] = newKey;

    using namespace CryptoPP;
    //Makes the initilization vector
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x00, AES::BLOCKSIZE); // Initialize IV to all zeros

    //Will Hold the encrypted message
    std::string encryptedMessage;
    
    try{
        //Makes an encryption object based on AES in CBC mode
        CBC_Mode<AES>::Encryption encryption;

        //Sets the encryption key (one added with the padding) with the initilization iv
        //Uses a string source with the unencrypted text (message), a String Sink for output
        // after the encryption with a padding scheme set of ZEROS_PADDING
        encryption.SetKeyWithIV((const byte*)newKey.data(), newKey.size(), iv);
            StringSource(message, true,
            new StreamTransformationFilter(encryption,
            new StringSink(encryptedMessage), CryptoPP::BlockPaddingSchemeDef::ZEROS_PADDING
            )
        );
    }
    catch (const Exception& ex)
    {
        //Handles any issues with the encryption
        // Handle exception
        std::cerr << "ENCRYPTION ERROR: " << ex.GetWhat() << endl;
        return "";
    }

    //Returns the successfully encrypted message
    return encryptedMessage;
}


std::string decrypt(const std::string& encryptedMessage, const std::string& key, map<string, string>& input) {
    //cout << "=====DECRYPT=====" << endl;

    //Makes an internal copy of the key incase padding is needed
    string newKey = padKey(input["-K"]);

   

    // cout << "newKey: " << newKey << endl;
    // cout << "newKey size: " << int(newKey.size()) << endl;
    // cout << "key: " << key << endl;
    // cout << "key size: " << key.size() << endl;

    using namespace CryptoPP;

     //Makes the initilization vector
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x00, AES::BLOCKSIZE); // Initialize IV to all zeros

    //Will hold the decrypted message
    std::string decryptedMessage;
    
    try
    {
    //Makes an decryption object set for AES CBC decryption mode
    CBC_Mode<AES>::Decryption decryption;

    //Sets the decrption key (padded if needed) with the initilization vector
    decryption.SetKeyWithIV((const byte*)newKey.data(), newKey.size(), iv);
    //Sets the incoming source of the encrypted source as a string (this is what is passed as an argument)
    // uses a StreamTransformation filter to pull in the string, decrypt the message
    //Then the decrypted message is sent out to a StringSink to the decryptedMessage variable
    //Uses the ZEROS_PADDING block padding scheme to decrypt the message
    StringSource(encryptedMessage, true,
    new StreamTransformationFilter(decryption,
    new StringSink(decryptedMessage), CryptoPP::BlockPaddingSchemeDef::ZEROS_PADDING)
    );
    }
    catch (const Exception& ex)
    {
        //If any issues with the encryption occur, they are dealt with here
        // Handle exception
        std::cerr << "DECRYPTION ERROR: " << ex.GetWhat() << endl;
        return "";

    }

    //cout << "=====END DECRYPT=====" << endl;

    //Returns the successfully decrypted message
    return decryptedMessage;
}
