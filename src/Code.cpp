#include "Code.h"

bool Code::legacy = true;

Code::~Code()
{
    //dtor
}

char Code::remap(char in)
{
    switch(in)
    {
        case 'A':
            return 'T';
        case 'T':
            return 'A';
        case 'G':
            return 'C';
        case 'C':
            return 'G';
    }
    return 'X';
}

void Code::convertAndWrite(Code &other, char *in)
{
    switch(in[0])
    {
        case 'A':
            switch(in[2])
            {
                case 'A':
                    writeCode(other, ">", 1);
                    break;
                case 'G':
                    writeCode(other, "<", 1);
                    break;
                case 'T':
                    writeCode(other, "+", 1);
                    break;
                case 'C':
                    writeCode(other, "-", 1);
                    break;
            }
            break;
        case 'G':
            switch(in[2])
            {
                case 'A':
                    writeCode(other, ".", 1);
                    break;
                case 'G':
                    writeCode(other, ",", 1);
                    break;
                case 'T':
                    writeCode(other, "[", 1);
                    break;
                case 'C':
                    writeCode(other, "]", 1);
                    break;
            }
            break;
        case 'T':
            switch(in[2])
            {
                case 'A':
                    if(legacy)
                    {
                        writeCode(other, "X=", 2);
                    }
                    else
                    {
                        writeCode(other, ":=", 2);
                    }
                    break;
                case 'G':
                    writeCode(other, "+=", 2);
                    break;
                case 'T':
                    writeCode(other, "-=", 2);
                    break;
                case 'C':
                    writeCode(other, "*=", 2);
                    break;
            }
            break;
        case 'C':
            switch(in[2])
            {
                case 'A':
                    writeCode(other, "/=", 2);
                    break;
                case 'G':
                    writeCode(other, "~", 1);
                    break;
                case 'T':
                    writeCode(other, "?", 1);
                    break;
                case 'C':
                    writeCode(other, "X", 1);
                    break;
            }
            break;
    }
}

void Code::convert(char *op, char *dna, std::streamsize len)
{
    switch(len)
    {
        case 1:
            switch(op[0])
            {
                case '>':
                    dna[0] = 'A';
                    dna[1] = 'A';
                    break;
                case '<':
                    dna[0] = 'A';
                    dna[1] = 'G';
                    break;
                case '+':
                    dna[0] = 'A';
                    dna[1] = 'T';
                    break;
                case '-':
                    dna[0] = 'A';
                    dna[1] = 'C';
                    break;
                case '.':
                    dna[0] = 'G';
                    dna[1] = 'A';
                    break;
                case ',':
                    dna[0] = 'G';
                    dna[1] = 'G';
                    break;
                case '[':
                    dna[0] = 'G';
                    dna[1] = 'T';
                    break;
                case ']':
                    dna[0] = 'G';
                    dna[1] = 'C';
                    break;
                case '~':
                    dna[0] = 'C';
                    dna[1] = 'G';
                    break;
                case '?':
                    dna[0] = 'C';
                    dna[1] = 'T';
                    break;
                case 'X':
                    dna[0] = 'C';
                    dna[1] = 'C';
                    break;
            }
            if(legacy && op[0] == '=')
            {
                dna[0] = 'T';
                dna[1] = 'A';
            }
            break;
        case 2:
            if(op[1] != '=')
            {
                std::cerr << "Expected '=' but got " << op[1] << " while translating ByteCode to DNA." << std::endl;
                return;
            }
            switch(op[0])
            {
                case ':':
                    dna[0] = 'T';
                    dna[1] = 'A';
                    break;
                case '+':
                    dna[0] = 'T';
                    dna[1] = 'G';
                    break;
                case '-':
                    dna[0] = 'T';
                    dna[1] = 'T';
                    break;
                case '*':
                    dna[0] = 'T';
                    dna[1] = 'C';
                    break;
                case '/':
                    dna[0] = 'C';
                    dna[1] = 'A';
                    break;
                default:
                    std::cerr << "Expected +,-,*,/,: but got '" << op[0] << "' while translating ByteCode to DNA." << std::endl;
                    return;
            }
            break;
    }
}
