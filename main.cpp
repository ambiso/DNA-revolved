#include <iostream>
#include <string.h>
#include <fstream>
#include "utils.hpp"
#include "Code.h"

int main(int argc, char **argv)
{

    ByteCode bc;
    std::ifstream rd("test");

    DNACode dc;

    dc << rd;
    dc >> bc;
    std::cout << dc << std::endl;
    std::cout << bc << std::endl;


    return 0;
}
