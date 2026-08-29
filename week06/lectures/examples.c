#include <stddef.h>
#include <stdio.h>

typedef struct TreeNode {
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

static size_t tree_size(const TreeNode *root) {
    return root == NULL ? 0 : 1 + tree_size(root->left) + tree_size(root->right);
}

static void print_preorder(const TreeNode *root) {
    if (root == NULL) {
        return;
    }
    printf("%d ", root->value);
    print_preorder(root->left);
    print_preorder(root->right);
}

static void print_postorder(const TreeNode *root) {
    if (root == NULL) {
        return;
    }
    print_postorder(root->left);
    print_postorder(root->right);
    printf("%d ", root->value);
}

int main(void) {
    TreeNode left = {1, NULL, NULL};
    TreeNode right = {3, NULL, NULL};
    TreeNode root = {2, &left, &right};
    printf("size = %zu\npre: ", tree_size(&root));
    print_preorder(&root);
    printf("\npost: ");
    print_postorder(&root);
    putchar('\n');
    return 0;
}
