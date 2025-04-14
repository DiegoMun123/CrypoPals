#include <iostream>
#include <string>
#include <vector> 
#include <stdexcept>
#include "common_functions.h" //my file with the aux function declarations


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






int main(){

    std::string No_Padding = "YELLOW SUBMARINE";
    
        try {
            int Desired_Len = 20; //in bytes
            std::string PKCS_Padded = PKCS_Padding(No_Padding, Desired_Len);
            PrintHex(PKCS_Padded); //it doesnt show the 04 with just std::cout

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl; //just in case
        }
    
        return 0;  
}