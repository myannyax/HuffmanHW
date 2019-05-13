#ifndef HW_03_HUFFTREE_H
#define HW_03_HUFFTREE_H

#include <queue>
#include <memory>

class TreeNode {
public:
    TreeNode(uint32_t freq, uint32_t data, TreeNode *left = nullptr, TreeNode *right = nullptr,
             bool is_internal_node = false);

    uint32_t freq;
    uint32_t data;
    bool is_internal_node;
    std::unique_ptr<TreeNode> left, right;
};

struct NodeComp {
    bool operator()(const TreeNode *a, const TreeNode *b) const;
};

class HuffTree {
public:
    void add(TreeNode *elem);

    void build();

    const TreeNode *get_head() const;

    size_t q_size() const;

private:
    std::priority_queue<TreeNode *, std::vector<TreeNode *>, NodeComp> q;
    std::unique_ptr<TreeNode> head;
};


#endif //HW_03_HUFFTREE_H
