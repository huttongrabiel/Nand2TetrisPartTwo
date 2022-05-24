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

    std::fstream output_file(file_name);
    if (!output_file.is_open()) {
        std::cerr << "\033[01;31mError: \033[0mFile '" << file_name << "' not found!" << std::endl;
        exit(1);
    }

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

