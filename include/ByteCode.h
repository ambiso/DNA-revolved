#ifndef BYTECODE_H
#define BYTECODE_H

#include "Code.h"


class ByteCode : public Code
{
    public:
        ByteCode();
        virtual ~ByteCode();
        virtual void operator>>(ByteCode& other); //translate from Bytecode to local Code type
        virtual std::istream& operator<<(std::istream& is); //read Code from stream
        char* toCharArray();
    protected:
        bool isValidChar(char);
        bool isArithmeticOp(char);
        bool isSingleCharOp(char x);
        bool isTok(char, char);
    private:
};

#endif // BYTECODE_H
