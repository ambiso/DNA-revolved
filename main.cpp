#include <iostream>
#include <string.h>
#include <fstream>
#include "utils.hpp"
#include "Code.h"

int main(int argc, char **argv)
{

    ByteCode bc;
    std::ifstream rd("test");
    bc << rd;
    std::ofstream wr("out.txt");
    wr << bc;

    return 0;
}
