#include <openssl/evp.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "common_functions.h" //from the previous challenges

std::vector<unsigned char> aes_128_ecb_decrypt(const std::vector<unsigned char>& Ciphertext,
    const std::vector<unsigned char>& Key)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    std::vector<unsigned char> plaintext(ciphertext.size() + AES_BLOCK_SIZE);
    int len = 0, plaintext_len = 0;

    EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, key.data(), nullptr);
    EVP_CIPHER_CTX_set_padding(ctx, 0);//no padding, just the ecb

    EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size());
    plaintext_len = len;

    EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len);
    plaintext_len += len;

    plaintext.resize(plaintext_len);
    EVP_CIPHER_CTX_free(ctx);
    return plaintext;
}



int main(){

    const string Key = "YELLOW SUBMARINE";
    
        try {


            std::cout << "\n We recovered the plaintext: \n" << std::endl;
            for (unsigned char c : Plaintext) { //pretty printing
                std::cout << c;
            }
            std::cout << std::endl;


        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl; //just in case
        }
    
        return 0;  
    }