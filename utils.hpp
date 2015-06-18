#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>

void printHelp();
char remap(char);
void gen(std::istream& rd, std::ostream& wr);
std::string openFiles(int argc, char **argv, std::ifstream& rd, std::ofstream& wr);
bool istoken(char *in);

#endif // UTILS_HPP_INCLUDED
