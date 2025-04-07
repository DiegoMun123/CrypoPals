#include <iostream>
#include <string>
#include <vector> 
#include <iomanip>  // For std::setw and std::setfill
#include "common_functions.h" //my file with the aux function declarations





int main(){

    const std::string Ciphertext = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

    
    
        try {
            std::vector<unsigned char> Recovered_Plaintext = Single_Byte_XOR_Attack(Ciphertext);

            for (unsigned char c : Recovered_Plaintext) {
                std::cout << c;
            }
            std::cout << std::endl;
    
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl; //just in case
        }
    
        return 0;  
    }