/*
 *  Copyright 2022, Hutton Grabiel
 */

#include <Codegen.h>

int Codegen::unique_identifier;

std::string Codegen::generate_hack_asm(std::vector<Parser::CommandType> const& parsed_instruction, std::vector<std::string> const& tokens, std::string const& instruction, std::string const& source_code_file_name)
{
    std::string output;

    output.append("// Instruction: " + instruction + "\n");

    unique_identifier++;

    switch(parsed_instruction[0]) {
    case Parser::CommandType::Add:
        output.append(artithmetic_asm_gen(Arithmetic::Add));
        break;
    case Parser::CommandType::Subtract:
        output.append(artithmetic_asm_gen(Arithmetic::Subtract));
        break;
    case Parser::CommandType::Negate:
        output.append(artithmetic_asm_gen(Arithmetic::Negate));
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
        output.append(bitwise_operator_asm_gen(BitwiseOperator::And));
        break;
    case Parser::CommandType::Or:
        output.append(bitwise_operator_asm_gen(BitwiseOperator::Or));
        break;
    case Parser::CommandType::Not:
        output.append(bitwise_operator_asm_gen(BitwiseOperator::Not));
        break;
    case Parser::CommandType::Push:
        output.append(generate_push_assembly(parsed_instruction[1], tokens[2], source_code_file_name)); 
        break;
    case Parser::CommandType::Pop:
        output.append(generate_pop_assembly(parsed_instruction[1], tokens[2], source_code_file_name)); 
        break;
    case Parser::CommandType::Goto:
        output.append("@" + tokens[1] + "\n");
        output.append("0; JMP\n");
        break;
    case Parser::CommandType::IfGoto:
        output.append(pop_d_asm_gen());
        output.append("@" + tokens[1] + "\n");
        output.append("D; JNE\n");
        break;
    case Parser::CommandType::Label:
        output.append("(" + tokens[1] + ")\n");
        break;
    case Parser::CommandType::Function:
        output.append("(" + tokens[1] + "FUNCTION_ENTRY)\n");
        output.append("@" + tokens[2] + "\n");
        output.append("D=A\n");
        output.append("@count\n");
        output.append("M=D\n");
        output.append("(" + tokens[1] + ")\n");
        output.append("@count\n");
        output.append("D=M\n");
        output.append("@CONTINUE" + std::to_string(unique_identifier) + "\n");
        output.append("D; JEQ\n");
        output.append(generate_push_assembly(Parser::CommandType::Constant, "0", source_code_file_name));
        output.append("@count\n");
        output.append("M=M-1\n");
        output.append("@" + tokens[1] + "\n");
        output.append("0;JMP\n");
        output.append("(CONTINUE" + std::to_string(unique_identifier) + ")\n");
        break;
    case Parser::CommandType::Call:
        output.append(push_state());

        // reposition ARG
        output.append("@5\n");
        output.append("D=A\n");
        output.append("@" + tokens[2] + "\n");
        output.append("D=D+A\n");
        output.append("@SP\n");
        output.append("D=M-D\n");
        output.append("@ARG\n");
        output.append("M=D\n");

        // reposition LCL
        output.append("@SP\n");
        output.append("D=M\n");
        output.append("@LCL\n");
        output.append("M=D\n");

       // goto function
        output.append("@" + tokens[1] + "FUNCTION_ENTRY\n");
        output.append("0; JMP\n");

        // return address
        output.append("(returnAddress" + std::to_string(unique_identifier) + ")\n");
        
        break;
    case Parser::CommandType::Return:
        // set endframe
        output.append("@LCL\n");
        output.append("D=M\n");
        output.append("@endframe\n");
        output.append("M=D\n");

        // set return address
        output.append("@5\n");
        output.append("D=A\n");
        output.append("@endframe\n");
        output.append("D=M-D\n");
        output.append("A=D\n");
        output.append("D=M\n");
        output.append("@returnAddress" + std::to_string(unique_identifier) + "\n");
        output.append("M=D\n");

        // put return value in argument 0
        output.append(generate_pop_assembly(Parser::CommandType::Argument, "0", source_code_file_name));

        // set SP to ARG + 1
        output.append("@ARG\n");
        output.append("D=M\n");
        output.append("@SP\n");
        output.append("M=D+1\n");

        // set THAT to endframe-1
        output.append("@endframe\n");
        output.append("D=M\n");
        output.append("D=D-1\n");
        output.append("A=D\n");
        output.append("D=M\n");
        output.append("@THAT\n");
        output.append("M=D\n");
        
        // set THIS to endframe-2
        output.append("@endframe\n");
        output.append("D=M\n");
        output.append("@2\n");
        output.append("D=D-A\n");
        output.append("A=D\n");
        output.append("D=M\n");
        output.append("@THIS\n");
        output.append("M=D\n");

        // set ARG to endframe-3
        output.append("@endframe\n");
        output.append("D=M\n");
        output.append("@3\n");
        output.append("D=D-A\n");
        output.append("A=D\n");
        output.append("D=M\n");
        output.append("@ARG\n");
        output.append("M=D\n");

        // set LCL to endframe-4
        output.append("@endframe\n");
        output.append("D=M\n");
        output.append("@4\n");
        output.append("D=D-A\n");
        output.append("A=D\n");
        output.append("D=M\n");
        output.append("@LCL\n");
        output.append("M=D\n");

        // goto returnAddress
        output.append("@returnAddress" + std::to_string(unique_identifier) + "\n");
        output.append("A=M\n");
        output.append("0;JMP\n");

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

    return output; 
}

std::string Codegen::generate_push_assembly(Parser::CommandType const& next_instruction, std::string const& offset_token, std::string const& source_code_file_name)
{ 
    std::string output;

    output.append("@" + offset_token + "\n");
    output.append("D=A\n");

    if (next_instruction == Parser::CommandType::Temp) {
        output.append(label_name(next_instruction, source_code_file_name, offset_token));
        output.append("A=A+D\n");
        output.append("D=M\n");
    }
    else if (next_instruction == Parser::CommandType::Pointer) {
        if (!to_int(offset_token))
            output.append("@THIS\n");
        else
            output.append("@THAT\n");
        output.append("D=M\n");
    }
    else if (next_instruction != Parser::CommandType::Constant) {
        output.append(label_name(next_instruction, source_code_file_name, offset_token));
        output.append("A=M+D\n");
        output.append("D=M\n");
    }

    output.append(push_d_asm_gen());

    return output;
}

std::string Codegen::generate_pop_assembly(Parser::CommandType const& next_instruction, std::string const& offset_token, std::string const& source_code_file_name)
{ 
    std::string output;

    output.append("@" + offset_token + "\n");
    output.append("D=A\n");
    output.append(label_name(next_instruction, source_code_file_name, offset_token));

    if (next_instruction == Parser::CommandType::Temp)
        output.append("D=D+A\n");
    else if (next_instruction == Parser::CommandType::Pointer)
        output.append("D=A\n");
    else
        output.append("D=D+M\n");

    output.append("@R13\n");
    output.append("M=D\n");
    output.append(pop_d_asm_gen());
    output.append("@R13\n");
    output.append("A=M\n");
    output.append("M=D\n");

    return output;
}

std::string Codegen::artithmetic_asm_gen(Arithmetic arithmetic)
{
    std::string output;

    output.append(pop_d_asm_gen());
   
    if (arithmetic != Arithmetic::Negate) {
        output.append("@R13\n");
        output.append("M=D\n");
        output.append(pop_d_asm_gen());
        output.append("@R13\n");
    }

    switch (arithmetic) {
    case Arithmetic::Add:
        output.append("D=D+M\n");
        break;
    case Arithmetic::Subtract:
        output.append("D=D-M\n");
        break;
    case Arithmetic::Negate:
        output.append("D=-D\n");
        break;
    }

    output.append(push_d_asm_gen());

    return output;
}

std::string Codegen::comparison_operator_asm_gen(ComparisonOperator comparison_operator)
{
    std::string output;

    output.append(pop_d_asm_gen());
    
    output.append("@R13\n");
    output.append("M=D\n");

    output.append(pop_d_asm_gen());
    
    output.append("@R13\n");
    output.append("D=D-M\n");
    output.append("@true" + std::to_string(unique_identifier) + "\n");

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
    output.append("@continue" + std::to_string(unique_identifier) + "\n");
    output.append("0; JMP\n");
    output.append("(true" + std::to_string(unique_identifier) + ")\n");
    output.append("    D=-1\n");
    output.append("(continue" + std::to_string(unique_identifier) + ")\n");

    output.append(push_d_asm_gen());

    return output;
}

std::string Codegen::bitwise_operator_asm_gen(BitwiseOperator BitwiseOperator)
{
    std::string output;

    output.append(pop_d_asm_gen());

    if (BitwiseOperator != BitwiseOperator::Not) {
        output.append("@R13\n");
        output.append("M=D\n");
        output.append(pop_d_asm_gen());
        output.append("@R13\n");
    }

    switch (BitwiseOperator) {
    case BitwiseOperator::And:
        output.append("D=M&D\n");
        break;
    case BitwiseOperator::Or:
        output.append("D=M|D\n");
        break;
    case BitwiseOperator::Not:
        output.append("D=!D\n");
        break;
    }

    output.append(push_d_asm_gen());

    return output;
}

std::string Codegen::push_d_asm_gen()
{
    std::string output;

    output.append("@SP\n");
    output.append("A=M\n");
    output.append("M=D\n");
    output.append("@SP\n");
    output.append("M=M+1\n");

    return output;
}

std::string Codegen::pop_d_asm_gen()
{
    std::string output;

    output.append("@SP\n");
    output.append("M=M-1\n");
    output.append("@SP\n");
    output.append("A=M\n");
    output.append("D=M\n");

    return output;
}

std::string Codegen::push_state()
{
    std::string output;

    // push return address
    output.append("@returnAddress" + std::to_string(unique_identifier) + "\n");
    output.append("D=A\n");
    output.append(push_d_asm_gen());

    // push LCL, ARG, THIS, THAT
    output.append("@LCL\n");
    output.append("D=M\n");
    output.append(push_d_asm_gen());
    output.append("@ARG\n");
    output.append("D=M\n");
    output.append(push_d_asm_gen());
    output.append("@THIS\n");
    output.append("D=M\n");
    output.append(push_d_asm_gen());
    output.append("@THAT\n");
    output.append("D=M\n");
    output.append(push_d_asm_gen());

    return output;
}

std::string Codegen::sys_init()
{
    std::string output;

    output.append("@256\n");
    output.append("D=A\n");
    output.append("@SP\n");
    output.append("M=D\n");

    output.append(push_state());

    output.append("@Sys.initFUNCTION_ENTRY\n");
    output.append("0; JMP\n");

    return output;
}

std::string Codegen::label_name(Parser::CommandType const& command_type, std::string const& source_code_file_name, std::string const& offset_token)
{
    std::string output;

    if (command_type == Parser::CommandType::Local) 
        output.append("@LCL\n");
    else if (command_type == Parser::CommandType::Static)
        output.append("@" + parse_file_name(source_code_file_name) + "." + offset_token + "\n");
    else if (command_type == Parser::CommandType::This || (command_type == Parser::CommandType::Pointer && !to_int(offset_token)))
        output.append("@THIS\n");
    else if (command_type == Parser::CommandType::That || (command_type == Parser::CommandType::Pointer && to_int(offset_token)))
        output.append("@THAT\n");
    else if (command_type == Parser::CommandType::Argument)
        output.append("@ARG\n");
    else if (command_type == Parser::CommandType::Temp)
        output.append("@5\n");

    return output;
}

std::string Codegen::parse_file_name(std::string const& file_name)
{
    int start_index = 0;

    for (int i = file_name.length()-1; i >= 0; i--) {
        if (file_name[i] == '/') 
            break;
        start_index = i;
    }

    return file_name.substr(start_index, file_name.length()-start_index-3);
}