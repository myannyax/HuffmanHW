#include "HuffTree.h"

TreeNode::TreeNode(uint32_t freq, uint32_t data, TreeNode *left, TreeNode *right, bool is_internal_node)
        : data(data), freq(freq), left(left), right(right), is_internal_node(is_internal_node) {}

void HuffTree::add(TreeNode *elem) {
    q.push(elem);
}

void HuffTree::build() {
    while (q.size() > 1) {
        TreeNode *min1 = q.top();
        q.pop();
        TreeNode *min2 = q.top();
        q.pop();
        q.push(new TreeNode(min1->freq + min2->freq, 0, min1, min2, true));
    }
    head = std::unique_ptr<TreeNode>(q.top());
}

const TreeNode *HuffTree::get_head() const {
    return head.get();
}

size_t HuffTree::q_size() const {
    return q.size();
}

bool NodeComp::operator()(const TreeNode *a, const TreeNode *b) const {
    return a->freq > b->freq;
}
