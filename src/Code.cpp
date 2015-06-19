#include "Code.h"

Code::~Code()
{
    //dtor
}

char Code::remap(char in)
{
    switch(in)
    {
        case 'A':
            return 'T';
        case 'T':
            return 'A';
        case 'G':
            return 'C';
        case 'C':
            return 'G';
    }
    return 'X';
}
