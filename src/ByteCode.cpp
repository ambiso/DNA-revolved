#include "ByteCode.h"

ByteCode::ByteCode()
{
    //ctor
    legacy = true;
}

ByteCode::~ByteCode()
{
    //dtor
}

void ByteCode::operator>>(ByteCode& other)
{
    std::cout << "ByteCode >> called with " << other << std::endl;
    other.code.str(code.str());
}

void ByteCode::operator<<(ByteCode& other)
{

}

std::istream& ByteCode::operator<<(std::istream& is)
{
    code.str(std::string());
    char lastChar, curChar = 0;
    int len;
    bool lastTokErr = false;
    while(is.good())
    {
        do
        {
            lastChar = curChar;
            is.read(&curChar, 1);
        }
        while(!isValidChar(curChar) && is.good());

        if((isspace(lastChar) || lastChar == '/') && curChar == '/')
        {
            //skip comment
            do
            {
                lastChar = curChar;
                is.read(&curChar, 1);
            }
            while(is.good() && (lastChar != '*' || curChar != '/'));
        }
        else if(isTok(lastChar, curChar, len))
        {
            if(len == 1)
            {
                code << curChar;
                lastTokErr = false;
            }
            else if(len == 2)
            {
                code << lastChar << curChar;
                lastTokErr = false;
            }

        }
        else if(is.good())
        {
            if(lastTokErr)
            {
                std::cerr << "ERROR: Invalid Token at " << is.tellg() << ": '" << lastChar << curChar << "'" << std::endl;
            }
            lastTokErr = true;
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
        case '/': //also for comments
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
    }
    return legacy && x == '=';
}

bool ByteCode::isTok(char last, char cur, int& len)
{
    return ((len = 1), isSingleCharOp(cur)) || ((len = 2), (((!legacy && last == ':') || isArithmeticOp(last)) && cur == '=')) || ((len = 0), false);
}

char* ByteCode::toCharArray() //YOU MUST MANAGE THE RETURNED ARRAY YOURSELF
{
    std::streamsize len = code.gcount();
    char *arrCopy = new char[len];
    code.seekg(0, std::ios::beg);
    for(std::streamsize i = 0; i < len && !code.good(); i++)
    {
        code.read(arrCopy+i, 1);
    }
    return arrCopy;
}
