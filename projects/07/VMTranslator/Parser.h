/*
 *  Copyright 2022, Hutton Grabiel
 */

#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <map>

class Parser {
public:
    enum class CommandType {
        MemoryAccess,
        Arithmetic,
        Push,
        Pop,
        Add,
        Subtract,
        Negative,
        Equals,
        GreaterThan,
        LessThan,
        And,
        Or,
        Not,
        Constant,
        Local,
        Static,
        This,
        That,
        Offset
    };

    static std::vector<CommandType> parse(std::vector<std::string> const&);
private:
    static std::map<std::string, CommandType> command_enum_map;
};