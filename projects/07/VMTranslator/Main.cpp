/*
 *  Copyright 2022, Hutton Grabiel
 */

#include <Lexer.h>
#include <Parser.h>
#include <Codegen.h>
#include <fstream>
#include <iostream>

void output_hack_asm(char const* file_name) {
    // Open file for writing, translate file, close file 

    std::ifstream input_source_code(file_name);
    if (!input_source_code.is_open()) {
        std::cerr << "\033[01;31mError: \033[0mFile '" << file_name << "' not found!" << std::endl;
        exit(1);
    }

    std::string out_file_name = static_cast<std::string>(file_name).substr(0, static_cast<std::string>(file_name).length()-3) + ".asm";
    std::ofstream output_file(out_file_name);

    std::string instruction;
    while (getline(input_source_code, instruction)) {
        std::vector<std::string> tokens = Lexer::lex(instruction);

        if (!Lexer::is_lexable_line(instruction))
            continue;
    
        std::vector<Parser::CommandType> parsed_instruction = Parser::parse(tokens);

        std::string output = Codegen::generate_hack_asm(parsed_instruction, tokens, instruction, static_cast<std::string>(file_name));

        Codegen::unique_identifier++;

        output_file << output;
    }

    std::string infinite_termination;
    infinite_termination.append("(end)\n");
    infinite_termination.append("@end\n");
    infinite_termination.append("0; JMP\n");

    output_file << infinite_termination;

    input_source_code.close();
    output_file.close();
}

int main(int argc, char* argv[]) {
    // Open file provided in CLI args, pass to output_hack_asm, close file

    if (argc < 2) {
        std::cerr << "\033[01;31mError: \033[0mPlease provide source file." << std::endl;
        exit(1);
    }

    output_hack_asm(argv[1]);

    return 0;
}

