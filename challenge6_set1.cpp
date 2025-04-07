#include <iostream>
#include <string>
#include <vector> 
#include <iomanip>  // For std::setw and std::setfill
#include <limits> //for the score, I dont think i need it, but just in case
#include <fstream> //for the file
#include <bitset> //to count 1 bits
#include "common_functions.h" //my file with the aux function declarations
#include <cstdlib> //for system()
#include <cstdio> //for std::fopen
#include <map> //to get the best keysizes
#include <algorithm> //for sort
#include <tuple> //to get the best keysizes



const std::string base64_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


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

std::string Bytes_To_Hex(const std::vector<unsigned char>& Bytes) {
    std::ostringstream oss;
    for (unsigned char Byte : Bytes) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)Byte;
    }
    return oss.str(); //hex
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




std::vector<int> Find_Top_Keysizes(const std::vector<unsigned char>& Ciphertext) {
    std::vector<std::pair<int, float>> Keysize_Scores; //we will keep the pairs of keysize and scores here
    int Min_Keysize = 2;
    int Max_Keysize = 50;
    int Num_Possible_Keysizes = 5; //a bit bigger than suggested, just in case

    for (int keysize = Min_Keysize; keysize <= Max_Keysize; ++keysize) { //we iterate through all the keysize candidates
        if (Ciphertext.size() < keysize * 4) {
            continue; //we cant divide it
        }

        //we will take the first 4 blocks of size keysize
        std::vector<std::vector<unsigned char>> Blocks;
        for (int i = 0; i < 4; ++i) {
            Blocks.push_back(std::vector<unsigned char>(
                Ciphertext.begin() + i * keysize,
                Ciphertext.begin() + (i + 1) * keysize
            ));
        }
        //we have the blocks, we can compute the hamming distance 
        float Total_Distance = 0.0;
        int Number_Comparisons = 0; //this should always be 6 for 4 blocks

        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = i + 1; j < 4; ++j) {//this way we don't compare the same one twice
                Total_Distance += HammingDistance_Bytes(Blocks[i], Blocks[j]);
                Number_Comparisons++;
            }
        }

        float Avg_Distance = Total_Distance / Number_Comparisons;
        float Normalized_Distance = Avg_Distance / keysize;

        Keysize_Scores.emplace_back(keysize, Normalized_Distance);
    } //end for keysize

    //We have all the scores in Keysize_Scores, we can sort them. The bigger at the end.
    std::sort(Keysize_Scores.begin(), Keysize_Scores.end(), [](auto& a, auto& b) {
        return a.second < b.second;
    });

    // Extract the top N key sizes
    std::vector<int> Best_Keysizes;
    for (size_t i = 0; i < Num_Possible_Keysizes; ++i) {
        Best_Keysizes.push_back(Keysize_Scores[i].first); //the first ones were the smaller
        std::cout << "The Keysize " << Keysize_Scores[i].first << " has a distance: " 
        << Keysize_Scores[i].second << std::endl;
    }

    return Best_Keysizes;
}


std::vector<unsigned char> Repeating_Key_XOR_Attack(const std::vector<unsigned char>& Ciphertext, int Keysize) {
    std::vector<std::vector<unsigned char>> Transposed_Blocks(Keysize);//matrix of blocks


    for (size_t i = 0; i < Ciphertext.size(); ++i) {
        Transposed_Blocks[i % Keysize].push_back(Ciphertext[i]);
    }

    std::vector<unsigned char> Recovered_Key;
    std::vector<unsigned char> Recovered_Plaintext;

    for (int i = 0; i < Keysize; ++i) {
        //XOR attack needs a hex input
        std::ostringstream oss; //
        for (unsigned char byte : Transposed_Blocks[i]) {
            oss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
        }
        std::string Block_Hex = oss.str();

        unsigned char Key_Byte = Single_Byte_XOR_Attack_Key(Block_Hex);
        Recovered_Key.push_back(Key_Byte);
    }

    //now we have the full key, we can decrypt it
    for (size_t i = 0; i < Ciphertext.size(); ++i) {
        unsigned char Plaintext_Byte = (Ciphertext[i] ^ Recovered_Key[i % Keysize] );
        Recovered_Plaintext.push_back(Plaintext_Byte);
    }

    std::cout << "We found the key: \n" << std::endl;

    for (unsigned char c : Recovered_Key) { //pretty printing
        std::cout << c;
    }
    std::cout << std::endl;

    return Recovered_Plaintext; //bytes
}



int main(){

const std::string Hamming_Test1 = "this is a test";
const std::string Hamming_Test2 = "wokka wokka!!!";
const std::string Filename = "challenge6_set1_ciphertext.txt";

    
        try {
            std::vector<unsigned char> Bytes_Ciphertext = Read_And_Decode_base64_File(Filename);
            std::vector<int> Best_Keysizes = Find_Top_Keysizes(Bytes_Ciphertext);
            std::vector<unsigned char> Plaintext = Repeating_Key_XOR_Attack(Bytes_Ciphertext, Best_Keysizes[0]);
            //the key has 29 bytes
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