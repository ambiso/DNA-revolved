#include "DNACode.h"

DNACode::DNACode()
{
    //ctor
}

DNACode::~DNACode()
{
    //dtor
}

void DNACode::operator>>(const ByteCode& other)
{

}

std::istream& DNACode::operator>>(std::istream& is)
{
    const char templates[4][3][12] =
    {
        {"    ","","   "},
        {"   ","--","  "},
        {"  ","----"," "},
        {"  ","-----",""}
    }; //DO NOT CHANGE WITHOUT CHECKING BOUNDS

}
