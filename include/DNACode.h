#ifndef DNACODE_H
#define DNACODE_H

#include "Code.h"


class DNACode : public Code
{
    public:
        DNACode();
        virtual ~DNACode();
        virtual void operator>>(const ByteCode& other); //translate from Bytecode to local Code type
        virtual std::istream& operator>>(std::istream& is); //read Code from stream
    protected:
    private:
};

#endif // DNACODE_H
