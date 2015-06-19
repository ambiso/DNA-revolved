#ifndef LINECODE_H
#define LINECODE_H

#include "Code.h"


class LineCode : public Code
{
    public:
        LineCode();
        virtual ~LineCode();
        virtual ByteCode& operator>>(ByteCode &other); //local to bytecode -> ByteCode
        virtual Code& operator<<(ByteCode &other); //bytecode to local -> local
        virtual Code& operator>>(Code &other); //local -> code -> code
        virtual Code& operator<<(Code &other); //code -> local -> local
        virtual std::istream& operator<<(std::istream& is);
    protected:
        bool isValid(char in) { return in == 'A' || in == 'G' || in == 'T' || in == 'C'; }
        bool isValidSeq(char last, char cur) { return (last == '\0' || last == remap(cur)) && isValid(cur); }
    private:
};

#endif // LINECODE_H
