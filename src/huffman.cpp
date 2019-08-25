#include "huffman.h"

template<class T>
void add_bytes_to_vector(size_t n, std::vector<bool> &res, T &b) {
    for (size_t i = 0; i < n * 8u; i++) {
        res.push_back(b & (1u << ((n * 8u - 1) - i)));
    }
}

template<class T>
void get_bytes_from_vector(size_t n, const std::vector<bool> &v, T &to, size_t &t_pos) {
    if (v.size() < t_pos + n * 8u) throw HuffmanArchiver::huff_exception();
    for (size_t i = 0; i < n * 8u; i++) {
        to |= (v[t_pos + i] << ((n * 8u - 1) - i));
    }
    t_pos += n * 8;
}

void HuffmanArchiver::_calculate_codes(const std::shared_ptr<TreeNode>& node, std::vector<bool> code) {
    if (node == nullptr) return;

    if (!node->is_internal_node) {
        if(code.empty()) code.push_back(0);
        codes_coding.insert({node->data, std::move(code)});
        return;
    }
    code.push_back(false);
    _calculate_codes(node->left, code);
    code.pop_back();
    code.push_back(true);
    _calculate_codes(node->right, code);
    code.pop_back();
}

void HuffmanArchiver::calculate_codes(const std::string &in_n) {
    std::ifstream file;
    file.open(in_n, std::ios::binary);
    uint8_t a;
    std::unordered_map<uint8_t, uint32_t> w;
    while (file.read(reinterpret_cast<char *>(&a), 1)) {
        w[a]++;
        source_size++;
    }
    for (const auto& elem: w) {
        tree.add(std::make_shared<TreeNode>(elem.second, elem.first));
    }
    w.clear();
    if(!tree.q_size()) throw HuffmanArchiver::huff_exception();
    tree.build();
    _calculate_codes(tree.get_head(), std::vector<bool>());
    file.close();
}

void HuffmanArchiver::write_codes_to_buff() {
    uint32_t c_size = codes_coding.size();
    add_bytes_to_vector(4, buff, c_size);

    for (const auto& elem: codes_coding) {
        uint8_t code_size = elem.second.size();
        add_bytes_to_vector(1, buff, code_size);
        add_bytes_to_vector(1, buff, elem.first);
        for (auto i: elem.second) {
            buff.push_back(i);
        }
    }
    s_size = buff.size() / 8 + bool(buff.size() % 8);
}

void HuffmanArchiver::load_codes_from_buff() {
    uint32_t c_size = 0u;
    get_bytes_from_vector(4, buff, c_size, buff_pos);

    for (size_t i = 0; i < c_size; i++) {
        uint8_t code_size = 0u;
        get_bytes_from_vector(1, buff, code_size, buff_pos);
        uint8_t w = 0u;
        get_bytes_from_vector(1, buff, w, buff_pos);
        std::vector<bool> c;
        for (size_t j = 0; j < code_size; j++) {
            c.push_back(buff[buff_pos + j]);
        }
        buff_pos += code_size;
        codes_decoding[c] = w;
    }
    s_size = buff_pos / 8 + bool(buff_pos % 8);
}

void HuffmanArchiver::code(const std::string &in_n, const std::string &out_n) {
    try {
        buff.clear();
        std::ifstream in;
        std::ofstream out;

        calculate_codes(in_n);

        uint8_t a;

        write_codes_to_buff();

        in.open(in_n, std::ios::binary);
        while (in.read(reinterpret_cast<char *>(&a), 1)) {
            for (auto i: codes_coding[a]) {
                buff.push_back(i);
            }
        }
        in.close();

        f_size = buff.size() / 8u + bool(buff.size() % 8u) - s_size;

        uint32_t b_s = buff.size();
        out.open(out_n, std::ios::binary);
        out.write(reinterpret_cast<char *>(&b_s), 4);

        for (size_t i = 0u; i < buff.size(); i += 8u) {
            a = 0u;
            for (size_t j = i; j < std::min(buff.size(), i + 8u); j++) {
                a |= (buff[j] << (7u - j % 8u));
            }
            out.write(reinterpret_cast<char *>(&a), 1);
        }
        out.close();
    } catch (const std::ios_base::failure &e) {
        std::cerr << "Exception opening/reading/writing closing file\n";
        throw e;
    }
}

void HuffmanArchiver::decode(const std::string &in_n, const std::string &out_n) {
    try {
        buff.clear();
        std::ifstream in;
        in.open(in_n, std::ios::binary);
        std::ofstream out;
        out.open(out_n, std::ios::binary);

        buff_pos = 0u;

        uint32_t bit_cnt = 0u;
        in.read(reinterpret_cast<char *>(&bit_cnt), 4);

        uint8_t a;
        while (in.read(reinterpret_cast<char *>(&a), 1)) {
            for (size_t i = 0u; i < 8u; i++) {
                buff.push_back(a & (1u << (7u - i)));
            }
        }
        in.close();

        load_codes_from_buff();

        std::vector<bool> code;
        source_size = 0;
        while (buff_pos < bit_cnt) {
            code.push_back(buff[buff_pos++]);
            if (codes_decoding.find(code) != codes_decoding.end()) {
                out.write(reinterpret_cast<char *>(&codes_decoding[code]), 1);
                source_size++;
                code.clear();
            }
        }
        f_size = buff.size() / 8u + bool(buff.size() % 8u) - s_size;
        out.close();
    } catch (const std::ios_base::failure &e) {
        std::cerr << "Exception opening/reading/writing closing file\n";
        throw e;
    } catch (const HuffmanArchiver::huff_exception &e) {
        throw e;
    }
}

size_t HuffmanArchiver::get_source_size() const {
    return source_size;
}

size_t HuffmanArchiver::get_s_size() const {
    return s_size;
}

size_t HuffmanArchiver::get_f_size() const {
    return f_size;
}

const std::unordered_map<uint32_t, std::vector<bool>> &HuffmanArchiver::get_codes_coding() const {
    return codes_coding;
}

const std::unordered_map<std::vector<bool>, uint32_t> &HuffmanArchiver::get_codes_decoding() const {
    return codes_decoding;
}

const char *HuffmanArchiver::huff_exception::what() const noexcept {
    return "Incorrect file\n";
}
