#include <iostream>
#include <string>
#include <vector> 
#include <iomanip>  // For std::setw and std::setfill
#include <limits> //for the score, I dont think i need it, but just in case
#include <fstream> //for the file
#include "common_functions.h" //my file with the aux function declarations




int main(){


    
        try {
            std::vector<unsigned char> Recovered_Plaintext = SingleByteXORDetect("challenge4_set1.txt");
            for (unsigned char c : Recovered_Plaintext) {
                std::cout << c;
            }
            std::cout << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl; //just in case
        }
    
        return 0;  
    }