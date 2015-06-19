#ifndef DNACODE_H
#define DNACODE_H

#include "Code.h"


class DNACode : public Code
{
    public:
        DNACode();
        virtual ~DNACode();
        virtual void operator>>(ByteCode& other); //local -> Bytecode
        virtual std::istream& operator<<(std::istream& is);
        void toggleLegacy();
    protected:
        bool legacy;
    private:
};

#endif // DNACODE_H
