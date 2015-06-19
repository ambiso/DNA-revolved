#include "DNACode.h"

DNACode::DNACode()
{
    //ctor
    legacy = true;
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

void DNACode::toggleLegacy()
{
    legacy = !legacy;
}

void DNACode::operator>>(ByteCode& other) //local >> Bytecode (local -> byte)
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
        if(legacy && (j%8 == 7))
        {
            swapOp = !swapOp;
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
        switch(curOp[0])
        {
            case 'A':
                switch(curOp[2])
                {
                    case 'A':
                        writeCode(other, ">", 1);
                        break;
                    case 'G':
                        writeCode(other, "<", 1);
                        break;
                    case 'T':
                        writeCode(other, "+", 1);
                        break;
                    case 'C':
                        writeCode(other, "-", 1);
                        break;
                }
                break;
            case 'G':
                switch(curOp[2])
                {
                    case 'A':
                        writeCode(other, ".", 1);
                        break;
                    case 'G':
                        writeCode(other, ",", 1);
                        break;
                    case 'T':
                        writeCode(other, "[", 1);
                        break;
                    case 'C':
                        writeCode(other, "]", 1);
                        break;
                }
                break;
            case 'T':
                switch(curOp[2])
                {
                    case 'A':
                        if(legacy)
                        {
                            writeCode(other, "X=", 2);
                        }
                        else
                        {
                            writeCode(other, ":=", 2);
                        }
                        break;
                    case 'G':
                        writeCode(other, "+=", 2);
                        break;
                    case 'T':
                        writeCode(other, "-=", 2);
                        break;
                    case 'C':
                        writeCode(other, "*=", 2);
                        break;
                }
                break;
            case 'C':
                switch(curOp[2])
                {
                    case 'A':
                        writeCode(other, "/=", 2);
                        break;
                    case 'G':
                        writeCode(other, "~", 1);
                        break;
                    case 'T':
                        writeCode(other, "?", 1);
                        break;
                    case 'C':
                        writeCode(other, "X", 1);
                        break;
                }
                break;
        }
    }
}

const char templates[4][3][12] =
{
    {"    ","","   "},
    {"   ","--","  "},
    {"  ","----"," "},
    {"  ","-----",""}
}; //DO NOT CHANGE WITHOUT CHECKING BOUNDS

std::istream& DNACode::operator<<(std::istream& is)
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
                    std::cerr << "Invalid Base Pairs at " << is.tellg() << " (" << lastBase << " and " << curChar << ")." << std::endl;
                }
                lastBase = curChar;
                if(!is.good())
                {
                    break;
                }
                if(marker != lengths[u][j])
                {
                    std::cerr << "ERROR: Incorrect number of '" << comparators[j] << "' at " << is.tellg() << " (expected " << lengths[u][j] << " got " << marker << ")." << std::endl;
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
