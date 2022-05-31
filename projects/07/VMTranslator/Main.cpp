/*
 *  Copyright 2022, Hutton Grabiel
 */

#include <Lexer.h>
#include <Parser.h>
#include <Codegen.h>
#include <fstream>
#include <iostream>
#include <filesystem>

void output_hack_asm(char const* path) {
    // Open file for writing, translate file, close file 

    std::vector<std::string> file_paths;
    std::string path_as_string = static_cast<std::string>(path);

    bool is_directory = false;
    bool is_file = false; 

    if (!std::filesystem::is_regular_file(path) && std::filesystem::exists(path)) {
        is_directory = true;
        for (auto const& file : std::filesystem::directory_iterator(path)) {
            std::string file_path_as_string = file.path().string();
            if (file_path_as_string.substr(file_path_as_string.length()-3, 3) == ".vm")
                file_paths.push_back(file_path_as_string);
        }
    }
    else if (std::filesystem::is_regular_file(path)) {
        is_file = true;
        file_paths.push_back(path_as_string);
    }

    std::string out_file_name;
    if (is_directory) {
        // FIXME: Put this in a function
        // FIXME: Find a more specific name for path_name
        std::string path_name;
        size_t start_index = path_as_string.length()-1;
        if (path_as_string[start_index] == '/')
            start_index--;
            
        for (size_t i = start_index; i >= 0; i--) {
            if (path_as_string[i] == '/')
                break;
            path_name.insert(0, 1, path_as_string[i]);
        }

        out_file_name = path_as_string + path_name + ".asm";
    }
    else if (is_file)
        out_file_name = path_as_string.substr(0, path_as_string.length()-3) + ".asm";

    std::ofstream output_file(out_file_name);

    for (auto const& file : file_paths) {
        std::cout << file << std::endl;
        std::ifstream input_source_code(file);
        input_source_code.clear();
        input_source_code.seekg(0);

        if (!input_source_code.is_open()) {
            std::cerr << "\033[01;31mError: \033[0mFile '" + file + "' not found!! Check your paths!" << std::endl;
            exit(1);
        }

        std::string instruction;
        while (getline(input_source_code, instruction)) {
            std::vector<std::string> tokens = Lexer::lex(instruction);

            if (!Lexer::is_lexable_line(instruction))
                continue;
        
            std::vector<Parser::CommandType> parsed_instruction = Parser::parse(tokens);

            std::string output = Codegen::generate_hack_asm(parsed_instruction, tokens, instruction, static_cast<std::string>(path));

            Codegen::unique_identifier++;

            output_file << output;
        }

        input_source_code.close();
    }

    std::string infinite_termination;
    infinite_termination.append("(end)\n");
    infinite_termination.append("@end\n");
    infinite_termination.append("0; JMP\n");

    output_file << infinite_termination;

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

