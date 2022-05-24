/*
 *  Copyright 2022, Hutton Grabiel
 */

#include <Codegen.h>

std::string Codegen::generate_hack_asm(std::vector<Parser::CommandType> const& parsed_instruction, std::vector<std::string> const& tokens, std::string const& instruction, std::string const& output_file_name)
{
    std::string output;

    output.append("// Instruction: " + instruction + "\n");

    // :| yikes, or just make fun of nand2tetris for their space occupying assembly language
    auto parsed_instruction_iterator = parsed_instruction.begin();
    while (parsed_instruction_iterator != parsed_instruction.end()) {
        switch(*parsed_instruction_iterator) {
        case Parser::CommandType::Add:
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
            output.append("D=D+M\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("M=D\n");
            output.append("@SP\n");
            output.append("M=M=1\n");
            break;
        case Parser::CommandType::Subtract:
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
            output.append("D=M-D\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("M=D\n");
            output.append("@SP\n");
            output.append("M=M=1\n");
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
            output.append("(true)\n");
            output.append("    D=1\n");
            output.append("(false)\n");
            output.append("    D=0\n");
            output.append("@SP\n");
            output.append("M=M-1\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("D=M\n");
            output.append("@true\n");
            output.append("D; JEQ\n");
            output.append("@false\n");
            output.append("D; JNE\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("M=D\n");
            output.append("@SP\n");
            output.append("M=M+1\n");
            break;
        case Parser::CommandType::GreaterThan:
            output.append("(true)\n");
            output.append("    D=1\n");
            output.append("(false)\n");
            output.append("    D=0\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("D=M\n");
            output.append("@R13\n");
            output.append("M=D\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("M=M-1\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("D=M\n");
            output.append("@SP\n");
            output.append("M=M-1\n");
            output.append("@R13\n");
            output.append("D=D-M\n");
            output.append("@true\n");
            output.append("D;JGT\n");
            output.append("@false\n");
            output.append("D;JLT\n");
            output.append("@false\n");
            output.append("D;JEQ\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("M=D\n");
            output.append("@SP\n");
            output.append("M=M+1\n");
            break;
        case Parser::CommandType::LessThan:
            output.append("(true)\n");
            output.append("    D=1\n");
            output.append("(false)\n");
            output.append("    D=0\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("D=M\n");
            output.append("@R13\n");
            output.append("M=D\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("M=M-1\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("D=M\n");
            output.append("@SP\n");
            output.append("M=M-1\n");
            output.append("@R13\n");
            output.append("D=D-M\n");
            output.append("@true\n");
            output.append("D;JLT\n");
            output.append("@false\n");
            output.append("D;JGT\n");
            output.append("@false\n");
            output.append("D;JEQ\n");
            output.append("@SP\n");
            output.append("A=M\n");
            output.append("M=D\n");
            output.append("@SP\n");
            output.append("M=M+1\n");
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
        case Parser::CommandType::Local:
        case Parser::CommandType::Static:
        case Parser::CommandType::This:
        case Parser::CommandType::That:
        case Parser::CommandType::Push:
        case Parser::CommandType::Pop:
        case Parser::CommandType::Constant:
        default:
            std::cout << "Instruction not found" << std::endl;
        }
        parsed_instruction_iterator++;
    }

    return output; 
}