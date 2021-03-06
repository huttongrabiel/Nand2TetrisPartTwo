/*
 *  Copyright 2022, Hutton Grabiel
 */

#include <Parser.h>

std::map<std::string, Parser::CommandType> Parser::command_enum_map  = {
            {"add",      Parser::CommandType::Add},
            {"sub",      Parser::CommandType::Subtract},
            {"neg",      Parser::CommandType::Negate},
            {"eq",       Parser::CommandType::Equals},
            {"gt",       Parser::CommandType::GreaterThan},
            {"lt",       Parser::CommandType::LessThan},
            {"and",      Parser::CommandType::And},
            {"or",       Parser::CommandType::Or},
            {"not",      Parser::CommandType::Not},
            {"push",     Parser::CommandType::Push},
            {"pop",      Parser::CommandType::Pop},
            {"constant", Parser::CommandType::Constant},
            {"local",    Parser::CommandType::Local},
            {"static",   Parser::CommandType::Static},
            {"this",     Parser::CommandType::This},
            {"that",     Parser::CommandType::That},
            {"argument", Parser::CommandType::Argument},
            {"temp",     Parser::CommandType::Temp},
            {"pointer",  Parser::CommandType::Pointer},
            {"goto",     Parser::CommandType::Goto},
            {"if-goto",  Parser::CommandType::IfGoto},
            {"label",    Parser::CommandType::Label},
            {"function", Parser::CommandType::Function},
            {"call",     Parser::CommandType::Call},
            {"return",   Parser::CommandType::Return}
};

std::vector<Parser::CommandType> Parser::parse(std::vector<std::string> const& tokens)
{
    std::vector<Parser::CommandType> parsed_instruction;

    // token can equal push, pop, add, sub, neg, eq, gt, lt, and, or, not, constant, local, static, this, that, offset

    if (tokens.size() == 1) {
        auto command = Parser::command_enum_map.find(tokens[0]);
        parsed_instruction.push_back(command->second);
    }
    else {
        auto push_or_pop = Parser::command_enum_map.find(tokens[0]);
        parsed_instruction.push_back(push_or_pop->second);

        auto memory_segment = Parser::command_enum_map.find(tokens[1]);
        parsed_instruction.push_back(memory_segment->second);
    }

    return parsed_instruction;
}