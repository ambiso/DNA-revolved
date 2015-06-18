#include "ByteCode.h"

ByteCode::ByteCode()
{
    //ctor
}

ByteCode::~ByteCode()
{
    //dtor
}

void ByteCode::operator>>(const ByteCode& other)
{
    code.str(other.code.str());
}

std::istream& ByteCode::operator<<(std::istream& is)
{
    code.str(std::string());
    char lastChar, curChar = 0;
    while(is.good() && !is.eof())
    {
        do
        {
            lastChar = curChar;
            is.read(&curChar, 1);
        }
        while(!isValidChar(curChar) && is.good() && !is.eof());

        if((isspace(lastChar) || lastChar == '/') && curChar == '/')
        {
            //skip comment
            do
            {
                lastChar = curChar;
                is.read(&curChar, 1);
            }
            while(is.good() && !is.eof() && (lastChar != '*' || curChar != '/'));
        }
        else if(isTok(lastChar, curChar))
        {
            code << curChar;
        }
        else if(is.good() && !is.eof())
        {
            std::cerr << "ERROR: Invalid Token at " << is.tellg() << ": '" << lastChar << curChar << "'" << std::endl;
        }
    }
    return is;
}

bool ByteCode::isArithmeticOp(char x)
{
    switch(x)
    {
        case '+':
        case '-':
        case '*':
        case '/':
            return true;
        default:
            return false;
    }
}

bool ByteCode::isValidChar(char x)
{
    if(isSingleCharOp(x))
    {
        return true;
    }
    switch(x)
    {
        case '=':
        case ':':
        case '+':
        case '-':
        case '*':
        case '/':
            return true;
        default:
            return false;
    }
}

bool ByteCode::isSingleCharOp(char x)
{
    switch(x)
    {
        case '<':
        case '>':
        case '+':
        case '-':
        case '.':
        case ',':
        case '[':
        case ']':
        case '~':
        case '?':
        case 'X':
            return true;
        default:
            return false;
    }
}

bool ByteCode::isTok(char last, char cur)
{
    return isSingleCharOp(cur) || isSingleCharOp(last) || (isArithmeticOp(last) && cur == '=');
}

char* ByteCode::toCharArray()
{
    size_t len = code.gcount();
    char *arrCopy = new char[len];
    code.seekg(0);
    for(size_t i = 0; i < len && !code.eof(); i++)
    {
        code.read(arrCopy+i, 1);
    }
    return arrCopy;
}
