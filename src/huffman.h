#include <cstddef>
#include <vector>
#include <unordered_map>
#include <queue>
#include <memory>
#include <fstream>
#include <iostream>
#include "HuffTree.h"

class HuffmanArchiver {
public:
    void calculate_codes(const std::string &in_n);

    void _calculate_codes(const TreeNode *node, std::vector<bool> code);

    void write_codes_to_buff();

    void load_codes_from_buff();

    void code(const std::string &in_n, const std::string &out_n);

    void decode(const std::string &in_n, const std::string &out_n);

    size_t get_source_size() const;

    size_t get_s_size() const;

    size_t get_f_size() const;

    const std::unordered_map<uint32_t, std::vector<bool>>& get_codes_coding() const;

    const std::unordered_map<std::vector<bool>, uint32_t>& get_codes_decoding() const;

    class huff_exception: public std::exception
    {
    public:
        const char* what() const noexcept override;
    };

private:
    size_t source_size = 0;
    size_t s_size = 0;
    size_t f_size = 0;
    std::vector<bool> buff;
    size_t buff_pos = 0;
    std::unordered_map<uint32_t, std::vector<bool>> codes_coding;
    std::unordered_map<std::vector<bool>, uint32_t> codes_decoding;
    HuffTree tree;
};
