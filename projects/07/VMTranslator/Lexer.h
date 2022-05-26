/*
 *  Copyright 2022, Hutton Grabiel
 */

#pragma once

#include <vector>
#include <string>
#include <iostream>

class Lexer {
public:
    static std::vector<std::string> lex(std::string);
private:
    static bool is_new_line(char); 
};