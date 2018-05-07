/*
    Temporary proof-of-concept demonstrator.
*/

#include "posit.h"
#include <iostream>

int main() {
    std::cout 
        << "Hello Tom!" 
        << std::endl;
    std::cout 
        << "Netcode platform: " 
        << posit::poc()
        << std::endl;
    return 0;
}
