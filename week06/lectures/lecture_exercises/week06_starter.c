#include <stddef.h>

typedef struct TreeNode {
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

size_t tree_size(const TreeNode *root) {
    /* TODO: base case plus the sizes of both recursive subtrees. */
    (void)root;
    return 0;
}

void tree_print_postorder(const TreeNode *root) {
    /* TODO: recurse left/right, then visit root. */
    (void)root;
}

void tree_destroy(TreeNode *root) {
    /* TODO: destroy owned children before their parent. */
    (void)root;
}
