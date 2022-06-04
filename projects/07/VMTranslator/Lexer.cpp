/*
 *  Copyright 2022, Hutton Grabiel
 */

#include <Lexer.h>

std::vector<std::string> Lexer::lex(std::string line)
{
    std::vector<std::string> tokens;

    std::string string_builder;
    for (size_t i = 0; i <= line.length(); i++) {

        if (line[i] == '/')
            break;

        if (line[i] != ' ' && i != line.length() && !(is_new_line(line[i])))
            string_builder.push_back(line[i]);

        if (line[i] == ' ' || line[i] == '\n' || i == line.length() || line[i] == '\r' || line[i] == '\t') {
            tokens.push_back(string_builder);
            string_builder.clear();
        }
    }

    return tokens;
}

bool Lexer::is_lexable_line(std::string line)
{
    bool flag = true;
    size_t blank_char_count = 0;

    if (line.substr(0,2) == "//")
        return false;

    for (auto ch : line) {
        if (is_new_line(ch) && ch == line[0])
            flag = false;
        if (ch == ' ')
            blank_char_count++;
    }

    if (blank_char_count == line.length())
        flag = false;

    return flag;
}

bool Lexer::is_new_line(char ch)
{
    if (ch == 0x0d || ch == 0x0a || ch == 0x0d0a || ch == '\n')
        return true;

    return false;
}

std::string Lexer::trim_path(std::string path, PathType PathType, bool full_trim)
{
    size_t path_length = path.length()-1;

    size_t length_modifier = 0;
    size_t substr_start_index = 0;

    for (size_t i = path_length; i >= 0; i--) {
        if (path[i] == '/' && i != path_length) {
            substr_start_index = i+1;
            break;
        }
        if (path[i] == '/' && i == path_length) 
            length_modifier = 1;
    }

    if (PathType == PathType::Directory)
        return path.substr(substr_start_index, path.length()-substr_start_index-length_modifier);
    else if (PathType == PathType::File && full_trim)
        return path.substr(substr_start_index, path.length()-substr_start_index-length_modifier-3);
    else
        return path.substr(0, path.length()-3);
}
