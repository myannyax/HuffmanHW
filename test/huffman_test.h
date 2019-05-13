#ifndef HW_03_HUFFMAN_TEST_H
#define HW_03_HUFFMAN_TEST_H


#include "autotest.h"
#include "../src/huffman.h"

class huffman_test : public autotest {
public:
    void test_NodeComp();

    void test_HuffTree_built();

    void test_calculate_codes();

    void test_write_and_load_codes();

    void test_code_decode(const std::string &in_n, const std::string &out_n);

    void runAllTests() override;
};


#endif //HW_03_HUFFMAN_TEST_H
