#ifndef CODE_H
#define CODE_H

#include <iostream>
#include <fstream>
#include <sstream>

class ByteCode;

class Code
{
    public:
        virtual ~Code();
        virtual void operator>>(const ByteCode& other) = 0; //translate from Bytecode to local Code type
        virtual std::istream& operator<<(std::istream& is) = 0; //read Code from stream
        friend std::ostream& operator<<(std::ostream& os, Code& param)
        {
            os << param.code.str();
            return os;
        } //output code to stream
    protected:
        std::stringstream code;
    private:

};

#include "ByteCode.h"

#endif // CODE_H
