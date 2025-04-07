#include <iostream>
#include <string>
#include <vector> 
#include <iomanip>  // For std::setw and std::setfill
#include "common_functions.h" //my file with the aux function declarations



int main(){

    std::string Hex_String1 = "1c0111001f010100061a024b53535009181c";
    std::string Hex_String2 = "686974207468652062756c6c277320657965";
    
    
        try {
            std::vector<unsigned char> Xored_Bytes = XOR(Hex_String1, Hex_String2);
            PrintBytesAsHex(Xored_Bytes);
    
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl; //just in case
        }
    
        return 0;  
    }