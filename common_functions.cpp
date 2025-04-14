#include "common_functions.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <map> //for letter frequency 
#include <cctype> //for letter frequency 
#include <limits> //for the score, I dont think i need it, but just in case
#include <fstream> //for the file
#include <bitset> //to count 1 bits
#include <cstdlib> //for system()
#include <cstdio>  //for std::fopen
#include <openssl/evp.h> //openssl 

unsigned char HexCharToByte(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    if ('a' <= c && c <= 'f') return c - 'a' + 10;
    if ('A' <= c && c <= 'F') return c - 'A' + 10;
    throw std::invalid_argument("Invalid hex character");
}

const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

//read from b64 and return it as bytes
std::vector<unsigned char> base64_Decode(const std::string& Base64_Str) {
        std::vector<unsigned char> Decoded_Data;
        
        int val = 0, valb = -8;
        for (size_t i = 0; i < Base64_Str.size(); i++) {
            char c = Base64_Str[i];

            if (c == '\n' || c == '\r' || c == ' ') //it stopped at each end of line
            continue;
            if (c == '=')
                break; //this is the end
            
            int index = base64_chars.find(c);
            if (index == std::string::npos) {
                throw std::invalid_argument("Invalid Base64 character.");
            }
    
            val = (val << 6) + index;
            valb += 6;
            
            if (valb >= 0) {
                Decoded_Data.push_back(static_cast<unsigned char>((val >> valb) & 0xFF));
                valb -= 8;
            }
        }
        return Decoded_Data; //bytes
}



std::vector<unsigned char> Read_And_Decode_base64_File(const std::string& filename) {
    // Open the file
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error: Unable to open file.");
    }
    std::string base64_str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::vector<unsigned char> Decoded_Bytes = base64_Decode(base64_str);

    //return Bytes_To_Hex(decoded_bytes);    //return as hex
    return Decoded_Bytes; //return as bytes
}


std::vector<unsigned char> HexToBytes(const std::string& Hex_String){
    std::vector<unsigned char> Bytes; 
    if( (Hex_String.length() % 2)  ) throw std::invalid_argument("Cannot convert hex of odd parity to bytes");

    for(size_t i = 0; i < Hex_String.length() ; i +=2){
        unsigned char Byte = ( (HexCharToByte(Hex_String[i]) << 4) | HexCharToByte(Hex_String[i + 1])  );
        Bytes.push_back(Byte);
    }

    return Bytes;
}



int PrintBytesAsHex(std::vector<unsigned char> Bytes){
    for(size_t i = 0; i < Bytes.size(); i++){
        printf("%02x", Bytes[i]);
    }
    printf("\n");
    return 0;
}


void PrintHex(const std::string& s) {
    for (unsigned char c : s) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)c << " ";
    }
    std::cout << std::dec << std::endl;
}

//XOR two hex strings
std::vector<unsigned char> XOR(const std::string& Hex1,const std::string& Hex2){
    //They must have the same len
    if(Hex1.length() != Hex2.length()) throw std::invalid_argument("Cannot xor strings of different len");


    std::vector<unsigned char> Bytes1 = HexToBytes(Hex1); //we use the function from the previous challenge
    std::vector<unsigned char> Bytes2 = HexToBytes(Hex2);
 
    std::vector<unsigned char> Xored_Bytes;
    for(size_t i = 0; i < Bytes1.size(); i ++){
        Xored_Bytes.push_back( Bytes1[i] ^ Bytes2[i] );
    }

    return Xored_Bytes;
}

/*
XOR of a string and a single char (repeated many times)
*/
std::vector<unsigned char> Single_Byte_XOR(const std::string& Ciphertext, unsigned char Key){
    std::vector<unsigned char> result;
    std::vector<unsigned char> Ciphertext_Bytes = HexToBytes(Ciphertext);
    for (unsigned char byte : Ciphertext_Bytes) {
        result.push_back(byte ^ Key);
    }
    return result;
}

