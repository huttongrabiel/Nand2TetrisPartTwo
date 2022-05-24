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
};