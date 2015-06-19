#ifndef DNACODE_H
#define DNACODE_H

#include "Code.h"


class DNACode : public Code
{
    public:
        DNACode();
        virtual ~DNACode();
        virtual void operator>>(ByteCode& other); //local -> Bytecode
        virtual void operator<<(ByteCode& other);
        virtual std::istream& operator<<(std::istream& is);
    protected:
    private:
};

#endif // DNACODE_H