/*
Bruteforce encryption with a single byte as key. Challenge3_set1
*/
std::vector<unsigned char> Single_Byte_XOR_Attack(const std::string& Ciphertext_Hex){
    float Best_Score = 0.0; 
    std::vector<unsigned char> Best_Plaintext;
    unsigned char Best_Key; 

    for(size_t i = 0; i < 255; i ++){
        unsigned char Key = static_cast<unsigned char>(i); 
        std::vector<unsigned char> Possible_Plaintext = Single_Byte_XOR(Ciphertext_Hex, Key);
        float Score = get_english_score(Possible_Plaintext);
        if (Score > Best_Score){
            Best_Score = Score; 
            Best_Key = Key; 
            Best_Plaintext = Possible_Plaintext;
            //std::cout << "The score for key: " << (int)Best_Key << "is " << Best_Score << std::endl; //debug
        }
        

    }

    //std::cout << "The best key found is: " << (int)Best_Key << std::endl; //just for debug
    return Best_Plaintext;
}


/*
Modification of the previous one, used for challenge6_set1. It returns the best key, not the plaintext
*/
unsigned char Single_Byte_XOR_Attack_Key(const std::string& Ciphertext_Hex){
    float Best_Score = 0.0; 
    std::vector<unsigned char> Best_Plaintext;
    unsigned char Best_Key; 

    for(size_t i = 0; i < 255; i ++){
        unsigned char Key = static_cast<unsigned char>(i); 
        std::vector<unsigned char> Possible_Plaintext = Single_Byte_XOR(Ciphertext_Hex, Key);
        float Score = get_english_score(Possible_Plaintext);
        if (Score > Best_Score){
            Best_Score = Score; 
            Best_Key = Key; 
            Best_Plaintext = Possible_Plaintext;
            //std::cout << "The score for key: " << (int)Best_Key << "is " << Best_Score << std::endl; //debug
        }
    
    }

    //std::cout << "The best key found is: " << (int)Best_Key << std::endl; //just for debug
    return Best_Key;
}



/*
Function to detect a ciphertext produced on a single byte xor from a file. 
The file must be in the same directory (or input the full path)
*/
std::vector<unsigned char> SingleByteXORDetect(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line; //we will iterate each line
    std::vector<unsigned char> Best_Plaintext;
    float Best_Score = 0.0;
    int Line_Number = 0; //to return in which line the plaintext was
    int Possible_Line_Number = 0; 

    while (std::getline(file, line)) {
        Possible_Line_Number++;
        try {
            std::vector<unsigned char> Possible_Plaintext = Single_Byte_XOR_Attack(line); //Best in the line
            float Score = get_english_score(Possible_Plaintext);
            if (Score > Best_Score) {
                Best_Score = Score;
                Best_Plaintext = Possible_Plaintext;
                Line_Number = Possible_Line_Number;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error processing line: " << e.what() << std::endl;
        }
    }
    std::cout << "The best plaintext was found in line " << Line_Number << std::endl; 
    return Best_Plaintext;
}

/*
Encrypts a plaintext XORing a repeated key. it iterates through the key. Challenge5_set1
*/

std::vector<unsigned char> Repeated_Key_XOR_Encrypt(const std::string& Plaintext, const std::string& Key){
    std::vector<unsigned char> Ciphertext;
    int Key_Len = Key.length();

    for (size_t i = 0; i < Plaintext.length(); ++i) {
        unsigned char Result = Plaintext[i] ^ Key[i % Key_Len]; //the key loops
        Ciphertext.push_back(Result);
    }

    return Ciphertext;
}


/*
Compute the Hamming distance between two strings. challenge6_set1
*/
int HammingDistance(const std::string& String1, const std::string& String2){
    int Distance = 0; 
    if(String1.length() != String2.length()) throw std::invalid_argument("Cannot compute hamming distance if the strings have different len");

    for(size_t i = 0; i < String1.length(); i++){
        unsigned char Result = (String1[i] ^ String2[i] ); //we can operate over strings and store it as byte
        Distance += std::bitset<8>(Result).count();
    }
    return Distance;
}   

/*
Same as the previous one but it takes bytes as an argument
*/
int HammingDistance_Bytes(std::vector<unsigned char>& Bytes1, std::vector<unsigned char>& Bytes2){
    int Distance = 0; 
    if(Bytes1.size() != Bytes2.size()) throw std::invalid_argument("Cannot compute hamming distance if the strings have different len");

    for(size_t i = 0; i < Bytes1.size(); i++){
        unsigned char Result = (Bytes1[i] ^ Bytes2[i] ); //we can operate over strings and store it as byte
        Distance += std::bitset<8>(Result).count();
    }
    return Distance;
}  

/*
OpenSSL functions
*/
//using openssl, if I have time I will update it to my own implementation
std::vector<unsigned char> aes_128_ecb_decrypt(const std::vector<unsigned char>& Ciphertext,
    const std::vector<unsigned char>& Key)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    std::vector<unsigned char> Plaintext(Ciphertext.size() + 128);// + blocksize just in case it has padding
    //it should not use padding, but we can leave it just in case
    int len = 0, Plaintext_Len = 0;

    EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, Key.data(), nullptr);
    EVP_CIPHER_CTX_set_padding(ctx, 0);//no padding, just the ecb

    EVP_DecryptUpdate(ctx, Plaintext.data(), &len, Ciphertext.data(), Ciphertext.size());
    Plaintext_Len = len;

    EVP_DecryptFinal_ex(ctx, Plaintext.data() + len, &len);
    Plaintext_Len += len;

    Plaintext.resize(Plaintext_Len);
    EVP_CIPHER_CTX_free(ctx);
    return Plaintext;
}



