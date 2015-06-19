#include "LineCode.h"

LineCode::LineCode()
{
    //ctor
}

LineCode::~LineCode()
{
    //dtor
}

ByteCode& LineCode::operator>>(ByteCode &other)
{
    code.clear();
    code.seekg(0, std::ios::beg);
    char curOp[4];
    resetCode(other);
    for(int i = 0; code.good(); i++)
    {
        code.read(curOp, 4);
        if(code.good())
        {
            curOp[1] = curOp[2];
            convertAndWrite(other, curOp);
        }
    }
    return other;
}

Code& LineCode::operator<<(ByteCode &other)
{
    other.resetReader();
    char op[2];
    char dna[2];
    while(other.good())
    {
        int len = other.readTok(op);
        if(len > 0)
        {
            convert(op, dna, len);
            code << dna[0] << remap(dna[0]) << dna[1] << remap(dna[1]);
        }
    }
    return *this;
}

Code& LineCode::operator>>(Code &other) //local -> Code (Code)
{
    ByteCode localConv;
    this->operator>>(localConv);
    other << localConv;
    return other;
}

Code& LineCode::operator<<(Code &other)
{
    ByteCode localConv;
    other >> localConv;
    this->operator<<(localConv);
    return *this;
}

std::istream& LineCode::operator<<(std::istream &is)
{
    char lastChar, curChar = '\0';

    for(int i = 0; is.good(); i++, i%=2)
    {
        do
        {
            lastChar = curChar;
            is.read(&curChar, 1);
        }
        while(isspace(curChar) && is.good());

        if(curChar == '/')
        {
            if(is.good())
            {
                lastChar = curChar;
                is.read(&curChar, 1);
            }
            if(curChar == '*')
            {
                do
                {
                    lastChar = curChar;
                    is.read(&curChar, 1);
                }
                while(is.good() && (lastChar != '*' && curChar != '/'));
                if(is.good())
                {
                    lastChar = curChar;
                    is.read(&curChar, 1);
                }
                i--;
                continue;
            }
        }
        if(i == 1 && !isValidSeq(lastChar, curChar))
        {
            if(remap(lastChar) == 'X')
            {
                std::cerr << typeid(*this).name() << ": ERROR: \"" << lastChar << curChar << "\" is not a valid character sequence." << std::endl;
            }
            else
            {
                std::cerr << typeid(*this).name() << ": ERROR: Expected " << remap(lastChar) << " but got " << curChar << " while parsing LineCode at " << is.tellg() << "." << std::endl;
            }
            return is;
        }

        if(isValid(curChar))
        {
            code.write(&curChar, 1);
        }
    }
    return is;
}
