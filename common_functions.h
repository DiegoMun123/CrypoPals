#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include <vector>
#include <string>


unsigned char HexCharToByte(char c);

std::vector<unsigned char> HexToBytes(const std::string& Hex_String);

int PrintBytesAsHex(std::vector<unsigned char> Bytes);

std::vector<unsigned char> XOR(const std::string& Hex1,const std::string& Hex2);

double get_english_score(const std::vector<unsigned char>& input_bytes);

std::vector<unsigned char> Single_Byte_XOR(const std::string& Ciphertext, unsigned char key);

std::vector<unsigned char> Single_Byte_XOR_Attack(const std::string& Ciphertext_Hex);

std::vector<unsigned char> SingleByteXORDetect(const std::string& filename);

std::vector<unsigned char> Repeated_Key_XOR_Encrypt(const std::string& Plaintext, const std::string& Key);

int HammingDistance(const std::string& String1, const std::string& String2);

int HammingDistance_Bytes(std::vector<unsigned char>& Bytes1, std::vector<unsigned char>& Bytes2);

unsigned char Single_Byte_XOR_Attack_Key(const std::string& Ciphertext_Hex);

std::vector<unsigned char> Read_And_Decode_base64_File(const std::string& filename);

std::vector<unsigned char> base64_Decode(const std::string& Base64_Str);

std::vector<unsigned char> aes_128_ecb_decrypt(const std::vector<unsigned char>& Ciphertext,
    const std::vector<unsigned char>& Key);

void PrintHex(const std::string& s);

std::string PKCS_Padding(std::string Text, int Desired_Len);

std::vector<unsigned char> aes_128_ecb_encrypt(const std::vector<unsigned char>& Plaintext,
    const std::vector<unsigned char>& Key);

#endif
