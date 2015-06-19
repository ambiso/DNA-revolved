#include <iostream>
#include <string.h>
#include <fstream>
#include "utils.hpp"
#include "Code.h"

int main(int argc, char **argv)
{

    ByteCode bc;
    DNACode dc;
    std::ifstream rd("test");
    bc << rd;

    std::cout << bc << std::endl;

    dc << bc;
    dc >> bc;

    std::cout << dc << std::endl;


    return 0;
}
