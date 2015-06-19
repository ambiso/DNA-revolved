#include "DNACode.h"

DNACode::DNACode()
{
    //ctor
}

DNACode::~DNACode()
{
    //dtor
}

const int lengths[4][3] =
{
    {4,0,3},
    {3,2,2},
    {2,4,1},
    {2,5,0}
};

Code& DNACode::operator>>(Code &other)
{
    ByteCode localConv;
    this->operator>>(localConv);
    other << localConv;
    return other;
}

Code& DNACode::operator<<(Code &other)
{
    ByteCode localConv;
    other >> localConv;
    this->operator<<(localConv);
    return *this;
}

ByteCode& DNACode::operator>>(ByteCode &other) //local to bytecode -> ByteCode
{
    resetCode(other);
    code.clear();
    code.seekg(0);
    char curChar;
    char curOp[4];
    bool swapOp = false;
    for(int j = 0; code.good(); j++)
    {
        for(int i = 0; i < 4; i++)
        {
            while(code.read(&curChar, 1), (curChar != 'A' && curChar != 'G' && curChar != 'T' && curChar != 'C') && code.good());
            curOp[i] = curChar;
        }
        if(swapOp)
        {
            curOp[0] ^= curOp[1];
            curOp[1] ^= curOp[0];
            curOp[0] ^= curOp[1];

            curOp[2] ^= curOp[3];
            curOp[3] ^= curOp[2];
            curOp[2] ^= curOp[3];
        }
        convertAndWrite(other, curOp);
        if(!legacy && (j%4 == 3))
        {
            swapOp = !swapOp;
        }
    }
    return other;
}


const char templates[4][3][12] =
{
    {"    ","","   "},
    {"   ","--","  "},
    {"  ","----"," "},
    {"  ","-----",""}
}; //DO NOT CHANGE WITHOUT CHECKING BOUNDS

Code& DNACode::operator<<(ByteCode &other)
{
    char op[2];
    char dna[2];
    char line[12];
    int len;
    other.resetReader();
    while(other.good())
    {
        for(int i = 0; i < 8 && ((len = other.readTok(op)) != 0); i++)
        {
            int u = i < 4 ? i : 8 - i - 1;
            convert(op, dna, len);
            sprintf(line, "%s%c%s%c%s\n", templates[u][0], dna[0], templates[u][1], remap(dna[0]), templates[u][2]);
            code << line;

            i++;
            u = i < 4 ? i : 8 - i - 1;

            sprintf(line, "%s%c%s%c%s\n", templates[u][0], dna[1], templates[u][1], remap(dna[1]), templates[u][2]);
            code << line;
        }
    }
    return *this;
}

std::istream& DNACode::operator<<(std::istream &is)
{
    const char comparators[2] = {' ', '-'};

    char lastBase = 0;
    char curChar;
    int marker;
    while(is.good())
    {
        for(int i = 0; i < 8; i++)
        {
            int u = i < 4 ? i : 8 - i - 1; //0, 1, 2, 3, 3, 2, 1, 0
            for(int j = 0; j < 2; j++)
            {
                marker = -1;
                if(!is.good())
                {
                    break;
                }
                do
                {
                    is.read(&curChar, 1);
                    marker++;
                }
                while(curChar == comparators[j] && is.good());
                if(j == 1 && remap(lastBase) != curChar)
                {
                    std::cerr << typeid(*this).name() << ": ERROR: Invalid Base Pairs at " << is.tellg() << " (" << lastBase << " and " << curChar << ")." << std::endl;
                    return is;
                }
                lastBase = curChar;
                if(!is.good())
                {
                    break;
                }
                if(marker != lengths[u][j])
                {
                    std::cerr << typeid(*this).name() << ": ERROR: Incorrect number of '" << comparators[j] << "' at " << is.tellg() << " (expected " << lengths[u][j] << " got " << marker << ")." << std::endl;
                    return is;
                }
                code << templates[u][j];
                code << curChar;
            }
            //skip comments and spaces
            if(!is.good())
            {
                break;
            }
            do
            {
                is.read(&curChar, 1);
            }
            while(curChar != '\n' && is.good());
            code << templates[u][2] << curChar;
        }

    }
    return is;
}
