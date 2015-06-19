#ifndef BYTECODE_H
#define BYTECODE_H

#include "Code.h"


class ByteCode : public Code
{
    public:
        ByteCode();
        virtual ~ByteCode();
        virtual void operator>>(ByteCode& other); //
        virtual void operator<<(ByteCode& other);
        virtual std::istream& operator<<(std::istream& is); //read Code from stream
        char* toCharArray();
        void resetReader()
        {
            code.clear();
            code.seekg(0, std::ios::beg);
        }
        bool good()
        {
            return code.good();
        }
        int readTok(char* out)
        {
            out[0] = '\0'; out[1] = '\0';
            if(code.good())
            {
                code.read(out, 1);
            }
            if(isSingleCharOp(*out))
            {
                return 1;
            }
            if(code.good())
            {
                code.read(out+1, 1);
            }
            int len;
            isTok(*out, *(out+1), len);
            return len;
        }
    protected:
        bool isValidChar(char);
        bool isArithmeticOp(char);
        bool isSingleCharOp(char x);
        bool isTok(char, char, int& len);
    private:
};

#endif // BYTECODE_H
