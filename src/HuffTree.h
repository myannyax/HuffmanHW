#ifndef HW_03_HUFFTREE_H
#define HW_03_HUFFTREE_H

#include <queue>
#include <memory>

class TreeNode {
public:
    TreeNode(uint32_t freq, uint32_t data, std::shared_ptr<TreeNode> left = nullptr, std::shared_ptr<TreeNode> right = nullptr,
             bool is_internal_node = false);

    uint32_t freq;
    uint32_t data;
    bool is_internal_node;
    std::shared_ptr<TreeNode> left, right;
};

struct NodeComp {
    bool operator()(const std::shared_ptr<TreeNode>& a, const std::shared_ptr<TreeNode>& b) const;
};

class HuffTree {
public:
    void add(const std::shared_ptr<TreeNode>& elem);

    void build();

    const std::shared_ptr<TreeNode>& get_head() const;

    size_t q_size() const;

private:
    std::priority_queue<std::shared_ptr<TreeNode>, std::vector<std::shared_ptr<TreeNode>>, NodeComp> q;
    std::shared_ptr<TreeNode> head;
};


#endif //HW_03_HUFFTREE_H
