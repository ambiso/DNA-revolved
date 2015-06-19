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
        virtual void operator>>(ByteCode& other) = 0; //translate from Bytecode to local Code type
        virtual std::istream& operator<<(std::istream& is) = 0; //read Code from stream
        friend std::ostream& operator<<(std::ostream& os, const Code& param)
        {
            os << param.code.str();
            return os;
        } //output code to stream
    protected:
        std::stringstream code;
        char remap(char in);
        friend void resetCode(Code& x) { x.code.str(std::string()); }
        friend void writeCode(Code& x, const char *in, std::streamsize len) { x.code.write(in, len); }
        friend void readCode(Code& x, char *out, std::streamsize len) { x.code.read(out, len); }
    private:

};

#include "ByteCode.h"
#include "DNACode.h"

#endif // CODE_H
