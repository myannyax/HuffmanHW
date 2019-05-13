#include <fstream>
#include <iostream>
#include <cstdio>
#include "huffman.h"

int main(int argc, char *argv[]) {
    if (argc < 1) {
        return 1;
    }
    std::string source;
    std::string destination;
    bool enc = false, dec = false;
    bool in = false, out = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-c") {
            if (dec) {
                std::cerr << "Wrong arguments\n";
                return 0;
            }
            enc = true;
        } else if (arg == "-u") {
            if (enc) {
                std::cerr << "Wrong arguments\n";
                return 0;
            }
            dec = true;
        } else if (arg == "-f") {
            source = argv[i + 1];
            i++;
            in = true;
        } else if (arg == "-o") {
            destination = argv[i + 1];
            i++;
            out = true;
        } else {
            std::cerr << "Wrong arguments\n";
            return 0;
        }
    }
    if (!(in && out)) {
        std::cerr << "Wrong arguments\n";
        return 0;
    }
    if (enc) {
        try {
            HuffmanArchiver h1, h2;
            h1.code(source, "tmp");
            h2.code("tmp", destination);
            std::remove("tmp");
            std::cout << h1.get_source_size() << "\n" << h2.get_f_size() << "\n" << h2.get_s_size() << "\n";
        } catch (const std::ios_base::failure &e) {
            std::remove("tmp");
        } catch (const HuffmanArchiver::huff_exception &e) {
            std::cerr << e.what();
            std::remove("tmp");
        }
    } else if (dec) {
        try {
            HuffmanArchiver h1, h2;
            h1.decode(source, "tmp");
            h2.decode("tmp", destination);
            std::remove("tmp");
            std::cout << h2.get_source_size() << "\n" << h1.get_f_size() << "\n" << h1.get_s_size() << "\n";
        } catch (const std::ios_base::failure &e) {
            std::remove("tmp");
        } catch (const HuffmanArchiver::huff_exception &e) {
            std::cerr << e.what();
            std::remove("tmp");
        }
    } else {
        std::cerr << "Wrong arguments\n";
        return 1;
    }
}