std::vector<unsigned char> aes_128_ecb_encrypt(const std::vector<unsigned char>& Plaintext,
    const std::vector<unsigned char>& Key)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    std::vector<unsigned char> Ciphertext(Plaintext.size() + 128);// + blocksize just in case it has padding
    //it should not use padding, but we can leave it just in case
    int len = 0, Ciphertext_Len = 0;

    EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, Key.data(), nullptr);
    EVP_CIPHER_CTX_set_padding(ctx, 0);//no padding, just the ecb

    EVP_EncryptUpdate(ctx, Ciphertext.data(), &len, Plaintext.data(), Plaintext.size());
    Ciphertext_Len = len;

    EVP_EncryptFinal_ex(ctx, Ciphertext.data() + len, &len);
    Ciphertext_Len += len;

    Ciphertext.resize(Ciphertext_Len);
    EVP_CIPHER_CTX_free(ctx);
    return Ciphertext;
}

/*
PKCS padding for aes-cbc encryption
*/
std::string PKCS_Padding(std::string Text, int Desired_Len){
    if(Text.length() > Desired_Len) throw std::invalid_argument("Desired len cannot be bigger than Text len");
    if(Text.length() == Desired_Len){
        return Text;
    }

    int Remaining = Desired_Len - Text.length();
    char Remaining_Hex = static_cast<char>(Remaining);

    for(size_t i = 0; i < Remaining; i++){
        Text = Text + Remaining_Hex;
    }
    return Text;
}








/*
Function to score a string based on how close it is to English (decrypted)
*/
const std::map<char, double> CHARACTER_FREQ = {
    {'a', 0.0651738}, {'b', 0.0124248}, {'c', 0.0217339}, {'d', 0.0349835},
    {'e', 0.1041442}, {'f', 0.0197881}, {'g', 0.0158610}, {'h', 0.0492888},
    {'i', 0.0558094}, {'j', 0.0009033}, {'k', 0.0050529}, {'l', 0.0331490},
    {'m', 0.0202124}, {'n', 0.0564513}, {'o', 0.0596302}, {'p', 0.0137645},
    {'q', 0.0008606}, {'r', 0.0497563}, {'s', 0.0515760}, {'t', 0.0729357},
    {'u', 0.0225134}, {'v', 0.0082903}, {'w', 0.0171272}, {'x', 0.0013692},
    {'y', 0.0145984}, {'z', 0.0007836}, {' ', 0.1918182} // Space character as well
};

//WARNING: It takes bytes as input
double get_english_score(const std::vector<unsigned char>& input_bytes) {
    double score = 0.0;

    for (unsigned char byte : input_bytes) {
        char ch = static_cast<char>(byte); 
        ch = std::tolower(ch); //we only have lowercases
        if (CHARACTER_FREQ.find(ch) != CHARACTER_FREQ.end()) {
            score += CHARACTER_FREQ.at(ch);
        } else {
            score += 0.0; //if no defined frecuency (this should not happen, I keep it just in case)
        }
    }

    return score; //bigger score means more likely to be English, thus the original plaintext
}