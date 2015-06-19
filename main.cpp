#include <iostream>
#include <string.h>
#include <fstream>
#include <map>
#include "include/Code.h"
#include "utils.hpp"

int main(int argc, char **argv)
{
    if(argc > 1)
    {
        std::cout << "Warning: No arguments supports for now." << std::endl;
    }
    ByteCode Bc;
    DNACode dc;
    LineCode lc;
    std::map<std::string, Code*> cmdMap;
    cmdMap["ByteCode"] = &Bc; cmdMap["byte"] = &Bc;
    cmdMap["DNACode"] = &dc; cmdMap["dna"] = &dc;
    cmdMap["LineCode"] = &lc; cmdMap["line"] = &lc;

    std::ifstream rd;

    std::string line, cmd;
    bool cont = true;
    while(cont)
    {
        std::cout << "$>";
        std::getline(std::cin, line);
        std::stringstream opt(line);

        opt >> cmd;

        if(cmd == "load")
        {
            //load format file
            if(!opt.good())
            {
                errCmd("Expected <file> and <format>");
                continue;
            }
            opt >> cmd;
            rd.open(cmd.c_str());
            if(!rd.good())
            {
                errCmd("Could not open input file \"" + cmd + "\".");
                continue;
            }
            if(!opt.good())
            {
                errCmd("Expected <file> and <format>");
                continue;
            }
            opt >> cmd;
            Code *format = cmdMap[cmd];
            if(format == 0)
            {
                errCmd("Could not find mode \"" + cmd + "\".");
                continue;
            }
            format->clear();
            *format << rd;
            rd.close();
        }
        else if(cmd == "conv")
        {
            //conv format1 format2
            if(!opt.good())
            {
                errCmd("Expected <format1> and <format2>");
                continue;
            }
            opt >> cmd;
            Code *format1 = cmdMap[cmd];
            if(format1 == 0)
            {
                errCmd("Could not find mode \"" + cmd + "\".");
                continue;
            }
            if(!opt.good())
            {
                errCmd("Expected <format1> and <format2>");
                continue;
            }
            opt >> cmd;
            Code *format2 = cmdMap[cmd];
            if(format2 == 0)
            {
                errCmd("Could not find mode \"" + cmd + "\".");
                continue;
            }

            format2->clear();
            *format1 >> *format2;
        }
        else if(cmd == "print")
        {
            //print format
            if(!opt.good())
            {
                errCmd("Expected <format>");
                continue;
            }
            opt >> cmd;
            Code *format = cmdMap[cmd];
            if(format == 0)
            {
                errCmd("Could not find mode \"" + cmd + "\".");
                continue;
            }
            std::cout << *format << std::endl;
        }
        else if(cmd == "save")
        {
            if(!opt.good())
            {
                errCmd("Expected <file> <format>");
                continue;
            }
            opt >> cmd;
            Code *format = cmdMap[cmd];
            if(format == 0)
            {
                errCmd("Could not find mode \"" + cmd + "\".");
                continue;
            }
            if(!opt.good())
            {
                errCmd("Expected <input file> and <format>");
                continue;
            }
            opt >> cmd;
            std::ofstream wr(cmd.c_str(), std::ios::trunc);
            if(!wr.good())
            {
                errCmd("Could not open input file \"" + cmd + "\".");
                continue;
            }
            wr << *format;
            wr.close();
        }
        else if(cmd == "help")
        {
            printHelp();
        }
        else if(cmd == "legacy")
        {
            Code::toggleLegacy();
            std::cout << "Legacy mode turned " << (Code::getLegacy() ? "on" : "off") << "." << std::endl;
        }
        else
        {
            errCmd("Invalid command.");
            printHelp();
        }
    }


    return 0;
}
