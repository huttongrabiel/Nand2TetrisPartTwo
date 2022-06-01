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
    static std::string sys_init();

    static int unique_identifier;
private:
    enum class Arithmetic {
        Add,
        Subtract,
        Negate
    };

    enum class ComparisonOperator {
        LessThan,
        GreaterThan,
        EqualTo
    };

    enum class BitwiseOperator {
        And,
        Or,
        Not
    };
    
    static std::string generate_push_assembly(Parser::CommandType const&, std::string const&, std::string const&);
    static std::string generate_pop_assembly(Parser::CommandType const&, std::string const&, std::string const&);
    static std::string artithmetic_asm_gen(Arithmetic);
    static std::string comparison_operator_asm_gen(ComparisonOperator);
    static std::string bitwise_operator_asm_gen(BitwiseOperator);
    static std::string push_d_asm_gen();
    static std::string pop_d_asm_gen();
    static std::string push_state();
    static std::string label_name(Parser::CommandType const&, std::string const&, std::string const&);
    static std::string parse_file_name(std::string const&);

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