/*
 *  Copyright 2022, Hutton Grabiel
 */

#include <Lexer.h>

std::vector<std::string> Lexer::lex(std::string line)
{
    std::vector<std::string> tokens;

    std::string string_builder;
    for (size_t i = 0; i <= line.length(); i++) {

        if (line[i] != ' ' && i != line.length())
            string_builder.push_back(line[i]);

        if (line[i] == ' ' || line[i] == '\n' || i == line.length()) {
            tokens.push_back(string_builder);
            string_builder.clear();
        }
    }

    return tokens;
}