/*
 *  Copyright 2022, Hutton Grabiel
 */

#include <Codegen.h>

int Codegen::m_continue_counter;

std::string Codegen::generate_hack_asm(std::vector<Parser::CommandType> const& parsed_instruction, std::vector<std::string> const& tokens, std::string const& instruction, std::string const& source_code_file_name)
{
    std::string output;

    output.append("// Instruction: " + instruction + "\n");

    m_continue_counter++;

    // :| yikes, or just make fun of nand2tetris for their space occupying assembly language
    auto parsed_instruction_iterator = parsed_instruction.begin();
    while (parsed_instruction_iterator != parsed_instruction.end()) {
        switch(*parsed_instruction_iterator) {
        case Parser::CommandType::Add:
            output.append(artithmetic_asm_gen(Arithmetic::Add));
            break;
        case Parser::CommandType::Subtract:
            output.append(artithmetic_asm_gen(Arithmetic::Subtract));
            break;
        case Parser::CommandType::Negative:
            output.append("@SP\n");
            output.append("M=M-1\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("D=M\n");
            output.append("D=-D\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("M=D\n");
            output.append("@SP\n");
            output.append("M=M+1\n");
            break;
        case Parser::CommandType::Equals:
            output.append(comparison_operator_asm_gen(ComparisonOperator::EqualTo)); 
            break;
        case Parser::CommandType::GreaterThan:
            output.append(comparison_operator_asm_gen(ComparisonOperator::GreaterThan)); 
            break;
        case Parser::CommandType::LessThan:
            output.append(comparison_operator_asm_gen(ComparisonOperator::LessThan)); 
            break;
        case Parser::CommandType::And:
            output.append("@SP\n");
            output.append("M=M-1\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("D=M\n");
            output.append("@R13\n");
            output.append("M=D\n");
            output.append("@SP\n");
            output.append("M=M-1\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("D=M\n");
            output.append("@R13\n");
            output.append("D=M&D\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("M=D\n");
            output.append("@SP\n");
            output.append("M=M+1\n");
            break;
        case Parser::CommandType::Or:
            output.append("@SP\n");
            output.append("M=M-1\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("D=M\n");
            output.append("@R13\n");
            output.append("M=D\n");
            output.append("@SP\n");
            output.append("M=M-1\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("D=M\n");
            output.append("@R13\n");
            output.append("D=M|D\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("M=D\n");
            output.append("@SP\n");
            output.append("M=M+1\n");
            break;
        case Parser::CommandType::Not:
            output.append("@SP\n");
            output.append("M=M-1\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("D=M\n");
            output.append("D=!D\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("M=D\n");
            output.append("@SP\n");
            output.append("M=M+1\n");
            break;
        case Parser::CommandType::Push:
            if (*(parsed_instruction_iterator+1) == Parser::CommandType::Constant) {
                output.append("@" + tokens[tokens.size()-1] + "\n");
                output.append("D=A\n");
            }
            else {
                output.append(label_name(*(parsed_instruction_iterator+1), source_code_file_name, tokens));
                output.append("A=M\n");
                output.append("D=M\n");
            }
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("M=D\n");
            output.append("@SP\n");
            output.append("M=M+1\n");
            break;
        case Parser::CommandType::Pop:
            output.append("@" + tokens[2] + "\n");
            output.append("D=A\n");
            output.append(label_name(*(parsed_instruction_iterator+1), source_code_file_name, tokens));
            output.append("M=M+D\n");
            output.append("@SP\n");
            output.append("M=M-1\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("D=M\n");
            output.append(label_name(*(parsed_instruction_iterator+1), source_code_file_name, tokens));
            output.append("M=D\n");
            output.append("@" + tokens[2] + "\n");
            output.append("D=A\n");
            output.append(label_name(*(parsed_instruction_iterator+1), source_code_file_name, tokens));
            output.append("M=M-D\n");
            break;
        case Parser::CommandType::Constant:
        case Parser::CommandType::Local:
        case Parser::CommandType::Static:
        case Parser::CommandType::This:
        case Parser::CommandType::That:
        case Parser::CommandType::Argument:
        case Parser::CommandType::Temp:
        default:
            break;
        }
        parsed_instruction_iterator++;
    }

    return output; 
}

std::string Codegen::artithmetic_asm_gen(Arithmetic arithmetic)
{
    std::string output;

    output.append("@SP\n");
    output.append("M=M-1\n");
    output.append("@SP\n");
    output.append("A=M\n");
    output.append("D=M\n");
    output.append("@R13\n");
    output.append("M=D\n");
    output.append("@SP\n");
    output.append("M=M-1\n");
    output.append("@SP\n");
    output.append("A=M\n");
    output.append("D=M\n");
    output.append("@R13\n");

    switch (arithmetic) {
    case Arithmetic::Add:
        output.append("D=D+M\n");
        break;
    case Arithmetic::Subtract:
        output.append("D=D-M\n");
        break;
    }

    output.append("@SP\n");
    output.append("A=M\n");
    output.append("M=D\n");
    output.append("@SP\n");
    output.append("M=M+1\n");

    return output;
}

std::string Codegen::comparison_operator_asm_gen(ComparisonOperator comparison_operator)
{
    std::string output;

    output.append("@SP\n");
    output.append("M=M-1\n");
    output.append("@SP\n");
    output.append("A=M\n");
    output.append("D=M\n");
    output.append("@R13\n");
    output.append("M=D\n");
    output.append("@SP\n");
    output.append("M=M-1\n");
    output.append("@SP\n");
    output.append("A=M\n");
    output.append("D=M\n");
    output.append("@R13\n");
    output.append("D=D-M\n");
    output.append("@true" + std::to_string(m_continue_counter) + "\n");

    switch (comparison_operator) {
    case ComparisonOperator::EqualTo:
        output.append("D; JEQ\n");
        break;
    case ComparisonOperator::LessThan:
        output.append("D; JLT\n");
        break;
    case ComparisonOperator::GreaterThan:
        output.append("D; JGT\n");
        break;
    }

    output.append("D=0\n");
    output.append("@continue" + std::to_string(m_continue_counter) + "\n");
    output.append("0; JMP\n");
    output.append("(true" + std::to_string(m_continue_counter) + ")\n");
    output.append("    D=-1\n");
    output.append("(continue" + std::to_string(m_continue_counter) + ")\n");
    output.append("@SP\n");
    output.append("A=M\n");
    output.append("M=D\n");
    output.append("@SP\n");
    output.append("M=M+1\n");

    return output;
}

std::string Codegen::label_name(Parser::CommandType const& command_type, std::string const& source_code_file_name, std::vector<std::string> const& tokens)
{
    std::string output;

    if (command_type == Parser::CommandType::Local) 
        output.append("@LCL\n");
    else if (command_type == Parser::CommandType::Static)
        output.append("@" + source_code_file_name.substr(0, source_code_file_name.length()-3) + "." + tokens[2] + "\n");
    else if (command_type == Parser::CommandType::This) 
        output.append("@" + tokens[2] + "\n");
    else if (command_type == Parser::CommandType::That)
        output.append("@THAT\n");
    else if (command_type == Parser::CommandType::Argument)
        output.append("@ARG\n");
    else if (command_type == Parser::CommandType::Temp)
        output.append("@" + std::to_string(Codegen::to_int(tokens[2]) + 5) + "\n");

    return output;
}