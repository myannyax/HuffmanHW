#include "HuffTree.h"

#include <utility>

TreeNode::TreeNode(uint32_t freq, uint32_t data, std::shared_ptr<TreeNode> left, std::shared_ptr<TreeNode> right, bool is_internal_node) : data(data),
                                            freq(freq), left(std::move(left)), right(std::move(right)), is_internal_node(is_internal_node) {};

void HuffTree::add(const std::shared_ptr<TreeNode>& elem) {
    q.push(elem);
}

void HuffTree::build() {
    while (q.size() > 1) {
        std::shared_ptr<TreeNode> min1 = q.top();
        q.pop();
        std::shared_ptr<TreeNode> min2 = q.top();
        q.pop();
        q.push(std::make_shared<TreeNode>(min1->freq + min2->freq, 0, std::move(min1), std::move(min2), true));
    }
    head = q.top();
}

const std::shared_ptr<TreeNode>& HuffTree::get_head() const {
    return head;
}

size_t HuffTree::q_size() const {
    return q.size();
}

bool NodeComp::operator()(const std::shared_ptr<TreeNode> &a, const std::shared_ptr<TreeNode> &b) const {
    return a->freq > b->freq;
}
