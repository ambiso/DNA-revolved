#include "utils.hpp"


void printHelp()
{
    std::cout << "-g \tGenerate DNA from String" << std::endl;
}

char remap(char in)
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

void gen(std::istream& rd, std::ostream& wr)
{
    const char templates[4][3][12] =
    {
        {"    ","","   "},
        {"   ","--","  "},
        {"  ","----"," "},
        {"  ","-----",""}
    }; //DO NOT CHANGE WITHOUT CHECKING BOUNDS

    char *line = new char[11]; //DO NOT CHANGE WITHOUT CHECKING BOUNDS
    char lastChar = '\0', curChar = '\0';
    if(rd.good())
    {
        rd.read(&lastChar, 1);
    }
    else
    {
        throw 4;
    }

    while(rd.good() && wr.good())
    {
        for(int i = 0; rd.good() && wr.good() && i < 8; i++)
        {
            int u = i < 4 ? i : 8 - i - 1;
            rd.read(&curChar, 1);
            if(lastChar == '/' && curChar == '*') //if comment
            {
                while(rd.good())
                {
                    lastChar = curChar;
                    rd.read(&curChar, 1);
                    if(lastChar == '*' && curChar == '/')
                    {
                        break;
                    }
                }
                i--;
                lastChar = curChar;
            }
            else if(!isalpha(lastChar) || !isalpha(curChar))
            {
                lastChar = curChar;
                i--;
            }
            else
            {
                int len = sprintf(line, "%s%c%s%c%s\n", templates[u][0], lastChar, templates[u][1], curChar, templates[u][2]);
                wr.write(line, len);
                rd.read(&lastChar, 1);
            }
        }
    }
}


std::string openFiles(int argc, char **argv, std::ifstream& rd, std::ofstream& wr)
{
    std::string file = argv[2];
    rd.open(file.c_str());
    if(!rd.is_open() || !rd.good())
    {
        std::cerr << "Could not open input file or file is empty." << std::endl;
        throw 1;
    }
    std::string output = file + ".dna";
    if(argc > 3)
    {
        output = argv[3];
    }

    wr.open(output.c_str(), std::ios::trunc);

    if(!wr.is_open() || !wr.good())
    {
        std::cerr << "Could not open output file or file is empty." << std::endl;
        throw 2;
    }

    return output;
}

bool istoken(char *in)
{
    if(in[2] != '\0')
    {
        throw 3;
    }
    if(in[1] == '\0' && in[2] == '\0')
    {
        switch(in[0])
        {
            case '>':
            case '<':
            case '.':
            case ',':
            case '[':
            case ']':
            case '~':
            case '?':
            case 'X':
                return true;
            default:
                return false;
        }
    }
    switch(in[0])
    {

        case '+':
        case '-':
            if((in[1] == '=' || in[1] == '\0') && in[2] == '\0')
                return true;
            break;
        case '*':
        case '/':
        case ':':
            if(in[1] == '=' && in[2] == '\0')
                return true;
            break;
    }
    return false;
}
