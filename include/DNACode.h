#ifndef DNACODE_H
#define DNACODE_H

#include "Code.h"


class DNACode : public Code
{
    public:
        DNACode();
        virtual ~DNACode();
        virtual ByteCode& operator>>(ByteCode &other); //local to bytecode -> ByteCode
        virtual Code& operator<<(ByteCode &other); //bytecode to local -> local
        virtual Code& operator>>(Code &other); //local -> code -> code
        virtual Code& operator<<(Code &other); //code -> local -> local
        virtual std::istream& operator<<(std::istream &is);
    protected:
    private:
};

#endif // DNACODE_H
