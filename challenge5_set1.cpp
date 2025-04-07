#include <iostream>
#include <string>
#include <vector> 
#include <iomanip>  // For std::setw and std::setfill
#include <limits> //for the score, I dont think i need it, but just in case
#include <fstream> //for the file
#include "common_functions.h" //my file with the aux function declarations




int main(){

    const std::string Plaintext = "Burning 'em, if you ain't quick and nimble I go crazy when I hear a cymbal";
    const std::string Key = "ICE";

    
        try {
            std::vector<unsigned char> Encrypted = Repeated_Key_XOR_Encrypt(Plaintext, Key);
            PrintBytesAsHex(Encrypted); //pretty printing



        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl; //just in case
        }
    
        return 0;  
    }