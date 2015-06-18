#ifndef LINECODE_H
#define LINECODE_H

#include "Code.h"


class LineCode : public Code
{
    public:
        LineCode();
        virtual ~LineCode();
        virtual void operator>>(const ByteCode& other); //translate from Bytecode to local Code type
        virtual std::istream& operator>>(std::istream& is); //read Code from stream
    protected:
    private:
};

#endif // LINECODE_H
