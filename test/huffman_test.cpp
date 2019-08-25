#include <cstring>
#include "huffman_test.h"

template<typename InputIterator1, typename InputIterator2>
bool
range_equal(InputIterator1 first1, InputIterator1 last1,
            InputIterator2 first2, InputIterator2 last2)
{
    while(first1 != last1 && first2 != last2)
    {
        if(*first1 != *first2) return false;
        ++first1;
        ++first2;
    }
    return (first1 == last1) && (first2 == last2);
}

bool compare_files(const std::string& filename1, const std::string& filename2)
{
    std::ifstream file1(filename1);
    std::ifstream file2(filename2);

    std::istreambuf_iterator<char> begin1(file1);
    std::istreambuf_iterator<char> begin2(file2);

    std::istreambuf_iterator<char> end;

    return range_equal(begin1, end, begin2, end);
}

void huffman_test::test_NodeComp() {
    std::shared_ptr<TreeNode> a = std::make_shared<TreeNode>(12, uint32_t('a'));
    std::shared_ptr<TreeNode> b = std::make_shared<TreeNode>(32, uint32_t('b'));
    NodeComp c;
    DO_CHECK(!c(a, b));
}

void huffman_test::test_HuffTree_built() {
    HuffTree tree;
    for (uint32_t i = 1; i < 13; i += 4)
        tree.add(std::make_shared<TreeNode>(i, uint32_t('a' + i)));
    tree.build();
    bool exp1 = (tree.get_head()->freq == 15);
    bool exp2 = (tree.get_head()->right->freq == 9);
    bool exp3 = (tree.get_head()->left->freq == 6);
    bool exp4 = (tree.get_head()->left->right->freq == 5);
    bool exp5 = (tree.get_head()->left->left->freq == 1);
    DO_CHECK(exp1 && exp2 && exp3 && exp4 && exp5);
}

void huffman_test::test_calculate_codes() {
    HuffmanArchiver h;
    h.calculate_codes("../test/test_f/s_text");
    const auto& codes = h.get_codes_coding();
    bool exp1 = codes.at('H').size() <= 3 && codes.at('H').size() >=2;
    bool exp2 = codes.at('o').size() <= 3 && codes.at('o').size() >=2;
    bool exp3 = codes.at('\n').size() <= 3 && codes.at('\n').size() >=2;
    bool exp4 = codes.at('e').size() <= 3 && codes.at('e').size() >=2;
    bool exp5 = codes.at('l').size() == 2 && codes.at('l').size() >=2;
    bool exp6 = codes.at('!').size() <= 3 && codes.at('!').size() >=2;
    DO_CHECK(exp1 && exp2 && exp3 && exp4 && exp5 && exp6);
}

void huffman_test::test_write_and_load_codes() {
    HuffmanArchiver h;
    h.calculate_codes("../test/test_f/s_text");
    h.write_codes_to_buff();
    h.load_codes_from_buff();
    const auto& codes = h.get_codes_coding();
    const auto& de_codes = h.get_codes_decoding();
    bool ch = true;
    for(const auto& elem: codes) {
        if (de_codes.find(elem.second) == de_codes.end()) {
            ch &= false;
            break;
        }
        ch &= (de_codes.at(elem.second) == elem.first);
        if(!ch) break;
    }
    DO_CHECK(ch);
}

void huffman_test::test_code_decode(const std::string &in_n, const std::string &out_n) {
    HuffmanArchiver h1, h2;
    h1.code(in_n, "../test/test_f/tmp");
    h2.code("../test/test_f/tmp", "../test/test_f/tmp2");
    HuffmanArchiver h4, h3;
    h3.decode("../test/test_f/tmp2", "../test/test_f/tmp");
    h4.decode("../test/test_f/tmp", out_n);
    std::remove("../test/test_f/tmp");
    std::remove("../test/test_f/tmp2");
    DO_CHECK(compare_files(in_n, out_n));
}

void huffman_test::runAllTests() {
    test_NodeComp();
    test_HuffTree_built();
    test_calculate_codes();
    test_write_and_load_codes();
    test_code_decode("../test/test_f/text", "../test/test_f/text_n");
    std::remove("../test/test_f/text_n");
    test_code_decode("../test/test_f/pic.jpg", "../test/test_f/pic_n.jpg");
    std::remove("../test/test_f/pic_n.jpg");
}
