/*
 *  Copyright 2022, Hutton Grabiel
 */

#pragma once

#include <vector>
#include <string>
#include <iostream>

class Lexer {
public:
    enum class PathType {
        File,
        Directory
    };

    static std::vector<std::string> lex(std::string);
    static bool is_lexable_line(std::string);
    static std::string trim_path(std::string, PathType);
private:
    static bool is_new_line(char); 
};