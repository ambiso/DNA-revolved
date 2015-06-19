#ifndef CODE_H
#define CODE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>

class ByteCode;

class Code
{
    public:
        virtual ~Code();
        virtual ByteCode& operator>>(ByteCode &other) = 0; //local to bytecode -> ByteCode
        virtual Code& operator<<(ByteCode &other) = 0; //bytecode to local -> local
        virtual Code& operator>>(Code &other) = 0; //local -> code -> code
        virtual Code& operator<<(Code &other) = 0; //code -> local -> local
        virtual std::istream& operator<<(std::istream &is) = 0; //read Code from stream
        friend std::ostream& operator<<(std::ostream &os, const Code &param)
        {
            os << param.code.str();
            return os;
        } //output code to stream
        static void toggleLegacy() { legacy = !legacy; }
        static bool getLegacy() { return legacy; }
        void clear() { code.clear(); code.seekg(0); }
    protected:
        std::stringstream code;
        char remap(char in);
        friend void resetCode(Code &x) { x.code.clear(); x.code.str(std::string()); }
        friend void writeCode(Code &x, const char *in, std::streamsize len) { x.code.write(in, len); }
        friend void readCode(Code &x, char *out, std::streamsize len) { x.code.read(out, len); }
        static bool legacy;
        void convertAndWrite(Code &other, char *in);
        void convert(char *, char *, std::streamsize);
    private:

};

#include "ByteCode.h"
#include "DNACode.h"
#include "LineCode.h"



#endif // CODE_H
