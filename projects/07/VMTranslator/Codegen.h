/*
 *  Copyright 2022, Hutton Grabiel
 */

#pragma once

#include <iostream>
#include <fstream>
#include <Parser.h>
#include <string>

class Codegen {
public:
    static std::string generate_hack_asm(std::vector<Parser::CommandType> const&, std::vector<std::string> const&, std::string const&, std::string const&);
private:
    static std::string label_name(Parser::CommandType const&, std::string const&, std::vector<std::string> const&);
    static int to_int(std::string str) 
    {
        int res = 0;
        int mult = 1;
        for (int i = str.size()-1; i >= 0; i--) {
            res += mult * (str[i] - '0');
            mult *= 10;
        }
        return res;
    } 
};